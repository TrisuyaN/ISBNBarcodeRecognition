/*
	Recognizer��ʶ��������һ���ַ�ͼƬ����ʶ��
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
	std::vector<cv::Mat> image_set;		// ��ʶ���ͼƬ����
	std::string template_path;			// ģ��ͼƬ·��

	double charMatch(cv::Mat, cv::Mat);
	char charRecognizer(cv::Mat, int);

public:
	Recognizer(std::vector<cv::Mat>&, std::string);
	RecognizeResult recognize();
};