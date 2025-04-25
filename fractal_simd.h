#ifndef _FRACTAL_SIMD_H
#define _FRACTAL_SIMD_H
#include <cstdint>
#pragma once

void julia_simd(double x_min, double y_min, double x_max, double y_max, uint32_t* pixel_buffer);
void julia_simd_2(double x_min, double y_min, double x_max, double y_max, uint32_t* pixel_buffer);



#endif