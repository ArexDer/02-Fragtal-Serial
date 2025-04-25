#include "fractal_serial.h"
#include <immintrin.h> //avx
#include <complex>
#include <vector>
#include <cstdint>

#define WIDTH 1600
#define HEIGHT 900

extern int max_iteraciones;

const std::complex<double> c;
#define PALETTE_SIZE 16
extern std::vector<unsigned int> color_ramp;

void julia_simd(double x_min, double y_min, double x_max, double y_max, uint32_t *pixel_buffer)
{

    double dx = (x_max - x_min) / WIDTH;
    double dy = (y_max - y_min) / HEIGHT;

    // {x_min, x_min, ..., x_min}  //serian 8 veces
    __m256 xmin = _mm256_set1_ps(x_min);

    // {y_max, y_max, ..., y_max}  //serian 8 veces
    __m256 ymax = _mm256_set1_ps(y_max);

    __m256 xscale = _mm256_set1_ps(dx);
    __m256 yscale = _mm256_set1_ps(dy);

    // definimos el C
    __m256 c_real = _mm256_set1_ps(c.real());
    __m256 c_imag = _mm256_set1_ps(c.imag());

    __m256 threshold = _mm256_set1_ps(4);

    __m256 one = _mm256_set1_ps(1);

    // vector

    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j += 8)
        {
            //{i, i, i, i, i, i, i, i,}
            __m256 mx = _mm256_set1_ps(i);

            //[j+0, j+1, j+2, j+3, j+4, j+5, j+6, j+7]

            __m256 my = _mm256_set_ps((float)(j + 7), (float)(j + 6), (float)(j + 5), (float)(j + 4),
                          (float)(j + 3), (float)(j + 2), (float)(j + 1), (float)(j + 0));


            int iter = 1;

            /*
            X = xmin +i*dx
            y = ymax-j*dy
            */

            // Los mx serian los i, scale (dx, dx, ... 8 veces)
            __m256 cr = _mm256_add_ps(_mm256_mul_ps(mx, xscale), xmin); // Lo que nos va a dar son todos los X.

            // Ahora la IMAGINARIA de los complejos
            __m256 ci = _mm256_sub_ps(ymax, _mm256_mul_ps(my, yscale));

            // vamos a guardar una copia
            __m256 zr = cr;
            __m256 zi = ci;

            __m256 mk = _mm256_set1_ps(iter); // todos van a empezar con 1 --< 7 vees

            while (iter < max_iteraciones)
            {
                // Zn+1 = Zn^2 +c
                __m256 zr2 = _mm256_mul_ps(zr, zr);
                __m256 zi2 = _mm256_mul_ps(zi, zi);
                __m256 zrzi = _mm256_mul_ps(zr, zi);

                // = zr*zr-zi*zi + c.real
                zr = _mm256_add_ps(_mm256_sub_ps(zr2, zi2), c_real);
                // 2*zr*zi+c.imag
                zi = _mm256_add_ps(_mm256_add_ps(zrzi, zrzi), c_imag);

                // Norma
                zr2 = _mm256_mul_ps(zr, zr);
                zi2 = _mm256_mul_ps(zi, zi);

                __m256 mag2 = _mm256_add_ps(zr2, zi2);

                // comparar las 8 normas
                __m256 mask = _mm256_cmp_ps(mag2, threshold, _CMP_LE_OS); // COMPALES DAN

                /*
                Necesitamos un vector para que compare de 8 en 8 []
                con un AND a nivel de bits 1 = PASA, 0 = cero y no pasa
                */
                mk = _mm256_add_ps(_mm256_and_ps(mask, one), mk);

                if (_mm256_testz_ps(mask, _mm256_set1_ps(-1)))
                {
                    // si no hay ceros, entonces no se ha salido de la norma
                    break;
                }
                // si no se ha salido de la norma, entonces seguimos iterando
                // zr = zr2 - zi2 + c.real
                // zi = 2*zr*zi + c.imag
                // iter++;
                // zr = zr2 - zi2 + c.real
                // zi = 2*zr*zi + c.imag
                // iter++;
                //...

                iter++;
            }
            // Ahora desempaquetamos
            float d[8];
            _mm256_store_ps(d, mk); // desempaquetamos el vector de iteraciones
            // ahora tenemos que guardar el color en el pixel buffer
            for (int it = 0; it < 8; it++)
            {
                int index = (j + it) * WIDTH + i; // el index del pixel buffer
                if (index < WIDTH * HEIGHT)
                {
                    if (d[it] < max_iteraciones)
                    { // NO ES ACOTADO
                        // si no ha llegado al maximo de iteraciones, entonces guardamos el color
                        int color_idx = (int)d[it] % PALETTE_SIZE;
                        pixel_buffer[index] = color_ramp[color_idx]; // guardamos el color
                    }
                    else
                    {
                        pixel_buffer[index] = 0; // si no ha llegado al maximo de iteraciones, entonces
                    }
                }
            }
        }
    }
} // fin de la funcion
