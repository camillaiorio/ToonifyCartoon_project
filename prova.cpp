//
// Created by biar on 06/06/22.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

#define M_PI 3.14159265358979323846
using namespace std;

Image quantize_bw(Image& im, int a){
    Image res(im.w,im.h,0);//da restituire

    //riempio res.
        for(int i=0; i<im.h;i++){
            for(int j=0;j<im.w;j++){
                res(j,i,0) = (floor((im(j,i,0)*255)/a)*a)/255;
                //printf("%f %f\n", im.pixel(j,i,0), res.pixel(j,i,0) );
            }
        }
    return res;
}
