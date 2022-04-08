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
    int area = im.w * im.h;

    for (int i = 0; i < area; i++) {
        float val;
        float sat;
        float hue;
        float r = *(im.data + i);
        float g = *(im.data + i + area);
        float b = *(im.data + i + (2 * area));

        // calculate Value
        val = three_way_max(r, g, b);

        // calculate Saturation
        float c = 0;
        if (r == 0 && g == 0 && b == 0) {
            sat = 0;
        } else {
            float min = three_way_min(r, g, b);
            c = val - min;
            sat = c / val;
        }

        // calculate Hue
        float h = 0;
        if (c == 0) {
            hue = 0;
        } else if (val == r) {  // V = R
            h = (g - b) / c;
        } else if (val == g) {  // V = G
            h = ((b - r) / c) + 2;
        } else if (val == b) {  // V = B
            h = ((r - g) / c) + 4;
        }
        if (c != 0) {
            if (h < 0) {
                hue = (h / 6) + 1;
            } else {
                hue = h / 6;
            }
        }

        im.data[i] = hue;
        im.data[i + area] = sat;
        im.data[i + (2 * area)] = val;
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    int area = im.w * im.h;

    for (int i = 0; i < area; i++) {
        float hue = im.data[i];
        float sat = im.data[i + area];
        float val = im.data[i + (2 * area)];

        float c = val * sat;
        float m = val - c;
        float h = hue * 6;
        float r = 0;
        float g = 0;
        float b = 0;

        if (h >= 0 && h < 1) {  // V = R, H' positive
            r = val;
            g = (c * h) + m;
            b = m;
        } else if (h >= 1 && h < 2) {   // V = G, H' negative
            r = m - (c * (h - 2));
            g = val;
            b = m;
        } else if (h >= 2 && h < 3) {   // V = G, H' positive
            r = m;
            g = val;
            b = m + (c * (h - 2));
        } else if (h >= 3 && h < 4) {   // V = B, H' negative
            r = m;
            g = m - (c * (h - 4));
            b = val;
        } else if (h >= 4 && h < 5) {   // V = B, H' positive
            r = m + (c * (h - 4));
            g = m;
            b = val;
        } else if (h >= 5 && h < 6) {   // V = R, H' negative
            r = val;
            g = m;
            b = m - (c * h);
        }

        im.data[i] = r;
        im.data[i + area] = g;
        im.data[i + (2 * area)] = b;
    }
}

// extra credit
void scale_image(image im, int c, float v) 
{
    for(int x = 0; x < im.w; x++) {
        for(int y = 0; y < im.h; y++) {
            set_pixel(im, x, y, c, get_pixel(im, x, y, c) * v);
        }
    }
}