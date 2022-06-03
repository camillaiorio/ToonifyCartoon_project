#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

#define M_PI 3.14159265358979323846

//1)Bilateral filter

//filtro di normalizzazione: somma tutti i pixel e li divide in modo che la somma sia uno.(per ogni canale)
void normalization( Image& im){

    for(int ch=0; ch< im.c ; ++ch){
        float tot=0;
        //sommo
        for(int i=0;i<im.w; ++i){
            for(int j=0; j<im.h; ++j){
                tot += im(i,j, ch);
            }

        }
        //divido
        for(int i=0;i<im.w; ++i){
            for(int j=0; j<im.h; ++j){
               im(i,j, ch) =  im(i,j, ch) / tot;
            }
        }
    }


}
//make box filter

void resize_image(){

}

Image make_bilateral_filter_f (const Image &im, const Image &sgf, int cx, int cy, int cc, float sigma) {

    // Color gaussian filter
    Image cgf (sgf.w, sgf.h, 1);

    for (int y = 0; y < sgf.w; y ++) {
        for (int x = 0; x < sgf.w; x ++) {
            int ax = cx - sgf.w/2 + x;
            int ay = cy - sgf.w/2 + y;

            float diff = im.clamped_pixel(ax ,ay ,cc) - im.clamped_pixel(cx, cy, cc);

            float var = powf(sigma, 2);
            float c = 2 * M_PI * var;
            float p = - powf(diff,2) / (2 * var);
            float e = expf(p);
            float val = e / c;

            cgf(x,y,0) = val;

        }
    }
    Image bf (sgf.w, sgf.h, 1);

    // Multiply space gaussian by color gaussian
    for (int y = 0; y < bf.h; y ++) {
        for (int x = 0; x < bf.w; x ++) {
            bf (x,y,0) = sgf(x,y,0) * cgf(x,y,0);
        }
    }

    normalization(bf);

    return bf;
}

//9x9 kernel iterato 14 volte.

Image bilateral_filter_f(const Image& im, float sigma1, float sigma2){ //i due sigma per il bilateral filter
    Image gf = make_gaussian_filter(sigma1); //filtro gaussiano
    Image res(im.w, im.h, im.c);

    for (int c = 0; c < res.c; c ++) {
        for (int y = 0; y < im.h; y ++) {
            for (int x = 0; x < im.w; x ++) {
                Image bf = make_bilateral_filter_f(im, gf, x, y, c, sigma2);

                float sum = 0.0;
                // convoluzione
                for (int fy = 0; fy < gf.w; fy ++) {
                    for (int fx = 0; fx < gf.w; fx ++) {
                        int ax = x - bf.w/2 + fx;
                        int ay = y - bf.w/2 + fy;

                        sum += bf(fx,fy,0) * im.clamped_pixel( ax, ay, c);
                    }
                }

                res (x,y,c) = sum;

            }
        }
    }

    return res;
}


//2) Median Filter

Image median_filter_color(const Image& im,float sigma){ //filtro 7x7 sigma è uguale a 1
    Image new_im = im;
    Image f = make_gaussian_filter(sigma);
    Image conv = convolve_image(new_im, f, true);

    return conv;
}

//3) Quantize Colors
Image quantize_colors(const Image& im, float a){ //a= factor to be reduced
    Image res(im.w,im.h,im.c);//da restituire

    //riempio res.
    for(int ch=0; ch<im.c ; ch++){
        for(int i=0; i<im.h;i++){
            for(int j=0;j<im.w;j++){
                res(j,i,ch) = (im(j,i,ch)/a)*a;
                printf("%x %x\n", im(j,i,ch), res(j,i,ch) );
            }
        }
    }
    return res;
}