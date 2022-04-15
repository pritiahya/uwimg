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
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    return 0;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    return make_image(1,1,1);
}

