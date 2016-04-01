Var x("x"), y("y"), z("z"), c("c");
Func blurx, blury, blurz, interpolated, bilateral_grid, h,
    converted, sobelx, sobely, clamped, cbilateral_grid, sobel;

converted(x,y) = input(x,y)/255.0f;
clamped(x,y) = converted(clamp(x, 0, cols-1), clamp(y, 0, rows-1));
// Construct the bilateral grid
RDom r(0, s_sigma, 0, s_sigma);
Expr val = clamped(x * s_sigma + r.x - s_sigma/2, y * s_sigma + r.y - s_sigma/2);
Expr zi = cast<int>(clamp(val, 0.0f, 1.0f) * (1.0f/r_sigma) + 0.5f);
h(x, y, z, c) = 0.0f;
h(x, y, zi, c) += select(c == 0, clamp(val, 0.0f, 1.0f), 1.0f);
// Blur the grid using a five-point weighted average
blurz(x,y,z,c) = (h(x,y,z-2,c)   + h(x,y,z-1,c)*4    + h(x,y,z,c)*6 +
                  h(x,y,z+1,c)*4 + h(x, y, z+2, c));
blurx(x,y,z,c) = (blurz(x-2,y,z,c)   + blurz(x-1,y,z,c)*4 + blurz(x,y,z,c)*6 +
                  blurz(x+1,y,z,c)*4 + blurz(x+2,y,z,c));
blury(x,y,z,c) = (blurx(x,y-2,z,c)   + blurx(x,y-1,z,c)*4 + blurx(x,y,z,c)*6 +
                  blurx(x,y+1,z,c)*4 + blurx(x,y+2,z,c));
// Take trilinear samples to compute the output
Expr zv = clamp(converted(x, y), 0.0f, 1.0f) * (1.0f/r_sigma);
zi = cast<int>(zv);
Expr xf = cast<float>(x % s_sigma) / s_sigma;
Expr yf = cast<float>(y % s_sigma) / s_sigma;
Expr xi = x/s_sigma;
Expr yi = y/s_sigma;
interpolated(x, y, c) =
    lerp(lerp(lerp(blury(xi, yi, zi, c), blury(xi+1, yi, zi, c), xf),
              lerp(blury(xi, yi+1, zi, c), blury(xi+1, yi+1, zi, c), xf), yf),
         lerp(lerp(blury(xi, yi, zi+1, c), blury(xi+1, yi, zi+1, c), xf),
              lerp(blury(xi, yi+1, zi+1, c), blury(xi+1, yi+1, zi+1, c), xf), yf), zv - zi);
// Normalize
bilateral_grid(x, y) = ((interpolated(x, y, 0)/interpolated(x, y, 1)));
cbilateral_grid(x,y) = bilateral_grid(clamp(x, 0, cols-1), clamp(y, 0, rows-1));
// Sobel Edge Detector
sobelx(x, y) = -1*cbilateral_grid(x-1, y-1) + cbilateral_grid(x+1, y-1) +
    -2*cbilateral_grid(x-1, y) + 2*cbilateral_grid(x+1, y) +
    -1*cbilateral_grid(x-1, y+1) + cbilateral_grid(x+1, y+1);
sobely(x, y) = -1*cbilateral_grid(x-1, y-1) + -2*cbilateral_grid(x, y-1) + -1*cbilateral_grid(x+1,y-1) +
    cbilateral_grid(x-1, y+1) + 2*cbilateral_grid(x, y+1) + cbilateral_grid(x+1, y+1);
sobel(x, y) = cast<uint8_t>((sqrt(sobelx(x, y) * sobelx(x, y) + sobely(x, y) * sobely(x, y)))*255);


// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------- Schedules ------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

Target target = get_target_from_environment();

if (schedule == CPU) {
    blurz.compute_root().reorder(c, z, x, y).parallel(y).vectorize(x, 8).unroll(c);
    h.compute_at(blurz, y);
    h.update().reorder(c, r.x, r.y, x, y).unroll(c);
    blurx.compute_root().reorder(c, x, y, z).parallel(z).vectorize(x, 8).unroll(c);
    blury.compute_root().reorder(c, x, y, z).parallel(z).vectorize(x, 8).unroll(c);
    bilateral_grid.compute_root().parallel(y).vectorize(x, 8);
    sobelx.vectorize(x, 8).compute_at(sobel, y);
    sobely.vectorize(x, 8).compute_at(sobel, y);
    sobel.parallel(y);

} else if (schedule == GPU) {
    target.set_feature(Target::OpenCL);
    blurz.compute_root().reorder(c, z, x, y).gpu_tile(x, y, 8, 8);
    h.reorder(c, z, x, y).compute_at(blurz, Var::gpu_blocks()).gpu_threads(x, y);
    h.update().reorder(c, r.x, r.y, x, y).gpu_threads(x, y).unroll(c);
    blurx.compute_root().gpu_tile(x, y, z, 8, 8, 1);
    blury.compute_root().gpu_tile(x, y, z, 8, 8, 1);
    bilateral_grid.compute_root().gpu_tile(x, y, s_sigma, s_sigma);
    sobel.compute_root().gpu_tile(x, y, 8, 8);
    sobelx.compute_at(sobel, Var::gpu_blocks()).gpu_threads(x, y);
    sobely.compute_at(sobel, Var::gpu_blocks()).gpu_threads(x, y);

} else if (schedule == DISTRIBUTED) {
    h.compute_at(blurz, y);
    h.update().reorder(c, r.x, r.y, x, y).unroll(c);
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
}
