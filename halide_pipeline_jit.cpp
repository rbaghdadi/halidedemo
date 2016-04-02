#include "Halide.h"

using namespace Halide;

#include <stdio.h>
#include <time.h>
#include <random>
#ifdef DEMO_DISTRIBUTED
#include "mpi.h"
#endif

namespace {

enum ScheduleType {
    CPU, GPU, SC, Invalid
};

#ifdef DEMO_CPU
    ScheduleType schedule = CPU;
    typedef Image<uint8_t> ImTy;
#elif DEMO_GPU
    ScheduleType schedule = GPU;
    typedef Image<uint8_t> ImTy;
#elif DEMO_DISTRIBUTED
    ScheduleType schedule = SC;
    typedef DistributedImage<uint8_t> ImTy;
#else
    ScheduleType schedule = Invalid;
#endif

const float WHITEF = 1.0f, GREYF = 0.5f, BLACKF = 0.0f;
const uint8_t WHITE = 255, GREY = 128, BLACK = 0;
const int nsquares = 3;
const int square_padding = 50;
int w = 0, h = 0;
int square_size = 0;

std::default_random_engine generator(0);
//std::uniform_real_distribution<float> distribution(BLACK, WHITE);
std::normal_distribution<float> distribution(GREYF, 0.2f);

void set_input_image_params(int width, int height) {
    w = width;
    h = height;
    square_size = (w - ((nsquares + 1) * square_padding)) / nsquares;
}
} // end anonymous namespace

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: %s width height\n", argv[0]);
        return 1;
    } else if (schedule == Invalid) {
        printf("Invalid schedule type.\n");
        return 1;
    }

#ifdef DEMO_DISTRIBUTED
    int req = MPI_THREAD_MULTIPLE, prov;
    MPI_Init_thread(&argc, &argv, req, &prov);
    assert(prov == req);
    int rank = 0, numranks = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
#endif

    const int gw = atoi(argv[1]), gh = atoi(argv[2]);
    ImTy input(gw, gh);
    ImTy output(gw, gh);
    Var x("x"), y("y"), z("z"), c("c");

    // Filter parameters
    const int s_sigma = 16;
    const float r_sigma = 0.6f;

    // Convert to float
    Func clamped;
#ifdef DEMO_DISTRIBUTED
    clamped(x, y) = input(clamp(x, 0, input.global_width() - 1),
                          clamp(y, 0, input.global_height() - 1)) * 255.0f;
#else
    clamped(x, y) = input(clamp(x, 0, input.width() - 1),
                          clamp(y, 0, input.height() - 1)) * 255.0f;
#endif

    // Construct the bilateral grid
    RDom r(0, s_sigma, 0, s_sigma);
    Expr val = clamped(x * s_sigma + r.x - s_sigma/2, y * s_sigma + r.y - s_sigma/2);
    val = clamp(val, 0.0f, 1.0f);
    Expr zi = cast<int>(val * (1.0f/r_sigma) + 0.5f);
    Func histogram("histogram");
    histogram(x, y, z, c) = 0.0f;
    histogram(x, y, zi, c) += select(c == 0, val, 1.0f);

    // Blur the grid using a five-tap filter
    Func blurx("blurx"), blury("blury"), blurz("blurz");
    blurz(x, y, z, c) = (histogram(x, y, z-2, c) +
                         histogram(x, y, z-1, c)*4 +
                         histogram(x, y, z  , c)*6 +
                         histogram(x, y, z+1, c)*4 +
                         histogram(x, y, z+2, c));
    blurx(x, y, z, c) = (blurz(x-2, y, z, c) +
                         blurz(x-1, y, z, c)*4 +
                         blurz(x  , y, z, c)*6 +
                         blurz(x+1, y, z, c)*4 +
                         blurz(x+2, y, z, c));
    blury(x, y, z, c) = (blurx(x, y-2, z, c) +
                         blurx(x, y-1, z, c)*4 +
                         blurx(x, y  , z, c)*6 +
                         blurx(x, y+1, z, c)*4 +
                         blurx(x, y+2, z, c));

    // Take trilinear samples to compute the output
    val = clamp(input(x, y), 0.0f, 1.0f);
    Expr zv = val * (1.0f/r_sigma);
    zi = cast<int>(zv);
    Expr zf = zv - zi;
    Expr xf = cast<float>(x % s_sigma) / s_sigma;
    Expr yf = cast<float>(y % s_sigma) / s_sigma;
    Expr xi = x/s_sigma;
    Expr yi = y/s_sigma;
    Func interpolated("interpolated");
    interpolated(x, y, c) =
        lerp(lerp(lerp(blury(xi, yi, zi, c), blury(xi+1, yi, zi, c), xf),
                  lerp(blury(xi, yi+1, zi, c), blury(xi+1, yi+1, zi, c), xf), yf),
             lerp(lerp(blury(xi, yi, zi+1, c), blury(xi+1, yi, zi+1, c), xf),
                  lerp(blury(xi, yi+1, zi+1, c), blury(xi+1, yi+1, zi+1, c), xf), yf), zf);

    // Normalize
    Func bilateral_grid("bilateral_grid");
    bilateral_grid(x, y) = interpolated(x, y, 0)/interpolated(x, y, 1);

    Func clamped_bilateral_grid;
#ifdef DEMO_DISTRIBUTED
    clamped_bilateral_grid(x, y) = bilateral_grid(clamp(x, 0, input.global_width() - 1),
                                                  clamp(y, 0, input.global_height() - 1));
#else
    clamped_bilateral_grid(x, y) = bilateral_grid(clamp(x, 0, input.width() - 1),
                                                  clamp(y, 0, input.height() - 1));
#endif

    Func sobelx, sobely;
    sobelx(x, y) = -1*clamped_bilateral_grid(x-1, y-1) + clamped_bilateral_grid(x+1, y-1) +
        -2*clamped_bilateral_grid(x-1, y) + 2*clamped_bilateral_grid(x+1, y) +
        -1*clamped_bilateral_grid(x-1, y+1) + clamped_bilateral_grid(x+1, y+1);
    sobely(x, y) = -1*clamped_bilateral_grid(x-1, y-1) + -2*clamped_bilateral_grid(x, y-1) + -1*clamped_bilateral_grid(x+1,y-1) +
        clamped_bilateral_grid(x-1, y+1) + 2*clamped_bilateral_grid(x, y+1) + clamped_bilateral_grid(x+1, y+1);

    Func sobel;
    sobel(x, y) = cast<uint8_t>((sqrt(sobelx(x, y) * sobelx(x, y) + sobely(x, y) * sobely(x, y))) * 255);

    Target target = get_jit_target_from_environment();
    if (schedule == GPU) {
        target.set_feature(Target::OpenCL);
        blurz.compute_root().reorder(c, z, x, y).gpu_tile(x, y, 8, 8);
        histogram.reorder(c, z, x, y).compute_at(blurz, Var::gpu_blocks()).gpu_threads(x, y);
        histogram.update().reorder(c, r.x, r.y, x, y).gpu_threads(x, y).unroll(c);
        blurx.compute_root().gpu_tile(x, y, z, 8, 8, 1);
        blury.compute_root().gpu_tile(x, y, z, 8, 8, 1);
        bilateral_grid.compute_root().gpu_tile(x, y, s_sigma, s_sigma);
        sobel.compute_root().gpu_tile(x, y, 8, 8);
        sobelx.compute_at(sobel, Var::gpu_blocks()).gpu_threads(x, y);
        sobely.compute_at(sobel, Var::gpu_blocks()).gpu_threads(x, y);
    } else if (schedule == CPU) {
        histogram.compute_at(blurz, y);
        histogram.update().reorder(c, r.x, r.y, x, y).unroll(c);
        blurz.compute_root().reorder(c, z, x, y).parallel(y).vectorize(x, 4).unroll(c);
        blurx.compute_root().reorder(c, x, y, z).parallel(z).vectorize(x, 4).unroll(c);
        blury.compute_root().reorder(c, x, y, z).parallel(z).vectorize(x, 4).unroll(c);
        bilateral_grid.compute_root().parallel(y).vectorize(x, 4);
        sobelx.vectorize(x, 8).compute_at(sobel, y);
        sobely.vectorize(x, 8).compute_at(sobel, y);
        sobel.compute_root().parallel(y);
    }

    #ifdef DEMO_DISTRIBUTED
        if (rank == 0) printf("Scheduling for distributed.\n");
        // Computation distribution
        histogram.compute_at(blurz, y);
        histogram.update().reorder(c, r.x, r.y, x, y).unroll(c);
        blurz.compute_root().reorder(c, z, x, y).parallel(y).vectorize(x, 4).unroll(c).distribute(y);
        blurx.compute_root().reorder(c, x, y, z).parallel(z).vectorize(x, 4).unroll(c).distribute(y);
        blury.compute_root().reorder(c, x, y, z).parallel(z).vectorize(x, 4).unroll(c).distribute(y);
        bilateral_grid.compute_root().parallel(y).vectorize(x, 4).distribute(y);
        sobelx.vectorize(x, 8).compute_at(sobel, y);
        sobely.vectorize(x, 8).compute_at(sobel, y);
        sobel.compute_root().parallel(y).distribute(y);
        // Data distribution
        output.set_domain(x, y);
        output.placement().distribute(y);
        output.allocate();
        input.set_domain(x, y);
        input.placement().distribute(y);
        input.allocate(sobel, output);
#endif

    // Generate synthetic input image
    set_input_image_params(input.width(), input.height());
    //make_input_image(input);

    // Realize once to compile and warm up.
    sobel.realize(output, target);

    // Run pipeline 3 times and take the best time
    struct timespec start, stop;
    double sec = std::numeric_limits<double>::max();
    for (int i = 0; i < 3; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        sobel.realize(output, target);
        clock_gettime(CLOCK_MONOTONIC, &stop);
        double s = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;
        sec = std::min(sec, s);
    }

    // Empirically, this is a a decent approximation.
    const long long flops_per_pixel = 82;

#ifdef DEMO_DISTRIBUTED
    double maxElapsed = 0;
    MPI_Reduce(&sec, &maxElapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
//    const double megabytesPerSec = (input.global_width() * input.global_height() * sizeof(float)) / (maxElapsed * 1e6);
    const double gigapixelsPerSec = (input.global_width() * input.global_height()) / (maxElapsed * 1e9);
    const double gflopsPerSec = (input.global_width() * input.global_height() * flops_per_pixel) / (maxElapsed * 1e9);
    if (rank == 0) {
	printf("\n%-20s \n\n", "Image processed.");
        printf("%d MPI ranks.\n", numranks);
        printf("%-20s %.3f seconds\n", "Elapsed:", maxElapsed);
//        printf("%-20s %f MB/sec\n", "Agg. Throughput:", megabytesPerSec);
        printf("%-20s %f GP/sec\n", "Agg. Throughput:", gigapixelsPerSec);
        printf("%-20s %.3f gflops/sec\n", "Agg. Throughput:", gflopsPerSec);
    }
    MPI_Finalize();
#else
    const long long flops = input.width() * input.height() * flops_per_pixel;
    const double gflops = flops / 1e9;
//    const double megabytesPerSec = (w * h * sizeof(float)) / (sec * 1e6);
    const double gigapixelsPerSec = (w * h) / (sec * 1e9);
    printf("\n%-20s \n\n", "Image processed.");
    printf("%-20s %.3f seconds\n", "Elapsed:", sec);
//    printf("%-20s %.3f MB/sec\n", "Throughput:", megabytesPerSec);
    printf("%-20s %.3f GP/sec\n", "Throughput:", gigapixelsPerSec);
    printf("%-20s %.3f gflops/sec\n", "Throughput:", gflops / sec);
#endif

    return 0;
}
