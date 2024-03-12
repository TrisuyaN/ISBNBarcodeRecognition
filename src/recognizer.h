/*
	Recognizer：识别器，对一组字符图片进行识别
*/
#pragma once

#include <opencv2\opencv.hpp>
#include <utility>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>

struct CharMatchResult {
	int template_index;
	double diff;
};

struct RecognizeResult {
	std::string res;
};

class Recognizer {
private:
	std::vector<cv::Mat> image_set;		// 待识别的图片集合
	std::string template_path;			// 模板图片路径

	double charMatch(cv::Mat, cv::Mat);
	char charRecognizer(cv::Mat, int);

public:
	Recognizer(std::vector<cv::Mat>&, std::string);
	RecognizeResult recognize();
};