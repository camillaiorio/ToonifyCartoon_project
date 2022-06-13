#include "image.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#define IMAGE_PIXEL_LIMIT   700000

using namespace std;

Image prima_versione(Image& im){
    //edges
    pair<Image,Image> grad = compute_gradient(im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    Image nms = non_maximum_supp(mag, dir);
    float strong = 1.0;
    float weak = 0.2;
    Image dt = double_thresholding(nms, 0.03, 0.17, strong, weak);
    Image edge_track = edge_tracking(dt, weak, strong); //edge_track è  il bordo dell'immagine

    //colors
    Image im_sz = resizing(im);
    Image bf;
    for(int i=0; i<14; i++){
        bf = bilateral_filter(im_sz, 3, 0.1);
    }
    Image im_bigger = bilinear_resize(bf, bf.w *4 , bf.h *4);
    Image mfc= median_filter_color(im_bigger,1);
    Image qc_im = quantize_colors(mfc,24); //immagine quantizzata a colori

    //recombine
    Image merge = recombine(edge_track, qc_im);
    return merge;
}

Image seconda_versione(Image& im){

    //creo un'immagine quantizzata
    Image im_sz = resizing(im);
    Image bf;
    for(int i=0; i<14; i++){
        bf = bilateral_filter(im_sz, 3, 0.1);
    }
    Image im_bigger = bilinear_resize(bf, bf.w *4 , bf.h *4);
    Image mfc= median_filter_color(im_bigger,1);

    Image qc_im = quantize_colors(mfc,24);

    // edge
    pair<Image,Image> grad = compute_gradient(qc_im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    Image nms = non_maximum_supp(mag, dir);
    float strong = 1.0;
    float weak = 0.2;
    Image dt = double_thresholding(nms, 0.03, 0.17, strong, weak);
    Image edge_track = edge_tracking(dt, weak, strong);

    Image dlt = dilation(edge_track); //bordi spessi

    //color
    Image image_sz = resizing(im);
    Image bf_2;
    for(int i=0; i<14; i++){
        bf_2 = bilateral_filter(image_sz, 3, 0.1);
    }
    Image im_bigger_2 = bilinear_resize(bf_2, bf_2.w *4 , bf_2.h *4);
    Image mfc_2= median_filter_color(im_bigger_2,1);
    Image qc_im_2 = quantize_colors(mfc_2,24); //immagine quantizzata a colori

    //recombine
    Image merge_dil = recombine(dlt, qc_im_2);
    return merge_dil;
}


int main() {
    cout << "Programma di cartoonificazione immagini \n" ;
    cout << "di Camilla Iorio\nV 1.0\n" ;
    cout << "Inserire URL immagine da elaborare:" ;
    time_t t0,t1;
    char url[200];

    t0 = time(NULL);
    cin >> url;
    Image start_image = load_image(url);
    char *dot = strrchr( url, '.');
    if(dot != NULL && dot != url)
        *dot = '\0';


    int h=start_image.h;
    int w=start_image.w;
    cout << "L'immagine ha dimensione (h x w) " << h << " x "<< w << "\n";

    Image final_image;

    if(h*w>= IMAGE_PIXEL_LIMIT){
        cout << "L'immagine è superiore a " << IMAGE_PIXEL_LIMIT << " pixel\n";

        cout << "Elaborazione in corso. Attendere...\n ";
        final_image = seconda_versione(start_image);

    }else{
        cout << "L'immagine è inferiore a " << IMAGE_PIXEL_LIMIT << " pixel\n";

        cout << "Elaborazione in corso. Attendere...\n ";
        final_image = prima_versione(start_image);
    }


    char url_out[200];
    strcpy(url_out, url);
    strcat(url_out, "_cartoonized");
    final_image.save_png(url_out);
    cout << "l'immagine elaborata è stata salvata come " << url_out;
    t1 = time(NULL);
    cout << "Tempo impiegato: "<<(t1-t0) <<" secondi.\n";

    return 0;
}

