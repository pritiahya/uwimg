#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    return 0;
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    return make_image(1,1,1);
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
    return make_image(1,1,1);
}

