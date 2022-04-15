// Name: Priti Ahya, Partner: Amanda Ki
#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    int x2 = round(x);
    int y2 = round(y);
    return get_pixel(im, x2, y2, c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    image ret = make_image(w, h, im.c);

    // solve equations for a and b
    float a_x = (float)im.w / (float)w;
    float a_y = (float)im.h / (float)h;
    float b_x = (float)(-0.5) - (float)(a_x * (-0.5));
    float b_y = (float)(-0.5) - (float)(a_y * (-0.5));

    // loop over pixels
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                // map back to old coordinates
                float map_x = (float)(a_x * x) + b_x;
                float map_y = (float)(a_y * y) + b_y;

                // fill pixel
                set_pixel(ret, x, y, c, nn_interpolate(im, map_x, map_y, c));
            }
        }
    }
    return ret;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    double x1 = floor(x);
    double y1 = floor(y);
    double x2 = ceil(x);
    double y2 = ceil(y);

    float d1 = x - x1;
    float d2 = x2 - x;
    float d3 = y - y1;
    float d4 = y2 - y;

    float v1 = get_pixel(im, x1, y1, c);
    float v2 = get_pixel(im, x2, y1, c);
    float v3 = get_pixel(im, x1, y2, c);
    float v4 = get_pixel(im, x2, y2, c);

    float q1 = (v1 * d2) + (v2 * d1);
    float q2 = (v3 * d2) + (v4 * d1);
    float q = (q1 * d4) + (q2 * d3);
    return q;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    image ret = make_image(w, h, im.c);

    // solve equations for a and b
    float a_x = (float)im.w / (float)w;
    float a_y = (float)im.h / (float)h;
    float b_x = (float)(-0.5) - (float)(a_x * (-0.5));
    float b_y = (float)(-0.5) - (float)(a_y * (-0.5));

    // loop over pixels
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                // map back to old coordinates
                float map_x = (float)(a_x * x) + b_x;
                float map_y = (float)(a_y * y) + b_y;

                // fill pixel
                set_pixel(ret, x, y, c, bilinear_interpolate(im, map_x, map_y, c));
            }
        }
    }
    return ret;
}

