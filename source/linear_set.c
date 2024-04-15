#include "linear_set.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Complex* generateLinearSet(int width, int height, LINEAR_SET_VALUE_TYPE magnification, const Complex* center)
{
    Complex* grid = malloc(width * height * sizeof(Complex));

    // preserve 1:1 ratio on the graph
    const LINEAR_SET_VALUE_TYPE aspectRatio = (LINEAR_SET_VALUE_TYPE)width / height;

    LINEAR_SET_VALUE_TYPE yDelta = (4.0 / height) / magnification;
    LINEAR_SET_VALUE_TYPE xDelta = (4.0 / width * aspectRatio) / magnification;

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            LINEAR_SET_VALUE_TYPE xOffset = creal(*center) - (2.0 * aspectRatio) / magnification;
            LINEAR_SET_VALUE_TYPE yOffset = cimag(*center) - 2.0 / magnification;

            // m_grid(x, y) = { static_cast<Value_type>(x)*xDelta + xDelta/2.0, static_cast<Value_type>(y)*yDelta +
            // yDelta/2.0 };
            size_t idx  = y * width + x;
            grid[idx]   = (x * xDelta + xDelta / 2.0) + (y * yDelta + yDelta / 2.0) * I;
            grid[idx]  += xOffset + yOffset * I;
        }
    }

    return grid;
}

void printLinearSet(int width, int height, Complex* set)
{
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            size_t                idx  = y * width + x;
            LINEAR_SET_VALUE_TYPE real = creal(set[idx]);
            LINEAR_SET_VALUE_TYPE imag = cimag(set[idx]);
            printf("(%.2f%c%.2fi)   ", real, "+-"[imag < 0.0], fabs(imag));
        }
        printf("\n");
    }
}

void deleteLinearSet(Complex* set)
{
    free(set);
}
