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

//provare a clusterizzare con algortimo k-means

//random pixel
float* choose_clusters(const Image &im, int k){
    float* array = new float[k];

    for(int i=0; i<k; i++){
        int rmd= rand()%(im.h * im.w);//numero random
        int rmd_x= rmd%im.h; //resto a quale colonna
        int rmd_y= rmd/im.w; //colonna quale riga
        array[i] = im.pixel(rmd_x,rmd_y,0);
        //printf("%f\n", array[i]);
    }
    return array;
}
struct Clustered_pixel{
    int x;
    int y;
    float value;
    int cluster;

    Clustered_pixel()=default;

    Clustered_pixel(int x, int y, float value, int cluster):x(x),y(y),value(value),cluster(cluster){

    }
};


//image segmentation
Image image_segmentation(const Image& im, int k){
    float* array=choose_clusters(im,k);
    int* old_cluster_num = new int[k];//controlliamo se ogni cluster ha la stessa lunghezza
    bool should_stop = false;
    Clustered_pixel *pixels = new Clustered_pixel[im.h * im.w]; //array vuoto
    while(!should_stop) {


        float *cluster_sum = new float[k];
        int *cluster_num = new int[k];

        for (int i = 0; i < im.h; i++) {
            for (int j = 0; j < im.w; j++) {
                int best_cluster = 0;
                float best_difference = im.pixel(j, i, 0)- array[0];
                for (int u = 1; u < k; u++) {
                    float current_difference = im.pixel(j, i, 0) - array[u];
                    if (current_difference < best_difference) {
                        best_difference = current_difference;
                        best_cluster = u;
                    }

                }
                pixels[i * im.w + j].x = j;
                pixels[i * im.w + j].y = i;
                pixels[i * im.w + j].value = im.pixel(j, i, 0);
                pixels[i * im.w + j].cluster = best_cluster;

                cluster_sum[best_cluster] += im.pixel(j, i, 0);
                cluster_num[best_cluster] += 1;

            }

        }
        //calcola nuovi centri
        for (int z = 0; z < k; z++) {
            array[z] = cluster_sum[z] / cluster_num[z];
            printf("%d - %f\n",cluster_num[z], cluster_sum[z]);
        }

        //controllare variazioni

        bool cluster_same_size = true;

        for (int z = 0; z < k; z++) {
            if(old_cluster_num[z]!= cluster_num[z]){
                cluster_same_size=false;
            }
            old_cluster_num[z]= cluster_num[z];
            //printf("%f\n", array[z]);
        }
        should_stop = cluster_same_size;
        should_stop = true;
    }

    //devo scrivere l'immagine
    Image res(im.w,im.h,im.c);

    for(int z=0; z<im.w*im.h; z++){
        res.set_pixel(pixels[z].x,pixels[z].y,0,array[pixels[z].cluster]);
    }

    return res;
}
