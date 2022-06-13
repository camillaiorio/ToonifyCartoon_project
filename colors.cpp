#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

#define M_PI 3.14159265358979323846

//1)Bilateral filter

//downsampled by a factor of 4
Image resizing(Image& im){
    Image f = make_box_filter(7);
    Image blur = convolve_image(im,f,true);
    Image rsz = nearest_resize(blur,im.w/4,im.h/4);
    return rsz;
}

//apply bilateral filter

//2) Median Filter

Image median_filter_color(const Image& im,float sigma){ //filtro 7x7 sigma è uguale a 1
    Image new_im = im;
    Image f = make_gaussian_filter(sigma);
    Image conv = convolve_image(new_im, f, true);

    return conv;
}

//3) Quantize Colors
Image quantize_colors(const Image& im, int a){ //a= factor to be reduced
    Image res(im.w,im.h,im.c);//da restituire

    //riempio res.
    for(int ch=0; ch<im.c ; ch++){
        for(int i=0; i<im.h;i++){
            for(int j=0;j<im.w;j++){
                res(j,i,ch) = (floor((im(j,i,ch)*255)/a)*a)/255;
                //printf("%f %f\n", im.pixel(j,i,ch), res.pixel(j,i,ch) );
            }
        }
    }
    return res;
}
//recombine
Image recombine(const Image& im_edges, const Image& im_color){
    Image res(im_edges.w,im_edges.h, im_color.c);


    for(int ch=0; ch<im_color.c; ch++){
        for(int i=0; i<im_edges.h;i++){
            for(int j=0;j<im_edges.w;j++){
                if (im_edges(j,i,0)==1){
                    res(j,i,ch)= 0;
                }else{
                    res(j,i,ch) = im_color(j,i,ch);
                }
            }
        }
    }



    return res;
}

