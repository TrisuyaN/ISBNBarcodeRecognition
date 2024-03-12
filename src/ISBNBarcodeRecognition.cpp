#include <iostream>
#include "opencv_test.h"
#include "preprocessor.h"
#include "recognizer.h"

using namespace std;
using namespace cv;

void test();

int main() {
#ifdef OPENCVTEST
	opencv_test();
#endif
	test();
}

void test() {
	Mat image1 = imread("img\\test.jpg", IMREAD_COLOR);
	Preprocessor p(image1);
	p.preprocess();
	p.dbg_save("img\\test\\");
	vector<Mat> vm = p.get_preprocess_result();

	Recognizer r(vm, "img\\templates\\");
	r.recognize();
}