#include "image.h"
#include "utils.h"

#include <string>

using namespace std;


void test_smooth_image() {

    Image im = load_image("../data/big_dog.jpg");
    Image smooth = smooth_image(im, 1); //7x7
    smooth.save_png("../output/smooth_big_dog");

}


void test_gradient() {

    Image im = load_image("../data/big_dog.jpg");
    pair<Image,Image> grad = compute_gradient(im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    mag.save_png("../output/dx_big_dog");
}


void test_non_max_supp() {

    Image im = load_image("../data/big_dog.jpg");
    pair<Image,Image> grad = compute_gradient(im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    Image nms = non_maximum_supp(mag, dir);
    nms.save_png("../output/nms_big_dog");
}


void test_double_thresholding() {
    Image im = load_image("../data/big_dog.jpg");
    pair<Image,Image> grad = compute_gradient(im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    Image nms = non_maximum_supp(mag, dir);
    Image dt = double_thresholding(nms, 0.03, 0.17, 1.0, 0.2);
    dt.save_png("../output/double_threshold_big_dog");
}

void test_edge_tracking() {
    Image im = load_image("../data/big_dog.jpg");
    pair<Image,Image> grad = compute_gradient(im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    Image nms = non_maximum_supp(mag, dir);
    float strong = 1.0;
    float weak = 0.2;
    Image dt = double_thresholding(nms, 0.03, 0.17, strong, weak);
    Image edge_track = edge_tracking(dt, weak, strong);
    edge_track.save_png("../output/edge_track_big_dog");
}
void test_dilation(){
    Image im = load_image("../data/big_dog.jpg");
    pair<Image,Image> grad = compute_gradient(im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    Image nms = non_maximum_supp(mag, dir);
    float strong = 1.0;
    float weak = 0.2;
    Image dt = double_thresholding(nms, 0.03, 0.17, strong, weak);
    Image edge_track = edge_tracking(dt, weak, strong);
    Image dlt = dilation(edge_track);
    dlt.save_png("../output/dilated_big_dog");

}
void test_resizing_image(){
    Image im = load_image("../data/big_dog.jpg");
    Image im_sz = resizing(im);
    im_sz.save_png("../output/resize_big_dog");

}
void test_bilateral_app(){
    Image im = load_image("../data/big_dog.jpg");
    Image im_sz = resizing(im);
    Image bf;
    for(int i=0; i<14; i++){
        bf = bilateral_filter(im_sz, 3, 0.1);
    }
    bf.save_png("../output/bilateral_big_dog_2");
}
void test_median_color(){
    Image im = load_image("../data/big_dog.jpg");
    Image im_sz = resizing(im);
    Image bf;
    for(int i=0; i<14; i++){
        bf = bilateral_filter(im_sz, 3, 0.1);
    }
    Image im_bigger = bilinear_resize(bf, bf.w *4 , bf.h *4);
    im_bigger.save_png("../output/bigger_big_dog");
    Image mfc= median_filter_color(im_bigger,1);
    mfc.save_png("../output/median_color_big_dog");

}

void test_quantize_color(){

    Image im = load_image("../data/big_dog.jpg");
    Image im_sz = resizing(im);
    Image bf;
    for(int i=0; i<14; i++){
        bf = bilateral_filter(im_sz, 3, 0.1);
    }
    Image im_bigger = bilinear_resize(bf, bf.w *4 , bf.h *4);
    Image mfc= median_filter_color(im_bigger,1);

    Image qc_im = quantize_colors(mfc,24);
    qc_im.save_png("../output/quantize_big_dog");

}

void test_recombine(){
    Image im_edges = load_image("../output/edge_track_big_dog.png");
    Image im_color = load_image("../output/quantize_big_dog.png");

    Image merge = recombine(im_edges, im_color);
    merge.save_png("../output/final_big_dog");

    //tentativo con i bordi più spessi -> non va bene
    Image dil = load_image("../output/dilated_big_dog.png");
    Image merge2 = recombine(dil, im_color);
    merge2.save_png("../output/dil_final_big_dog");
}

void test_clusters(){
    Image im = load_image("../output/smooth_iguana.png");
    Image im_c = image_segmentation(im, 16);
    im_c.save_png("../output/cluster_iguana");
}

void test_prova(){
    //median filter + quantize
    //Image im= load_image("../data/iguana.jpg");
    Image im= load_image("../data/big_dog.jpg");
    Image im_sz = resizing(im);
    Image bf;
    for(int i=0; i<14; i++){
        bf = bilateral_filter(im_sz, 3, 0.1);
    }
    Image im_bigger = bilinear_resize(bf, bf.w *4 , bf.h *4);
    Image mfc= median_filter_color(im_bigger,1);

    Image qc_im = quantize_colors(mfc,24); //immagine quantizzata == quantize_iguana

    //canny edge detector
    pair<Image,Image> grad = compute_gradient(qc_im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    Image nms = non_maximum_supp(mag, dir);
    float strong = 1.0;
    float weak = 0.2;
    Image dt = double_thresholding(nms, 0.03, 0.17, strong, weak);
    Image edge_track = edge_tracking(dt, weak, strong);

    //edge_track.save_png("../output/quantize_bw_iguana"); //edges desiderato
    edge_track.save_png("../output/quantize_bw_big_dog");
    /*
    Image im_qedges = load_image("../output/quantize_bw_iguana.png");
    Image im_color = load_image("../output/quantize_iguana.png");
    Image im_det_edges = load_image("../output/edge_track_iguana.png");
    */
    Image im_qedges= load_image("../output/quantize_bw_big_dog.png");
    Image im_color = load_image("../output/quantize_big_dog.png");
    Image im_det_edges = load_image("../output/edge_track_big_dog.png");

    Image dlt = dilation(im_qedges);
    dlt.save_png("../output/dilated_big_dog_2");

    Image merge = recombine(im_qedges, im_color); //inverto gli edges

    //merge.save_png("../output/final_iguana_2");
    merge.save_png("../output/final_big_dog_2");

    Image details = recombine2(im_det_edges,merge); //inverto gli edges

    //details.save_png("../output/final_det_iguana_2");
    details.save_png("../output/final_det_big_dog_2");

    //con i bordi più spessi

    Image merge_dil = recombine(dlt, im_color);
    merge_dil.save_png("../output/final_dil_big_dog_2");
    Image details_dil = recombine2(im_det_edges,merge);
    details_dil.save_png("../output/final_dil_det_big_dog_2");

    //tentativo con i bordi più spessi -> non va bene
    /*
    Image dil = load_image("../output/dilated_big_dog.png");
    Image merge2 = recombine(dil, im_color);
    merge2.save_png("../output/dil_final_big_dog");*/


}


void run_tests_edges() {

    test_smooth_image();
    test_clusters();
    test_gradient();
    test_non_max_supp();
    test_double_thresholding();
    test_edge_tracking();
    test_dilation();

    printf("%d tests, %d passed, %d failed\n", tests_total, tests_total - tests_fail, tests_fail);
}

void run_tests_color(){
    test_resizing_image();
    test_bilateral_app();
    test_median_color();
    test_quantize_color();
}

int main(int argc, char **argv) {
    //test_clusters();

    test_prova();

    //run_tests_edges();
    //run_tests_color();
    //test_recombine();
    return 0;
}

