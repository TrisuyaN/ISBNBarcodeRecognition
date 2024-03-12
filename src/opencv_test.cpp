/**@file		opencv_test.cpp
* @brief		OpenCV����
* @details		OpenCV����Դ�ļ�������Ŀ�������ڲ���Visual Studio��OpenCV���������Ƿ���ȷ���ṩ��ʾͼƬ��������ȡ������ͼƬ�Ĳ���
* @author		al_1suyan
* @date			2024-3-12
* @version		V0.1.0
*
**********************************************************************************
*/

#include "opencv_test.h"

#ifdef OPENCVTEST

using namespace std;
using namespace cv;

const string GLOW_TEST_PATH = ".\\img\\glow_test\\";



/**@brief OpenCV���Ժ����������˶���Ӳ��� 
* @return void
*/
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

/**@brief OpenCV����ͷ���Ժ���
* @return ����ִ�н��
*/
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

/**@brief OpenCVͼƬ��ʾ���Ժ���
* @return ����ִ�н��
*/
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
	imshow("���", image);

	// save test.jpg as test.bmp
	imwrite("img\\test.bmp", image);

	// wait for input and then delete test.bmp
	waitKey(0);
	remove("img\\test.bmp");
	return 0;
}
#endif

#ifdef OPENCV_GLOB_TEST

/**@brief OpenCV������ȡͼƬ����
* @return ������ȡ�õ���OpenCV Mat����
*/
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

/**@brief OpenCV����vector��ʾͼƬ���Ժ���
* @param[in] images ͼƬ����
* @return ����ִ�н��
*/
int opencv_glob_show_test(vector<Mat>&& images) {
	//cout << &images << endl;
	for (auto image : images) {
		imshow("Show", image);
		waitKey(0);
	}
	return 0;
}
#endif

#endif