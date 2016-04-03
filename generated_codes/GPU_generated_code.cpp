let bilateral_grid.s0.y.max = max(min((f4.min.1 + f4.extent.1), 799), 0)
let bilateral_grid.s0.y.min = max(min((f4.min.1 + -1), 799), 0)
let bilateral_grid.s0.x.max = max(min((f4.min.0 + f4.extent.0), 1279), 0)
let bilateral_grid.s0.x.min = max(min((f4.min.0 + -1), 1279), 0)
let f4.extent.0.required.s = (min((((((f4.extent.0 + -1)/8)*8) + f4.min.0) + 7), ((f4.min.0 + f4.extent.0) + -1)) - min(f4.min.0, ((f4.min.0 + f4.extent.0) + -8)))
let f4.min.0.required = min(f4.min.0, ((f4.min.0 + f4.extent.0) + -8))
let f4.extent.1.required.s = (min((((((f4.extent.1 + -1)/8)*8) + f4.min.1) + 7), ((f4.min.1 + f4.extent.1) + -1)) - min(f4.min.1, ((f4.min.1 + f4.extent.1) + -8)))
let f4.min.1.required = min(f4.min.1, ((f4.min.1 + f4.extent.1) + -8))
let i0.extent.0.required.s = (max(max(min((((bilateral_grid.s0.x.max/16)*16) + 55), 1279), min((((((bilateral_grid.s0.x.max - bilateral_grid.s0.x.min)/16)*16) + bilateral_grid.s0.x.min) + 15), bilateral_grid.s0.x.max)), 0) - min(max(min((((bilateral_grid.s0.x.min/16)*16) + -40), 1279), 0), min(bilateral_grid.s0.x.min, (bilateral_grid.s0.x.max + -15))))
let i0.min.0.required = min(max(min((((bilateral_grid.s0.x.min/16)*16) + -40), 1279), 0), min(bilateral_grid.s0.x.min, (bilateral_grid.s0.x.max + -15)))
let i0.extent.1.required.s = (max(max(min((((bilateral_grid.s0.y.max/16)*16) + 55), 799), min((((((bilateral_grid.s0.y.max - bilateral_grid.s0.y.min)/16)*16) + bilateral_grid.s0.y.min) + 15), bilateral_grid.s0.y.max)), 0) - min(max(min((((bilateral_grid.s0.y.min/16)*16) + -40), 799), 0), min(bilateral_grid.s0.y.min, (bilateral_grid.s0.y.max + -15))))
let i0.min.1.required = min(max(min((((bilateral_grid.s0.y.min/16)*16) + -40), 799), 0), min(bilateral_grid.s0.y.min, (bilateral_grid.s0.y.max + -15)))
assert((!i0.host_and_dev_are_null || ((0 <= i0.min.0.required) && ((i0.min.0.required + i0.extent.0.required.s) <= 1279))), halide_error_constraints_make_required_region_smaller("Input buffer i0", 0, 0, 1279, i0.min.0.required, (i0.min.0.required + i0.extent.0.required.s)))
assert((!i0.host_and_dev_are_null || ((0 <= i0.min.1.required) && ((i0.min.1.required + i0.extent.1.required.s) <= 799))), halide_error_constraints_make_required_region_smaller("Input buffer i0", 1, 0, 799, i0.min.1.required, (i0.min.1.required + i0.extent.1.required.s)))
if (f4.host_and_dev_are_null) {
  rewrite_buffer(f4.buffer, 1, f4.min.0.required, (f4.extent.0.required.s + 1), 1, f4.min.1.required, (f4.extent.1.required.s + 1), (f4.extent.0.required.s + 1))
}
if (i0.host_and_dev_are_null) {
  rewrite_buffer(i0.buffer, 1, 0, 1280, 1, 0, 800, 1280)
}
if (!(f4.host_and_dev_are_null || i0.host_and_dev_are_null)) {
  assert((f4.elem_size == 1), halide_error_bad_elem_size("Output buffer f4", "uint8", f4.elem_size, 1))
  assert((i0.elem_size == 1), halide_error_bad_elem_size("Input buffer i0", "uint8", i0.elem_size, 1))
  assert(((f4.min.0 <= f4.min.0.required) && ((((f4.min.0.required + f4.extent.0.required.s) - f4.extent.0) + 1) <= f4.min.0)), halide_error_access_out_of_bounds("Output buffer f4", 0, f4.min.0.required, (f4.min.0.required + f4.extent.0.required.s), f4.min.0, ((f4.min.0 + f4.extent.0) + -1)))
  assert(((f4.min.1 <= f4.min.1.required) && ((((f4.min.1.required + f4.extent.1.required.s) - f4.extent.1) + 1) <= f4.min.1)), halide_error_access_out_of_bounds("Output buffer f4", 1, f4.min.1.required, (f4.min.1.required + f4.extent.1.required.s), f4.min.1, ((f4.min.1 + f4.extent.1) + -1)))
  assert(((i0.min.0 <= i0.min.0.required) && ((((i0.min.0.required + i0.extent.0.required.s) - i0.extent.0) + 1) <= i0.min.0)), halide_error_access_out_of_bounds("Input buffer i0", 0, i0.min.0.required, (i0.min.0.required + i0.extent.0.required.s), i0.min.0, ((i0.min.0 + i0.extent.0) + -1)))
  assert(((i0.min.1 <= i0.min.1.required) && ((((i0.min.1.required + i0.extent.1.required.s) - i0.extent.1) + 1) <= i0.min.1)), halide_error_access_out_of_bounds("Input buffer i0", 1, i0.min.1.required, (i0.min.1.required + i0.extent.1.required.s), i0.min.1, ((i0.min.1 + i0.extent.1) + -1)))
  assert((f4.stride.0 == 1), halide_error_constraint_violated("f4.stride.0", f4.stride.0, "1", 1))
  assert((i0.stride.0 == 1), halide_error_constraint_violated("i0.stride.0", i0.stride.0, "1", 1))
  assert((i0.min.0 == 0), halide_error_constraint_violated("i0.min.0", i0.min.0, "0", 0))
  assert((i0.extent.0 == 1280), halide_error_constraint_violated("i0.extent.0", i0.extent.0, "1280", 1280))
  assert((i0.stride.1 == 1280), halide_error_constraint_violated("i0.stride.1", i0.stride.1, "1280", 1280))
  assert((i0.min.1 == 0), halide_error_constraint_violated("i0.min.1", i0.min.1, "0", 0))
  assert((i0.extent.1 == 800), halide_error_constraint_violated("i0.extent.1", i0.extent.1, "800", 800))
  let f4.total_extent.1 = (int64(f4.extent.1)*int64(f4.extent.0))
  assert((int64(f4.extent.0) <= (int64)2147483647), halide_error_buffer_allocation_too_large("f4", int64(f4.extent.0), (int64)2147483647))
  assert(((int64(f4.extent.1)*int64(f4.stride.1)) <= (int64)2147483647), halide_error_buffer_allocation_too_large("f4", (int64(f4.extent.1)*int64(f4.stride.1)), (int64)2147483647))
  assert((f4.total_extent.1 <= (int64)2147483647), halide_error_buffer_extents_too_large("f4", f4.total_extent.1, (int64)2147483647))
  let blurz.y.min_realized = min(((bilateral_grid.s0.y.min/16) + -2), ((bilateral_grid.s0.y.max/16) + -4))
  let blurz.y.extent_realized.s.s = min((((((((bilateral_grid.s0.y.max/16) - (bilateral_grid.s0.y.min/16)) + 5)/8)*8) + (bilateral_grid.s0.y.min/16)) + 5), ((bilateral_grid.s0.y.max/16) + 3))
  let blurz.x.min_realized = min((min((bilateral_grid.s0.x.min/16), ((bilateral_grid.s0.x.max/16) + -6)) + -2), min(((bilateral_grid.s0.x.min/16) + -2), ((bilateral_grid.s0.x.max/16) + -4)))
  let blurz.x.extent_realized.s.s = (let t44 = ((bilateral_grid.s0.x.max/16) - (bilateral_grid.s0.x.min/16)) in max((min((((((t44 + 1)/8)*8) + (bilateral_grid.s0.x.min/16)) + 7), ((bilateral_grid.s0.x.max/16) + 1)) + 2), min((((((t44 + 5)/8)*8) + (bilateral_grid.s0.x.min/16)) + 5), ((bilateral_grid.s0.x.max/16) + 3))))
  let blurz.stride.2 = (((blurz.x.extent_realized.s.s - blurz.x.min_realized) + 1)*((blurz.y.extent_realized.s.s - blurz.y.min_realized) + 1))
  allocate blurz[float32 * ((blurz.x.extent_realized.s.s - blurz.x.min_realized) + 1) * ((blurz.y.extent_realized.s.s - blurz.y.min_realized) + 1) * 3 * 2]
  let blurz.buffer = create_buffer_t(address_of(blurz[0]), 0.000000f, blurz.x.min_realized, ((blurz.x.extent_realized.s.s - blurz.x.min_realized) + 1), 1, blurz.y.min_realized, ((blurz.y.extent_realized.s.s - blurz.y.min_realized) + 1), ((blurz.x.extent_realized.s.s - blurz.x.min_realized) + 1), 0, 3, blurz.stride.2, 0, 2, (blurz.stride.2*3))
  produce blurz {
    let copy_to_device_result$2 = halide_copy_to_device(i0.buffer, halide_opencl_device_interface())
    assert((copy_to_device_result$2 == 0), copy_to_device_result$2)
    let device_malloc_result = halide_device_malloc(blurz.buffer, halide_opencl_device_interface())
    assert((device_malloc_result == 0), device_malloc_result)
    let copy_to_device_result = halide_copy_to_device(blurz.buffer, halide_opencl_device_interface())
    assert((copy_to_device_result == 0), copy_to_device_result)
    let blurz.s0.x.__block_id_x.loop_extent.s = ((bilateral_grid.s0.x.max/16) - (bilateral_grid.s0.x.min/16))
    let blurz.s0.y.__block_id_y.loop_extent.s = ((bilateral_grid.s0.y.max/16) - (bilateral_grid.s0.y.min/16))
    parallel<OpenCL> (blurz.s0.y.__block_id_y, 0, ((blurz.s0.y.__block_id_y.loop_extent.s + 13)/8)) {
      parallel<OpenCL> (blurz.s0.x.__block_id_x, 0, ((blurz.s0.x.__block_id_x.loop_extent.s + 13)/8)) {
        allocate __shared[uint8 * 3584]
        parallel<OpenCL> (.__thread_id_y, 0, 8) {
          parallel<OpenCL> (.__thread_id_x, 0, 8) {
            let blurz.s0.y.__thread_id_y.base = min((((blurz.s0.y.__block_id_y*8) + (bilateral_grid.s0.y.min/16)) + -2), ((bilateral_grid.s0.y.max/16) + -4))
            let blurz.s0.x.__thread_id_x.base = min((((blurz.s0.x.__block_id_x*8) + (bilateral_grid.s0.x.min/16)) + -2), ((bilateral_grid.s0.x.max/16) + -4))
            produce histogram {
              for<OpenCL> (histogram.s0.z, -2, 7) {
                for<OpenCL> (histogram.s0.c, 0, 2) {
                  __shared[((((.__thread_id_x + (.__thread_id_y*8)) + (histogram.s0.z*64)) + (histogram.s0.c*448)) + 128)] = 0.000000f
                }
              }
              halide_gpu_thread_barrier()
            } update histogram {
              for<OpenCL> (histogram.s1.r4.y$r, 0, 16) {
                for<OpenCL> (histogram.s1.r4.x$r, 0, 16) {
                  __shared[(((.__thread_id_x + (.__thread_id_y*8)) + (int32(((max(min((float32(i0[(max(min(((((.__thread_id_x + blurz.s0.x.__thread_id_x.base)*16) + histogram.s1.r4.x$r) + -8), 1279), 0) + (max(min(((((.__thread_id_y + blurz.s0.y.__thread_id_y.base)*16) + histogram.s1.r4.y$r) + -8), 799), 0)*1280))])*255.000000f), 1.000000f), 0.000000f)*1.666667f) + 0.500000f))*64)) + 128)] = (let t46 = max(min((float32(i0[(max(min(((((.__thread_id_x + blurz.s0.x.__thread_id_x.base)*16) + histogram.s1.r4.x$r) + -8), 1279), 0) + (max(min(((((.__thread_id_y + blurz.s0.y.__thread_id_y.base)*16) + histogram.s1.r4.y$r) + -8), 799), 0)*1280))])*255.000000f), 1.000000f), 0.000000f) in (__shared[(((.__thread_id_x + (.__thread_id_y*8)) + (int32(((t46*1.666667f) + 0.500000f))*64)) + 128)] + t46))
                  __shared[(((.__thread_id_x + (.__thread_id_y*8)) + (int32(((max(min((float32(i0[(max(min(((((.__thread_id_x + blurz.s0.x.__thread_id_x.base)*16) + histogram.s1.r4.x$r) + -8), 1279), 0) + (max(min(((((.__thread_id_y + blurz.s0.y.__thread_id_y.base)*16) + histogram.s1.r4.y$r) + -8), 799), 0)*1280))])*255.000000f), 1.000000f), 0.000000f)*1.666667f) + 0.500000f))*64)) + 576)] = (__shared[(((.__thread_id_x + (.__thread_id_y*8)) + (int32(((max(min((float32(i0[(max(min(((((.__thread_id_x + blurz.s0.x.__thread_id_x.base)*16) + histogram.s1.r4.x$r) + -8), 1279), 0) + (max(min(((((.__thread_id_y + blurz.s0.y.__thread_id_y.base)*16) + histogram.s1.r4.y$r) + -8), 799), 0)*1280))])*255.000000f), 1.000000f), 0.000000f)*1.666667f) + 0.500000f))*64)) + 576)] + 1.000000f)
                }
              }
              halide_gpu_thread_barrier()
            }
            for<OpenCL> (blurz.s0.z, 0, 3) {
              for<OpenCL> (blurz.s0.c, 0, 2) {
                blurz[(((((blurz.s0.x.__thread_id_x.base + .__thread_id_x) - blurz.x.min_realized) + (((blurz.s0.y.__thread_id_y.base + .__thread_id_y) - blurz.y.min_realized)*((blurz.x.extent_realized.s.s - blurz.x.min_realized) + 1))) + (blurz.s0.z*blurz.stride.2)) + ((blurz.s0.c*blurz.stride.2)*3))] = (let t47 = (((.__thread_id_x + (.__thread_id_y*8)) + (blurz.s0.z*64)) + (blurz.s0.c*448)) in ((((__shared[t47] + (__shared[(t47 + 64)]*4.000000f)) + (__shared[(t47 + 128)]*6.000000f)) + (__shared[(t47 + 192)]*4.000000f)) + __shared[(t47 + 256)]))
              }
            }
          }
        }
        free __shared
      }
    }
    set_dev_dirty(blurz.buffer, (uint8)1)
  }
  let blurx.y.min_realized = min((min((bilateral_grid.s0.y.min/16), ((bilateral_grid.s0.y.max/16) + -6)) + -2), blurz.y.min_realized)
  let blurx.y.extent_realized.s = (let t48 = ((bilateral_grid.s0.y.max/16) - (bilateral_grid.s0.y.min/16)) in (max((min((((((t48 + 1)/8)*8) + (bilateral_grid.s0.y.min/16)) + 7), ((bilateral_grid.s0.y.max/16) + 1)) + 2), min((((((t48 + 5)/8)*8) + (bilateral_grid.s0.y.min/16)) + 5), ((bilateral_grid.s0.y.max/16) + 3))) - min((min((bilateral_grid.s0.y.min/16), ((bilateral_grid.s0.y.max/16) + -6)) + -2), blurz.y.min_realized)))
  let blurx.x.min_realized = min((bilateral_grid.s0.x.min/16), ((bilateral_grid.s0.x.max/16) + -6))
  let blurx.x.extent_realized.s.s = min((((((((bilateral_grid.s0.x.max/16) - (bilateral_grid.s0.x.min/16)) + 1)/8)*8) + (bilateral_grid.s0.x.min/16)) + 7), ((bilateral_grid.s0.x.max/16) + 1))
  let blurx.stride.2 = (((blurx.x.extent_realized.s.s - blurx.x.min_realized) + 1)*(blurx.y.extent_realized.s + 1))
  allocate blurx[float32 * ((blurx.x.extent_realized.s.s - blurx.x.min_realized) + 1) * (blurx.y.extent_realized.s + 1) * 3 * 2]
  let blurx.buffer = create_buffer_t(address_of(blurx[0]), 0.000000f, blurx.x.min_realized, ((blurx.x.extent_realized.s.s - blurx.x.min_realized) + 1), 1, blurx.y.min_realized, (blurx.y.extent_realized.s + 1), ((blurx.x.extent_realized.s.s - blurx.x.min_realized) + 1), 0, 3, blurx.stride.2, 0, 2, (blurx.stride.2*3))
  produce blurx {
    let copy_to_device_result$4 = halide_copy_to_device(blurz.buffer, halide_opencl_device_interface())
    assert((copy_to_device_result$4 == 0), copy_to_device_result$4)
    let device_malloc_result$2 = halide_device_malloc(blurx.buffer, halide_opencl_device_interface())
    assert((device_malloc_result$2 == 0), device_malloc_result$2)
    let copy_to_device_result$3 = halide_copy_to_device(blurx.buffer, halide_opencl_device_interface())
    assert((copy_to_device_result$3 == 0), copy_to_device_result$3)
    let blurx.s0.x.__block_id_x.loop_extent.s = ((bilateral_grid.s0.x.max/16) - (bilateral_grid.s0.x.min/16))
    let blurx.s0.y.__block_id_y.loop_extent.s = ((bilateral_grid.s0.y.max/16) - (bilateral_grid.s0.y.min/16))
    for (blurx.s0.c, 0, 2) {
      parallel<OpenCL> (blurx.s0.z.__block_id_z, 0, 3) {
        parallel<OpenCL> (blurx.s0.y.__block_id_y, 0, ((blurx.s0.y.__block_id_y.loop_extent.s + 13)/8)) {
          parallel<OpenCL> (blurx.s0.x.__block_id_x, 0, ((blurx.s0.x.__block_id_x.loop_extent.s + 9)/8)) {
            parallel<OpenCL> (.__thread_id_z, 0, 1) {
              parallel<OpenCL> (.__thread_id_y, 0, 8) {
                parallel<OpenCL> (.__thread_id_x, 0, 8) {
                  let blurx.s0.y.__thread_id_y.base = min((((blurx.s0.y.__block_id_y*8) + (bilateral_grid.s0.y.min/16)) + -2), ((bilateral_grid.s0.y.max/16) + -4))
                  let blurx.s0.x.__thread_id_x.base = min(((blurx.s0.x.__block_id_x*8) + (bilateral_grid.s0.x.min/16)), ((bilateral_grid.s0.x.max/16) + -6))
                  blurx[(((((blurx.s0.x.__thread_id_x.base + .__thread_id_x) - blurx.x.min_realized) + (((blurx.s0.y.__thread_id_y.base + .__thread_id_y) - blurx.y.min_realized)*((blurx.x.extent_realized.s.s - blurx.x.min_realized) + 1))) + ((blurx.s0.z.__block_id_z + .__thread_id_z)*blurx.stride.2)) + ((blurx.s0.c*blurx.stride.2)*3))] = (let t50 = (((((blurx.s0.x.__thread_id_x.base + .__thread_id_x) - blurz.x.min_realized) + (((blurx.s0.y.__thread_id_y.base + .__thread_id_y) - blurz.y.min_realized)*((blurz.x.extent_realized.s.s - blurz.x.min_realized) + 1))) + ((blurx.s0.z.__block_id_z + .__thread_id_z)*blurz.stride.2)) + ((blurx.s0.c*blurz.stride.2)*3)) in ((((blurz[(t50 + -2)] + (blurz[(t50 + -1)]*4.000000f)) + (blurz[t50]*6.000000f)) + (blurz[(t50 + 1)]*4.000000f)) + blurz[(t50 + 2)]))
                }
              }
            }
          }
        }
      }
    }
    free blurz
    set_dev_dirty(blurx.buffer, (uint8)1)
  }
  let blury.y.min_realized = min((min(bilateral_grid.s0.y.min, (bilateral_grid.s0.y.max + -15))/16), min((bilateral_grid.s0.y.min/16), ((bilateral_grid.s0.y.max/16) + -6)))
  let blury.y.extent_realized.s.s = max(((min((((((bilateral_grid.s0.y.max - bilateral_grid.s0.y.min)/16)*16) + bilateral_grid.s0.y.min) + 15), bilateral_grid.s0.y.max)/16) + 1), min((((((((bilateral_grid.s0.y.max/16) - (bilateral_grid.s0.y.min/16)) + 1)/8)*8) + (bilateral_grid.s0.y.min/16)) + 7), ((bilateral_grid.s0.y.max/16) + 1)))
  let blury.x.min_realized = min((min(bilateral_grid.s0.x.min, (bilateral_grid.s0.x.max + -15))/16), blurx.x.min_realized)
  let blury.x.extent_realized.s = (max(((min((((((bilateral_grid.s0.x.max - bilateral_grid.s0.x.min)/16)*16) + bilateral_grid.s0.x.min) + 15), bilateral_grid.s0.x.max)/16) + 1), min((((((((bilateral_grid.s0.x.max/16) - (bilateral_grid.s0.x.min/16)) + 1)/8)*8) + (bilateral_grid.s0.x.min/16)) + 7), ((bilateral_grid.s0.x.max/16) + 1))) - min((min(bilateral_grid.s0.x.min, (bilateral_grid.s0.x.max + -15))/16), blurx.x.min_realized))
  let blury.stride.2 = ((blury.x.extent_realized.s + 1)*((blury.y.extent_realized.s.s - blury.y.min_realized) + 1))
  allocate blury[float32 * (blury.x.extent_realized.s + 1) * ((blury.y.extent_realized.s.s - blury.y.min_realized) + 1) * 3 * 2]
  let blury.buffer = create_buffer_t(address_of(blury[0]), 0.000000f, blury.x.min_realized, (blury.x.extent_realized.s + 1), 1, blury.y.min_realized, ((blury.y.extent_realized.s.s - blury.y.min_realized) + 1), (blury.x.extent_realized.s + 1), 0, 3, blury.stride.2, 0, 2, (blury.stride.2*3))
  produce blury {
    let device_malloc_result$3 = halide_device_malloc(blury.buffer, halide_opencl_device_interface())
    assert((device_malloc_result$3 == 0), device_malloc_result$3)
    let copy_to_device_result$6 = halide_copy_to_device(blury.buffer, halide_opencl_device_interface())
    assert((copy_to_device_result$6 == 0), copy_to_device_result$6)
    let copy_to_device_result$5 = halide_copy_to_device(blurx.buffer, halide_opencl_device_interface())
    assert((copy_to_device_result$5 == 0), copy_to_device_result$5)
    let blury.s0.x.__block_id_x.loop_extent.s = ((bilateral_grid.s0.x.max/16) - (bilateral_grid.s0.x.min/16))
    let blury.s0.y.__block_id_y.loop_extent.s = ((bilateral_grid.s0.y.max/16) - (bilateral_grid.s0.y.min/16))
    for (blury.s0.c, 0, 2) {
      parallel<OpenCL> (blury.s0.z.__block_id_z, 0, 3) {
        parallel<OpenCL> (blury.s0.y.__block_id_y, 0, ((blury.s0.y.__block_id_y.loop_extent.s + 9)/8)) {
          parallel<OpenCL> (blury.s0.x.__block_id_x, 0, ((blury.s0.x.__block_id_x.loop_extent.s + 9)/8)) {
            parallel<OpenCL> (.__thread_id_z, 0, 1) {
              parallel<OpenCL> (.__thread_id_y, 0, 8) {
                parallel<OpenCL> (.__thread_id_x, 0, 8) {
                  let blury.s0.y.__thread_id_y.base = min(((blury.s0.y.__block_id_y*8) + (bilateral_grid.s0.y.min/16)), ((bilateral_grid.s0.y.max/16) + -6))
                  let blury.s0.x.__thread_id_x.base = min(((blury.s0.x.__block_id_x*8) + (bilateral_grid.s0.x.min/16)), ((bilateral_grid.s0.x.max/16) + -6))
                  blury[(((((blury.s0.x.__thread_id_x.base + .__thread_id_x) - blury.x.min_realized) + (((blury.s0.y.__thread_id_y.base + .__thread_id_y) - blury.y.min_realized)*(blury.x.extent_realized.s + 1))) + ((blury.s0.z.__block_id_z + .__thread_id_z)*blury.stride.2)) + ((blury.s0.c*blury.stride.2)*3))] = ((((blurx[(((((blury.s0.x.__thread_id_x.base + .__thread_id_x) - blurx.x.min_realized) + ((((blury.s0.y.__thread_id_y.base + .__thread_id_y) - blurx.y.min_realized) + -2)*((blurx.x.extent_realized.s.s - blurx.x.min_realized) + 1))) + ((blury.s0.z.__block_id_z + .__thread_id_z)*blurx.stride.2)) + ((blury.s0.c*blurx.stride.2)*3))] + (blurx[(((((blury.s0.x.__thread_id_x.base + .__thread_id_x) - blurx.x.min_realized) + ((((blury.s0.y.__thread_id_y.base + .__thread_id_y) - blurx.y.min_realized) + -1)*((blurx.x.extent_realized.s.s - blurx.x.min_realized) + 1))) + ((blury.s0.z.__block_id_z + .__thread_id_z)*blurx.stride.2)) + ((blury.s0.c*blurx.stride.2)*3))]*4.000000f)) + (blurx[(((((blury.s0.x.__thread_id_x.base + .__thread_id_x) - blurx.x.min_realized) + (((blury.s0.y.__thread_id_y.base + .__thread_id_y) - blurx.y.min_realized)*((blurx.x.extent_realized.s.s - blurx.x.min_realized) + 1))) + ((blury.s0.z.__block_id_z + .__thread_id_z)*blurx.stride.2)) + ((blury.s0.c*blurx.stride.2)*3))]*6.000000f)) + (blurx[(((((blury.s0.x.__thread_id_x.base + .__thread_id_x) - blurx.x.min_realized) + ((((blury.s0.y.__thread_id_y.base + .__thread_id_y) - blurx.y.min_realized) + 1)*((blurx.x.extent_realized.s.s - blurx.x.min_realized) + 1))) + ((blury.s0.z.__block_id_z + .__thread_id_z)*blurx.stride.2)) + ((blury.s0.c*blurx.stride.2)*3))]*4.000000f)) + blurx[(((((blury.s0.x.__thread_id_x.base + .__thread_id_x) - blurx.x.min_realized) + ((((blury.s0.y.__thread_id_y.base + .__thread_id_y) - blurx.y.min_realized) + 2)*((blurx.x.extent_realized.s.s - blurx.x.min_realized) + 1))) + ((blury.s0.z.__block_id_z + .__thread_id_z)*blurx.stride.2)) + ((blury.s0.c*blurx.stride.2)*3))])
                }
              }
            }
          }
        }
      }
    }
    free blurx
    set_dev_dirty(blury.buffer, (uint8)1)
  }
  let bilateral_grid.y.min_realized = min(max(min((f4.min.1.required + -1), 799), 0), min(bilateral_grid.s0.y.min, (bilateral_grid.s0.y.max + -15)))
  let bilateral_grid.y.extent_realized.s = (max(max(min(min((((((f4.extent.1 + -1)/8)*8) + f4.min.1) + 8), (f4.min.1 + f4.extent.1)), 799), min((((((bilateral_grid.s0.y.max - bilateral_grid.s0.y.min)/16)*16) + bilateral_grid.s0.y.min) + 15), bilateral_grid.s0.y.max)), 0) - min(max(min((f4.min.1.required + -1), 799), 0), min(bilateral_grid.s0.y.min, (bilateral_grid.s0.y.max + -15))))
  let bilateral_grid.x.min_realized = min(max(min((f4.min.0.required + -1), 1279), 0), min(bilateral_grid.s0.x.min, (bilateral_grid.s0.x.max + -15)))
  let bilateral_grid.x.extent_realized.s = (max(max(min(min((((((f4.extent.0 + -1)/8)*8) + f4.min.0) + 8), (f4.min.0 + f4.extent.0)), 1279), min((((((bilateral_grid.s0.x.max - bilateral_grid.s0.x.min)/16)*16) + bilateral_grid.s0.x.min) + 15), bilateral_grid.s0.x.max)), 0) - min(max(min((f4.min.0.required + -1), 1279), 0), min(bilateral_grid.s0.x.min, (bilateral_grid.s0.x.max + -15))))
  allocate bilateral_grid[float32 * (bilateral_grid.x.extent_realized.s + 1) * (bilateral_grid.y.extent_realized.s + 1)]
  let bilateral_grid.buffer = create_buffer_t(address_of(bilateral_grid[0]), 0.000000f, bilateral_grid.x.min_realized, (bilateral_grid.x.extent_realized.s + 1), 1, bilateral_grid.y.min_realized, (bilateral_grid.y.extent_realized.s + 1), (bilateral_grid.x.extent_realized.s + 1))
  produce bilateral_grid {
    let copy_to_device_result$9 = halide_copy_to_device(i0.buffer, halide_opencl_device_interface())
    assert((copy_to_device_result$9 == 0), copy_to_device_result$9)
    let copy_to_device_result$8 = halide_copy_to_device(blury.buffer, halide_opencl_device_interface())
    assert((copy_to_device_result$8 == 0), copy_to_device_result$8)
    let device_malloc_result$4 = halide_device_malloc(bilateral_grid.buffer, halide_opencl_device_interface())
    assert((device_malloc_result$4 == 0), device_malloc_result$4)
    let copy_to_device_result$7 = halide_copy_to_device(bilateral_grid.buffer, halide_opencl_device_interface())
    assert((copy_to_device_result$7 == 0), copy_to_device_result$7)
    parallel<OpenCL> (bilateral_grid.s0.y.__block_id_y, 0, (((bilateral_grid.s0.y.max - bilateral_grid.s0.y.min) + 16)/16)) {
      parallel<OpenCL> (bilateral_grid.s0.x.__block_id_x, 0, (((bilateral_grid.s0.x.max - bilateral_grid.s0.x.min) + 16)/16)) {
        parallel<OpenCL> (.__thread_id_y, 0, 16) {
          parallel<OpenCL> (.__thread_id_x, 0, 16) {
            let bilateral_grid.s0.y.__thread_id_y.base = min(((bilateral_grid.s0.y.__block_id_y*16) + bilateral_grid.s0.y.min), (bilateral_grid.s0.y.max + -15))
            let bilateral_grid.s0.x.__thread_id_x.base = min(((bilateral_grid.s0.x.__block_id_x*16) + bilateral_grid.s0.x.min), (bilateral_grid.s0.x.max + -15))
            bilateral_grid[(((bilateral_grid.s0.x.__thread_id_x.base + .__thread_id_x) - bilateral_grid.x.min_realized) + (((bilateral_grid.s0.y.__thread_id_y.base + .__thread_id_y) - bilateral_grid.y.min_realized)*(bilateral_grid.x.extent_realized.s + 1)))] = (let t58 = float32(((bilateral_grid.s0.x.__thread_id_x.base + .__thread_id_x) % 16)) in (let t59 = float32(((bilateral_grid.s0.y.__thread_id_y.base + .__thread_id_y) % 16)) in (let t60.s = min(i0[((bilateral_grid.s0.x.__thread_id_x.base + .__thread_id_x) + ((bilateral_grid.s0.y.__thread_id_y.base + .__thread_id_y)*1280))], (uint8)1) in (let t61 = ((float32(t60.s)*1.666667f) - float32(int32((float32(t60.s)*1.666667f)))) in (let t64 = ((((bilateral_grid.s0.x.__thread_id_x.base + .__thread_id_x)/16) - blury.x.min_realized) + ((((bilateral_grid.s0.y.__thread_id_y.base + .__thread_id_y)/16) - blury.y.min_realized)*(blury.x.extent_realized.s + 1))) in (let t65.s = int32((float32(t60.s)*1.666667f)) in (let t66 = (t64 + (t65.s*blury.stride.2)) in (let t67 = ((((bilateral_grid.s0.x.__thread_id_x.base + .__thread_id_x)/16) - blury.x.min_realized) + (((((bilateral_grid.s0.y.__thread_id_y.base + .__thread_id_y)/16) - blury.y.min_realized) + 1)*(blury.x.extent_realized.s + 1))) in (let t68 = (t67 + (t65.s*blury.stride.2)) in (let t69.s = int32((float32(t60.s)*1.666667f)) in (let t70 = (t64 + ((t69.s + 1)*blury.stride.2)) in (let t71 = (t67 + ((t69.s + 1)*blury.stride.2)) in (let t72 = (t66 + (blury.stride.2*3)) in (let t73 = (t68 + (blury.stride.2*3)) in (let t74 = (t70 + (blury.stride.2*3)) in (let t75 = (t71 + (blury.stride.2*3)) in (lerp(lerp(lerp(blury[t66], blury[(t66 + 1)], (t58*0.062500f)), lerp(blury[t68], blury[(t68 + 1)], (t58*0.062500f)), (t59*0.062500f)), lerp(lerp(blury[t70], blury[(t70 + 1)], (t58*0.062500f)), lerp(blury[t71], blury[(t71 + 1)], (t58*0.062500f)), (t59*0.062500f)), t61)/lerp(lerp(lerp(blury[t72], blury[(t72 + 1)], (t58*0.062500f)), lerp(blury[t73], blury[(t73 + 1)], (t58*0.062500f)), (t59*0.062500f)), lerp(lerp(blury[t74], blury[(t74 + 1)], (t58*0.062500f)), lerp(blury[t75], blury[(t75 + 1)], (t58*0.062500f)), (t59*0.062500f)), t61))))))))))))))))))
          }
        }
      }
    }
    free blury
    set_dev_dirty(bilateral_grid.buffer, (uint8)1)
  }
  let copy_to_device_result$12 = halide_copy_to_device(f4.buffer, halide_opencl_device_interface())
  assert((copy_to_device_result$12 == 0), copy_to_device_result$12)
  produce f4 {
    let copy_to_device_result$11 = halide_copy_to_device(f4.buffer, halide_opencl_device_interface())
    assert((copy_to_device_result$11 == 0), copy_to_device_result$11)
    let copy_to_device_result$10 = halide_copy_to_device(bilateral_grid.buffer, halide_opencl_device_interface())
    assert((copy_to_device_result$10 == 0), copy_to_device_result$10)
    parallel<OpenCL> (f4.s0.y.__block_id_y, 0, ((f4.extent.1 + 7)/8)) {
      parallel<OpenCL> (f4.s0.x.__block_id_x, 0, ((f4.extent.0 + 7)/8)) {
        allocate __shared[uint8 * 512]
        parallel<OpenCL> (.__thread_id_y, 0, 8) {
          parallel<OpenCL> (.__thread_id_x, 0, 8) {
            let f4.s0.y.__thread_id_y.base = min(((f4.s0.y.__block_id_y*8) + f4.min.1), ((f4.min.1 + f4.extent.1) + -8))
            let f4.s0.x.__thread_id_x.base = min(((f4.s0.x.__block_id_x*8) + f4.min.0), ((f4.min.0 + f4.extent.0) + -8))
            produce f2 {
              __shared[((.__thread_id_x + (.__thread_id_y*8)) + 64)] = (let t78.s = max(min(((.__thread_id_x + f4.s0.x.__thread_id_x.base) + 1), 1279), 0) in (let t79 = ((max(min(((.__thread_id_y + f4.s0.y.__thread_id_y.base) + -1), 799), 0) - bilateral_grid.y.min_realized)*(bilateral_grid.x.extent_realized.s + 1)) in (let t80.s = max(min(((.__thread_id_x + f4.s0.x.__thread_id_x.base) + -1), 1279), 0) in (let t81 = ((max(min((.__thread_id_y + f4.s0.y.__thread_id_y.base), 799), 0) - bilateral_grid.y.min_realized)*(bilateral_grid.x.extent_realized.s + 1)) in (let t82 = ((max(min(((.__thread_id_y + f4.s0.y.__thread_id_y.base) + 1), 799), 0) - bilateral_grid.y.min_realized)*(bilateral_grid.x.extent_realized.s + 1)) in (((((bilateral_grid[((t78.s - bilateral_grid.x.min_realized) + t79)] - bilateral_grid[((t80.s - bilateral_grid.x.min_realized) + t79)]) - (bilateral_grid[((t80.s - bilateral_grid.x.min_realized) + t81)]*2.000000f)) + (bilateral_grid[((t78.s - bilateral_grid.x.min_realized) + t81)]*2.000000f)) - bilateral_grid[((t80.s - bilateral_grid.x.min_realized) + t82)]) + bilateral_grid[((t78.s - bilateral_grid.x.min_realized) + t82)]))))))
              halide_gpu_thread_barrier()
            }
            produce f3 {
              __shared[(.__thread_id_x + (.__thread_id_y*8))] = (let t85.s = max(min(((.__thread_id_x + f4.s0.x.__thread_id_x.base) + -1), 1279), 0) in (let t86 = ((max(min(((.__thread_id_y + f4.s0.y.__thread_id_y.base) + -1), 799), 0) - bilateral_grid.y.min_realized)*(bilateral_grid.x.extent_realized.s + 1)) in (let t87.s = max(min((.__thread_id_x + f4.s0.x.__thread_id_x.base), 1279), 0) in (let t88.s = max(min(((.__thread_id_x + f4.s0.x.__thread_id_x.base) + 1), 1279), 0) in (let t89 = ((max(min(((.__thread_id_y + f4.s0.y.__thread_id_y.base) + 1), 799), 0) - bilateral_grid.y.min_realized)*(bilateral_grid.x.extent_realized.s + 1)) in ((((((bilateral_grid[((t85.s - bilateral_grid.x.min_realized) + t86)]*-1.000000f) - (bilateral_grid[((t87.s - bilateral_grid.x.min_realized) + t86)]*2.000000f)) - bilateral_grid[((t88.s - bilateral_grid.x.min_realized) + t86)]) + bilateral_grid[((t85.s - bilateral_grid.x.min_realized) + t89)]) + (bilateral_grid[((t87.s - bilateral_grid.x.min_realized) + t89)]*2.000000f)) + bilateral_grid[((t88.s - bilateral_grid.x.min_realized) + t89)]))))))
              halide_gpu_thread_barrier()
            }
            f4[(((f4.s0.x.__thread_id_x.base + .__thread_id_x) + ((f4.s0.y.__thread_id_y.base + .__thread_id_y)*f4.stride.1)) - (f4.min.0 + (f4.min.1*f4.stride.1)))] = (let t90 = (.__thread_id_x + (.__thread_id_y*8)) in (let t91 = __shared[(t90 + 64)] in (let t92 = __shared[t90] in uint8((sqrt_f32(((t91*t91) + (t92*t92)))*255.000000f)))))
          }
        }
        free __shared
      }
    }
    free bilateral_grid
    set_dev_dirty(f4.buffer, (uint8)1)
  }
  0
}
