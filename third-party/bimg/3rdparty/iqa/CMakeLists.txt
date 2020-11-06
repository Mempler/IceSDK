add_library(iqa STATIC
        source/convolve.c
        source/decimate.c
        source/math_utils.c
        source/ms_ssim.c
        source/mse.c
        source/psnr.c
        source/ssim.c)

target_include_directories(iqa PUBLIC include)
