#ifndef _FRACTAL_SERIAL_H
#define _FRACTAL_SERIAL_H
#include <cstdint>
#pragma once

void julia_serial(double x_min, double y_min, double x_max, double y_max, uint32_t* pixel_buffer);
void julia_serial_2(double x_min, double y_min, double x_max, double y_max, uint32_t* pixel_buffer);



#endif