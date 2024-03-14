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
#include <fstream>
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

	std::vector<std::string> isbn_answers;
	std::vector<std::string> isbn_recognize_results;


	int	ISBN_test_total;
	int	ISBN_test_accurate;
	double ISBN_accuracy;
	int char_total;
	int char_accurate;
	double char_accuracy;
	std::vector<int> templates_scores;				///< 模板总命中率，用于调试


	std::string digitFilter(std::string);
	void saveCmpResToFile(std::string, std::string, std::string, int);
	void readImages();
	void getISBNAnswers();

public:
	Tester(std::string, std::string);
	void test(bool, std::string preprocessed_images_savepath);
	void calcAccuracy(std::string);
	void saveArgs(std::string);
	void saveResult(std::string);
	void saveTemplateScores(std::string);
};