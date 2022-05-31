#include "image.h"
#include "utils.h"

#include <string>

using namespace std;


void test_smooth_image() {

    Image im = load_image("../data/iguana.jpg");
    Image smooth = smooth_image(im, 1); //7x7
    smooth.save_png("../output/smooth_iguana");

}


void test_gradient() {

    Image im = load_image("../data/iguana.jpg");
    pair<Image,Image> grad = compute_gradient(im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    mag.save_png("../output/dx_iguana");
}


void test_non_max_supp() {

    Image im = load_image("../data/iguana.jpg");
    pair<Image,Image> grad = compute_gradient(im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    Image nms = non_maximum_supp(mag, dir);
    nms.save_png("../output/nms_iguana");
}


void test_double_thresholding() {
    Image im = load_image("../data/iguana.jpg");
    pair<Image,Image> grad = compute_gradient(im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    Image nms = non_maximum_supp(mag, dir);
    Image dt = double_thresholding(nms, 0.03, 0.17, 1.0, 0.2);
    dt.save_png("../output/double_threshold_iguana");
}

void test_edge_tracking() {
    Image im = load_image("../data/iguana.jpg");
    pair<Image,Image> grad = compute_gradient(im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    Image nms = non_maximum_supp(mag, dir);
    float strong = 1.0;
    float weak = 0.2;
    Image dt = double_thresholding(nms, 0.03, 0.17, strong, weak);
    Image edge_track = edge_tracking(dt, weak, strong);
    edge_track.save_png("../output/edge_track_iguana");
}
void test_dilation(){
    Image im = load_image("../data/iguana.jpg");
    pair<Image,Image> grad = compute_gradient(im, 1);
    Image mag = grad.first;
    Image dir = grad.second;
    Image nms = non_maximum_supp(mag, dir);
    float strong = 1.0;
    float weak = 0.2;
    Image dt = double_thresholding(nms, 0.03, 0.17, strong, weak);
    Image edge_track = edge_tracking(dt, weak, strong);
    Image dlt = dilation(edge_track);
    dlt.save_png("../output/dilated_iguana");

}

void run_tests() {

    test_smooth_image();
    test_gradient();
    test_non_max_supp();
    test_double_thresholding();
    test_edge_tracking();
    test_dilation();

    printf("%d tests, %d passed, %d failed\n", tests_total, tests_total - tests_fail, tests_fail);
}

int main(int argc, char **argv) {
    test_dilation();
    //run_tests();
    return 0;
}

