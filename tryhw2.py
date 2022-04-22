from uwimg import *
im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-box7")

res = sobel_image(im)
mag = res[0]
feature_normalize(mag)
save_image(mag, "magnitude")

res = colorize_sobel(im)
feature_normalize(res)
save_image(res, "colorize")