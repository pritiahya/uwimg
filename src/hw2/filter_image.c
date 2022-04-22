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
    float sum = 0;
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                sum += get_pixel(im, x, y, c);
            }
        }
    }
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                set_pixel(im, x, y, c, get_pixel(im, x, y, c) / sum);
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

    // Find center of filter
    int mid_x = filter.w / 2;
    int mid_y = filter.h / 2;

    if (im.c == filter.c && preserve != 1) {
        // filter and im have the same number of channels and returns a 1 channel image
        image temp = make_image(im.w, im.h, im.c);
        new = make_image(im.w, im.h, 1);
        for (int c = 0; c < im.c; c++) {
            for (int y = 0; y < im.h; y++) {
                for (int x = 0; x < im.w; x++) {
                    float sum = 0.0;
                    for (int fy = 0; fy < filter.h; fy++) {
                        for (int fx = 0; fx < filter.w; fx++) {
                            sum = sum + (get_pixel(im, x + fx - mid_x, y + fy - mid_y, c) * get_pixel(filter, fx, fy, c));
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
                set_pixel(new, x, y, 0, sum);
            }
        }
        return new;
    } else if (preserve == 1 && filter.c > 1) {
        // filter contains more than 1 channel and returns an image with the same number of channels as input
        new = make_image(im.w, im.h, im.c);
        for (int c = 0; c < im.c; c++) {
            for (int y = 0; y < im.h; y++) {
                for (int x = 0; x < im.w; x++) {
                    float sum = 0.0;
                    for (int fy = 0; fy < filter.h; fy++) {
                        for (int fx = 0; fx < filter.w; fx++) {
                            sum = sum + (get_pixel(im, x + fx - mid_x, y + fy - mid_y, c) * get_pixel(filter, fx, fy, c));
                        }
                    }
                    set_pixel(new, x, y, c, sum);
                }
            }
        }
        return new;
    } else if (filter.c == 1 && preserve == 0) {
        // filter only contains 1 channel and returns a 1 channel image
        image temp = make_image(im.w, im.h, im.c);
        new = make_image(im.w, im.h, 1);
        for (int c = 0; c < im.c; c++) {
            for (int y = 0; y < im.h; y++) {
                for (int x = 0; x < im.w; x++) {
                    float sum = 0.0;
                    for (int fy = 0; fy < filter.h; fy++) {
                        for (int fx = 0; fx < filter.w; fx++) {
                            sum = sum + (get_pixel(im, x + fx - mid_x, y + fy - mid_y, c) * get_pixel(filter, fx, fy, 0));
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
                set_pixel(new, x, y, 0, sum);
            }
        }
        return new;
    } else {
        // filter contains only 1 channel and returns an image with the same number of channels as input
        new = make_image(im.w, im.h, im.c);
        for (int c = 0; c < im.c; c++) {
            for (int y = 0; y < im.h; y++) {
                for (int x = 0; x < im.w; x++) {
                    float sum = 0.0;
                    for (int fy = 0; fy < filter.h; fy++) {
                        for (int fx = 0; fx < filter.w; fx++) {
                            sum = sum + (get_pixel(im, x + fx - mid_x, y + fy - mid_y, c) * get_pixel(filter, fx, fy, 0));
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
    image ret = make_image(3, 3, 1);
    set_pixel(ret, 0, 0, 0, 0);
    set_pixel(ret, 0, 1, 0, -1);
    set_pixel(ret, 0, 2, 0, 0);
    set_pixel(ret, 1, 0, 0, -1);
    set_pixel(ret, 1, 1, 0, 4);
    set_pixel(ret, 1, 2, 0, -1);
    set_pixel(ret, 2, 0, 0, 0);
    set_pixel(ret, 2, 1, 0, -1);
    set_pixel(ret, 2, 2, 0, 0);
    return ret;
}

image make_sharpen_filter()
{
    // TODO
    image ret = make_image(3, 3, 1);
    set_pixel(ret, 0, 0, 0, 0);
    set_pixel(ret, 0, 1, 0, -1);
    set_pixel(ret, 0, 2, 0, 0);
    set_pixel(ret, 1, 0, 0, -1);
    set_pixel(ret, 1, 1, 0, 5);
    set_pixel(ret, 1, 2, 0, -1);
    set_pixel(ret, 2, 0, 0, 0);
    set_pixel(ret, 2, 1, 0, -1);
    set_pixel(ret, 2, 2, 0, 0);
    return ret;
}

image make_emboss_filter()
{
    // TODO
    image ret = make_image(3, 3, 1);
    set_pixel(ret, 0, 0, 0, -2);
    set_pixel(ret, 0, 1, 0, -1);
    set_pixel(ret, 0, 2, 0, 0);
    set_pixel(ret, 1, 0, 0, -1);
    set_pixel(ret, 1, 1, 0, 1);
    set_pixel(ret, 1, 2, 0, 1);
    set_pixel(ret, 2, 0, 0, 0);
    set_pixel(ret, 2, 1, 0, 1);
    set_pixel(ret, 2, 2, 0, 2);
    return ret;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: We should use preserve for the sharpen, emboss, and make_box filters because our convolution will return an image with the same number of channels as the input image when preserve is used. Using the dog image, if we use preserve with the sharpen filter, our convolution will return a sharpened dog image in color. If we use preserve with the emboss filter, our convolution will return a dog image in with the extra stylings. Using preserve in make_box_filter would blur the image, which should have color (not be in grayscale), hence we should use preserve.We would not use preserve for the highpass filter because the highpass filter finds edges. When preserve is not used, our convolution returns a 1 channel image, which is in grayscale. When using a highpass filter to find edges, it will be easier to see the edges in a grayscale image where the edges are white and other parts of the image are black.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: Post processing is required for the images where, after using the filters,
// the pixel values may be less that 0 or greater than 1. Post processing should not be required
// for make_box_filter, because none of the pixel values after applying this filter should be less
// than 0 or greater than 1. Post processing is required for the rest of the filters. Post processing
// can include using clamp_image, l1_normalize, or feature_normalize.

image make_gaussian_filter(float sigma)
{
    // TODO
    int k = 6*sigma;
    if (k % 2 == 0) {
        k++;
    }

    image ret = make_image(k, k, 1);

    int mid = k/2;

    for (int y = -mid; y < k - mid; y++) {
        for (int x = -mid; x < k - mid; x++) {
            float Gxy = ((float)1/(TWOPI*sigma*sigma)) * (expf((-x*x - y*y)/(2*sigma*sigma)));
            set_pixel(ret, x + mid, y + mid, 0, Gxy);
        }
    }
    l1_normalize(ret);
    return ret;

}

image add_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image ret = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w * a.h * a.c; i++) {
        ret.data[i] = a.data[i] + b.data[i];
    }
    return ret;
}

image sub_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image ret = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w * a.h * a.c; i++) {
        ret.data[i] = a.data[i] - b.data[i];
    }
    return ret;
}

image make_gx_filter()
{
    // TODO
    image ret = make_image(3, 3, 1);
    set_pixel(ret, 0, 0, 0, -1);
    set_pixel(ret, 0, 1, 0, -2);
    set_pixel(ret, 0, 2, 0, -1);
    set_pixel(ret, 1, 0, 0, 0);
    set_pixel(ret, 1, 1, 0, 0);
    set_pixel(ret, 1, 2, 0, 0);
    set_pixel(ret, 2, 0, 0, 1);
    set_pixel(ret, 2, 1, 0, 2);
    set_pixel(ret, 2, 2, 0, 1);
    return ret;
}

image make_gy_filter()
{
    // TODO
    image ret = make_image(3, 3, 1);
    set_pixel(ret, 0, 0, 0, -1);
    set_pixel(ret, 0, 1, 0, 0);
    set_pixel(ret, 0, 2, 0, 1);
    set_pixel(ret, 1, 0, 0, -2);
    set_pixel(ret, 1, 1, 0, 0);
    set_pixel(ret, 1, 2, 0, 2);
    set_pixel(ret, 2, 0, 0, -1);
    set_pixel(ret, 2, 1, 0, 0);
    set_pixel(ret, 2, 2, 0, 1);
    return ret;
}

void feature_normalize(image im)
{
    // TODO
    float min = 0;
    float max = 0;
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                float pix = get_pixel(im, x, y, c);
                min = MIN(pix, min);
                max = MAX(pix, max);
            }
        }
    }

    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                float r = max - min;
                if(max - min == 0) {
                    set_pixel(im, x, y, c, 0);
                } else {
                    float newVal = (float)(get_pixel(im, x, y, c) - min) / r;
                    set_pixel(im, x, y, c, newVal);
                }
            }
        }
    }
}

image *sobel_image(image im)
{
    // TODO

    // Compute gradient magnitude and direction
    image f_gx = make_gx_filter();
    image f_gy = make_gy_filter();
    image gx = convolve_image(im, f_gx, 0);
    image gy = convolve_image(im, f_gy, 0);

    image g = make_image(im.w, im.h, 1);
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            set_pixel(g, x, y, 0, sqrtf(get_pixel(gx, x, y, 0)*get_pixel(gx, x, y, 0) + get_pixel(gy, x, y, 0)*get_pixel(gy, x, y, 0)));
        }
    }

    image d = make_image(im.w, im.h, 1);
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            set_pixel(d, x, y, 0, atan2f(get_pixel(gy, x, y, 0), get_pixel(gx, x, y, 0)));
        }
    }

    image *res = calloc(2, sizeof(image));
    res[0] = g;
    res[1] = d;

    return res;
}

image colorize_sobel(image im)
{
    // TODO
    image color = make_image(im.w, im.h, 3);

    // smooth image
    image f = make_gaussian_filter(2);
    image blur = convolve_image(im, f, 1);
    clamp_image(blur);

    // sobel
    image *res = sobel_image(blur);
    image mag = res[0];
    image ang = res[1];
    feature_normalize(mag);
    feature_normalize(ang);
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            float hue = get_pixel(ang, x, y, 0);
            float sat = get_pixel(mag, x, y, 0);
            float val = get_pixel(mag, x, y, 0);
            set_pixel(color, x, y, 0, hue);
            set_pixel(color, x, y, 1, sat);
            set_pixel(color, x, y, 2, val);
        }
    }
    hsv_to_rgb(color);
    return color;
}
