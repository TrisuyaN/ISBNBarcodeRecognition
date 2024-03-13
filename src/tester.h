/**@file		tester.h
* @brief		批量测试类头文件
* @details		批量测试类头文件
* @author		al_1suyan
* @date			2024-3-13
* @version		V0.1.0
*
**********************************************************************************
*/
#pragma once
#include <iostream>
#include <string>
#include "recognizer.h"
#include "preprocessor.h"
#include "opencv2\opencv.hpp"

#define FILE_READ_FAILURE -1

#define SAVE_PREPROCESSED_IMAGES 1
#define NOSAVE_PREPROCESSED_IMAGES 0

class Tester {
private:
	std::string test_images_path;
	std::string template_images_path;
	std::vector<cv::String> image_files;
	std::vector<cv::Mat> images;
	int	ISBN_test_total;
	double ISBN_accuracy;
	double char_accuracy;

public:
	Tester(std::string, std::string);
	void test(bool, std::string preprocessed_images_savepath);
};