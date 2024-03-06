#include "opencv_test.h"


// change opencv test items here
#ifdef OPENCVTEST

//#define OPENCV_CAMERA_TEST
//#define OPENCV_IMAGE_TEST
#define OPENCV_GLOB_TEST

#endif

const std::string GLOW_TEST_PATH = ".\\img\\glow_test\\";

using namespace std;
using namespace cv;


void opencv_test() {

#ifdef OPENCV_CAMERA_TEST
	opencv_camera_test();
#endif

#ifdef OPENCV_IMAGE_TEST
	opencv_img_test();
#endif

#ifdef OPENCV_GLOB_TEST
	vector<Mat> images =  opencv_glob_readfile_test(GLOW_TEST_PATH);
	opencv_glob_show_test(move(images));
#endif

}

#ifdef OPENCV_CAMERA_TEST
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
#endif

#ifdef OPENCV_IMAGE_TEST
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

#ifdef OPENCV_GLOB_TEST
vector<Mat> opencv_glob_readfile_test(std::string path) {
	string pattern_jpg = path + "*.jpg";
	vector<cv::String> image_files;
	vector<Mat> images;

	glob(pattern_jpg, image_files);
	if (image_files.size() == 0) {
		cout << "No image in " + path << endl;
		exit(0);
	}

	for (auto image_file : image_files) {
		cout << "Reading " << image_file << endl;
		Mat image = imread(image_file, IMREAD_COLOR);

		if (image.empty()) {
			cerr << "Could not open or find " << image_file << endl;
			exit(-1);
		}
		
		images.push_back(image);
	}

	//cout << &images << endl;
	return images;
}

int opencv_glob_show_test(vector<Mat>&& images) {
	//cout << &images << endl;
	for (auto image : images) {
		imshow("Show", image);
		waitKey(0);
	}
	return 0;
}
#endif