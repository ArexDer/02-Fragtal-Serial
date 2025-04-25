#include "fractal_serial.h"
#include <iostream>
#include <complex>
#include <cstdint>

#include "palette.h"



#define WIDTH 1600
#define HEIGHT 900
extern int max_iteraciones;

// Constante C fija para Julia
const std::complex<double> c(-0.7, 0.27015);

// Función divergente usando std::complex
int divergente_julia(std::complex<double> z) {
    int iter = 0;
    while (std::abs(z) < 2.0 && iter < max_iteraciones) {
        z = z * z + c;
        iter++;
    }

    if (iter < max_iteraciones) {
        int index = iter % 16;
        return color_ramp[index];
    }

    return 0X00000000;
}

// Función divergente usando variables reales
int divergente_julia_2(double zr, double zi) {
    int iter = 0;
    double cr = -0.7;
    double ci = 0.27015;

    while ((zr * zr + zi * zi) < 4.0 && iter < max_iteraciones) {
        double dr = zr * zr - zi * zi + cr;
        double di = 2.0 * zr * zi + ci;

        zr = dr;
        zi = di;

        iter++;
    }

    if (iter < max_iteraciones) {
        int index = iter % 16;
        return color_ramp[index];
    }

    return 0X00000000;
}

// Julia con std::complex
void julia_serial(double x_min, double y_min, double x_max, double y_max, uint32_t* pixel_buffer) {
    double dx = (x_max - x_min) / WIDTH;
    double dy = (y_max - y_min) / HEIGHT;

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            double x = x_min + i * dx;
            double y = y_max - j * dy;

            std::complex<double> z(x, y);
            pixel_buffer[j * WIDTH + i] = divergente_julia(z);
        }
    }
}

// Julia con variables reales
void julia_serial_2(double x_min, double y_min, double x_max, double y_max, uint32_t* pixel_buffer) {
    double dx = (x_max - x_min) / WIDTH;
    double dy = (y_max - y_min) / HEIGHT;

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            double x = x_min + i * dx;
            double y = y_max - j * dy;

            pixel_buffer[j * WIDTH + i] = divergente_julia_2(x, y);
        }
    }
}
