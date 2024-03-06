#include "opencv_test.h"

#ifdef OPENCVTEST

using namespace std;
using namespace cv;

void opencv_test() {
	opencv_camera_test();
	opencv_img_test();
}

int opencv_camera_test() {
	VideoCapture capture;
	capture.open(0);

	Mat frame;
	while (true)
	{
		capture >> frame;
		imshow("Camera", frame);
		if (waitKey(30) >= 0)
			break;
	}

	return 0;
}

int opencv_img_test() {
	Mat image;

	// try to open test.jpg
	image = imread("img\\test.jpg", IMREAD_COLOR);

	// check if test.jpg is opened
	if (image.empty()) {
		cerr << "Could not open or find the image" << endl;
		return -1;
	}

	// show test.jpg
	imshow("ÄãºÃ", image);

	// save test.jpg as test.tmp
	imwrite("img\\test.bmp", image);

	// wait for input and then delete test.bmp
	waitKey(0);
	remove("img\\test.bmp");
	return 0;
}


#endif