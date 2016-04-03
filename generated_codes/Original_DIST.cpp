#include "Halide.h"
using namespace Halide;

int main(int argc, char **argv) {

    ImageParam input(type_of<uint8_t>(), 2);
    Param<int32_t> rows, cols;
    Var x, y, z, c;
    Func blurx, blury, blurz, interpolated, bilateral_grid, converted, sobelx, sobely, clamped, cb_grid, sobel, histogram;

    
    // Bilateral grid
    clamped(x,y) = input(clamp(x, 0, cols-1), clamp(y, 0, rows-1))/255.0f;
    RDom r(0, 16, 0, 16);
    val = clamp(clamped(x * s_sigma + r.x - s_sigma/2, y * s_sigma + r.y - s_sigma/2), 0.0f, 1.0f);
    Expr zi = cast<int>(val * (1.0f/0.1f) + 0.5f);
    histogram(x, y, z, c) = 0.0f;
    histogram(x, y, zi, c) += select(c == 0, val, 1.0f);
    blurz(x, y, z, c) = (histogram(x, y, z-2, c)   + histogram(x, y, z-1, c)*4 + histogram(x, y, z  , c)*6 + histogram(x, y, z+1, c)*4 + histogram(x, y, z+2, c));
    blurx(x, y, z, c) = (blurz(x-2, y, z, c) + blurz(x-1, y, z, c)*4 + blurz(x  , y, z, c)*6 + blurz(x+1, y, z, c)*4 + blurz(x+2, y, z, c));
    blury(x, y, z, c) = (blurx(x, y-2, z, c) + blurx(x, y-1, z, c)*4 + blurx(x, y  , z, c)*6 + blurx(x, y+1, z, c)*4 + blurx(x, y+2, z, c));
    val = clamp(input(x, y)/255.0f, 0.0f, 1.0f);
    Expr zf = val * (1.0f/0.1f) - cast<int>(val * (1.0f/0.1f));
    Expr xf = cast<float>(x % s_sigma) / s_sigma;
    Expr yf = cast<float>(y % s_sigma) / s_sigma;
    Expr xi = x/s_sigma;
    Expr yi = y/s_sigma;
    interpolated(x, y, c) = lerp(lerp(lerp(blury(xi, yi, zi, c), blury(xi+1, yi, zi, c), xf), lerp(blury(xi, yi+1, zi, c), blury(xi+1, yi+1, zi, c), xf), yf),
                                lerp(lerp(blury(xi, yi, zi+1, c), blury(xi+1, yi, zi+1, c), xf), lerp(blury(xi, yi+1, zi+1, c), blury(xi+1, yi+1, zi+1, c), xf), yf), zf);
    bilateral_grid(x, y) = ((interpolated(x, y, 0)/interpolated(x, y, 1)));
    cb_grid(x,y) = bilateral_grid(clamp(x, 0, cols-1), clamp(y, 0, rows-1));

    // Sobel edge detector
    sobelx(x, y) = -1*cb_grid(x-1, y-1) + cb_grid(x+1, y-1) -2*cb_grid(x-1, y) + 2*cb_grid(x+1, y) -1*cb_grid(x-1, y+1) + cb_grid(x+1, y+1);
    sobely(x, y) = -1*cb_grid(x-1, y-1) + -2*cb_grid(x, y-1) + -1*cb_grid(x+1,y-1) + cb_grid(x-1, y+1) + 2*cb_grid(x, y+1) + cb_grid(x+1, y+1);
    sobel(x, y) = cast<uint8_t>((sqrt(sobelx(x, y) * sobelx(x, y) + sobely(x, y) * sobely(x, y)))*255);

    
    

    // ----------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------
    // ----------------------------------------- The distributed schedule ---------------------------------------------
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
    // -----------------------------------------------------------------------------------------------------------------


    std::vector<Argument> args = {input, rows, cols};
    sobel.compile_to_file("halide_pipeline_aot", args);

    return 0;
}
