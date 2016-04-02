#include "Halide.h"

using namespace Halide;

#include <stdio.h>
#include <time.h>

int w = 0, h = 0;
void set_input_image_params(int width, int height);

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: %s width height\n", argv[0]);
        return 1;
    }
    const int gw = atoi(argv[1]), gh = atoi(argv[2]);
    Image<uint8_t> input(gw, gh);
    Image<uint8_t> output(gw, gh);
    Var x("x"), y("y"), z("z"), c("c");

    // Filter parameters
    const int s_sigma = 16;
    const float r_sigma = 0.6f;

    // Convert to float
    Func clamped;
    clamped(x, y) = input(clamp(x, 0, input.width() - 1),
                          clamp(y, 0, input.height() - 1)) * 255.0f;

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
    clamped_bilateral_grid(x, y) = bilateral_grid(clamp(x, 0, input.width() - 1),
                                                  clamp(y, 0, input.height() - 1));

    Func sobelx, sobely;
    sobelx(x, y) = -1*clamped_bilateral_grid(x-1, y-1) + clamped_bilateral_grid(x+1, y-1) +
        -2*clamped_bilateral_grid(x-1, y) + 2*clamped_bilateral_grid(x+1, y) +
        -1*clamped_bilateral_grid(x-1, y+1) + clamped_bilateral_grid(x+1, y+1);
    sobely(x, y) = -1*clamped_bilateral_grid(x-1, y-1) + -2*clamped_bilateral_grid(x, y-1) + -1*clamped_bilateral_grid(x+1,y-1) +
        clamped_bilateral_grid(x-1, y+1) + 2*clamped_bilateral_grid(x, y+1) + clamped_bilateral_grid(x+1, y+1);

    Func sobel;
    sobel(x, y) = cast<uint8_t>((sqrt(sobelx(x, y) * sobelx(x, y) + sobely(x, y) * sobely(x, y))) * 255);

    Target target = get_jit_target_from_environment();
    printf("Scheduling for CPU.\n");
    histogram.compute_at(blurz, y);
    histogram.update().reorder(c, r.x, r.y, x, y).unroll(c);
    blurz.compute_root().reorder(c, z, x, y).parallel(y).vectorize(x, 4).unroll(c);
    blurx.compute_root().reorder(c, x, y, z).parallel(z).vectorize(x, 4).unroll(c);
    blury.compute_root().reorder(c, x, y, z).parallel(z).vectorize(x, 4).unroll(c);
    bilateral_grid.compute_root().parallel(y).vectorize(x, 4);
    sobelx.vectorize(x, 8).compute_at(sobel, y);
    sobely.vectorize(x, 8).compute_at(sobel, y);
    sobel.compute_root().parallel(y);

    // Generate synthetic input image
    set_input_image_params(input.width(), input.height());

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
    const long long flops = input.width() * input.height() * flops_per_pixel;
    const double gflops = flops / 1e9;
    printf("%-20s %.3f seconds\n", "Elapsed:", sec);
    printf("%-20s %.3f gflops/sec\n", "Throughput:", gflops / sec);

    return 0;
}













void set_input_image_params(int width, int height) {
    w = width;
    h = height;
}
