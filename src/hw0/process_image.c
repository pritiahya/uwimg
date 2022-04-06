#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    if (x < 0) {
        x = 0;
    } else if (x >= im.w) {
        x = im.w -1;
    }

    if (y < 0) {
        y = 0;
    } else if (y >= im.h) {
        y = im.h -1;
    }
    
    int index = x + (y * im.w) + (c * im.w * im.h);
    return im.data[index];
;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    if(y >= 0 && y < im.h && x >=0 && x < im.w && c>= 0 && c<im.c) {
        int index = x + (y*im.w) + (c * im.w * im.h);
        im.data[index] = v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, im.w * im.h * im.c * sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    int area = im.w * im.h;
    for (int i = 0; i < area; i++) {
        *(gray.data + i) = (0.299 * (*(im.data + i))) + (0.587 * (*(im.data + i + area))) + (0.114 * (*(im.data + i + (2 * area))));
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for(int x = 0; x < im.w; x++) {
        for(int y = 0; y < im.h; y++) {
            float newVal = get_pixel(im, x, y, c) + v;
            set_pixel(im, x, y, c, newVal);     
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for(int x = 0; x < im.w; x++) {
        for(int y = 0; y < im.h; y++) {
            for(int c = 0; c < im.c; c++) {
                float pixelVal = get_pixel(im, x, y, c);
                if (pixelVal < 0) {
                    set_pixel(im, x, y, c, 0);
                } else if (pixelVal > 1) {
                    set_pixel(im, x, y, c, 1);
                }
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
}
