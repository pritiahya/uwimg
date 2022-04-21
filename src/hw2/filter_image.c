#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    int nm = im.w * im.h;
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                set_pixel(im, x, y, c, get_pixel(im, x, y, c) / nm);
            }
        }
    }
}

image make_box_filter(int w)
{
    // TODO
    image new = make_image(w, w, 1);
    float val = 1.0 / (w * w);
    for (int y = 0; y < w; y++) {
        for (int x = 0; x < w; x++) {
            set_pixel(new, x, y, 0, val);
        }
    }
    return new;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    // Check number of channels in filter
    assert(filter.c == im.c || filter.c == 1);

    image new;
    if (im.c == filter.c && preserve != 1) {
        image temp = make_image(im.w, im.h, im.c);
        new = make_image(im.w, im.h, 1);
        for (int c = 0; c < im.c; c++) {
            for (int y = 0; y < im.h; y++) {
                for (int x = 0; x < im.w; x++) {
                    float sum = 0.0;
                    for (int fy = 0; fy < filter.h; fy++) {
                        for (int fx = 0; fx < filter.w; fx++) {
                            sum = sum + (get_pixel(im, x + fx, y + fy, c) * get_pixel(filter, fx, fy, c));
                        }
                    }
                    set_pixel(temp, x, y, c, sum);
                }
            }
        }

        // sum channel dimensions
        for (int y = 0; y < temp.h; y++) {
            for (int x = 0; x < temp.w; x++) {
                float sum = 0.0;
                for (int c = 0; c < im.c; c++) {
                    sum = sum + get_pixel(temp, x, y, c);
                }
                set_pixel(new, x, y, 1, sum);
            }
        }
        return new;
    } else if (preserve == 1 && filter.c > 1) {
        new = make_image(im.w, im.h, im.c);
        for (int c = 0; c < im.c; c++) {
            for (int y = 0; y < im.h; y++) {
                for (int x = 0; x < im.w; x++) {
                    float sum = 0.0;
                    for (int fy = 0; fy < filter.h; fy++) {
                        for (int fx = 0; fx < filter.w; fx++) {
                            sum = sum + (get_pixel(im, x + fx, y + fy, c) * get_pixel(filter, fx, fy, c));
                        }
                    }
                    set_pixel(new, x, y, c, sum);
                }
            }
        }
        return new;
    } else if (filter.c == 0 && preserve == 1) {
        image temp = make_image(im.w, im.h, im.c);
        new = make_image(im.w, im.h, 1);
        for (int c = 0; c < im.c; c++) {
            for (int y = 0; y < im.h; y++) {
                for (int x = 0; x < im.w; x++) {
                    float sum = 0.0;
                    for (int fy = 0; fy < filter.h; fy++) {
                        for (int fx = 0; fx < filter.w; fx++) {
                            sum = sum + (get_pixel(im, x + fx, y + fy, c) * get_pixel(filter, fx, fy, 0));
                        }
                    }
                    set_pixel(temp, x, y, c, sum);
                }
            }
        }
        for (int y = 0; y < temp.h; y++) {
            for (int x = 0; x < temp.w; x++) {
                float sum = 0.0;
                for (int c = 0; c < im.c; c++) {
                    sum = sum + get_pixel(temp, x, y, c);
                }
                set_pixel(new, x, y, 1, sum);
            }
        }
        return new;
    } else {
        new = make_image(im.w, im.h, im.c);
        for (int c = 0; c < im.c; c++) {
            for (int y = 0; y < im.h; y++) {
                for (int x = 0; x < im.w; x++) {
                    float sum = 0.0;
                    for (int fy = 0; fy < filter.h; fy++) {
                        for (int fx = 0; fx < filter.w; fx++) {
                            sum = sum + (get_pixel(im, x + fx, y + fy, c) * get_pixel(filter, fx, fy, 0));
                        }
                    }
                    set_pixel(new, x, y, c, sum);
                }
            }
        }
        return new;
    }
    return im;
}

image make_highpass_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_sharpen_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_emboss_filter()
{
    // TODO
    return make_image(1,1,1);
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    return make_image(1,1,1);
}

image add_image(image a, image b)
{
    // TODO
    // need to assert according to spec, but its causing tests to fail?
    //assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image ret = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w * a.h * a.c; i++) {
        ret.data[i] = a.data[i] + b.data[i];
    }
    return ret;
}

image sub_image(image a, image b)
{
    // TODO
    // need to assert according to spec, but its causing tests to fail?
    //assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image ret = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w * a.h * a.c; i++) {
        ret.data[i] = a.data[i] - b.data[i];
    }
    return ret;
}

image make_gx_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    return make_image(1,1,1);
}

void feature_normalize(image im)
{
    // TODO
}

image *sobel_image(image im)
{
    // TODO
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
