/**@file		recognizer.cpp
* @brief		图片识别器类头文件
* @details		图片识别器类头文件
* @author		al_1suyan
* @date			2024-3-12
* @version		V0.1.0
*
**********************************************************************************
*/

#pragma once

#define N_TEMPLATE 4			///< 模板图片套数，依此参数和模板图片索引计算得到对应字符
#define N_TEMPLATE_IMAGES 14	///< 每套模板字符图片数量，依此参数和模板图片索引计算得到对应字符

#include <opencv2\opencv.hpp>
#include <utility>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>

/**@struct info
* @brief CharMatchResult \n
* 存储模板索引和charMatch函数的识别结果
*/
struct CharMatchResult {
	int template_index;	///< 使用的模板索引
	double diff;		///< charMatch函数的识别结果
};

/**@struct info
* @brief RecognizeResult \n
* 存储recognize最终识别结果，除了识别的字符串还附带了其他信息
*/
struct RecognizeResult {
	std::string res;	///< recognize最终识别结果字符串
};

/**
* @brief Recognizer \n
* 图片识别器类
*/
class Recognizer {
private:
	std::vector<cv::Mat> image_set;					///< 待识别的图片集合
	std::string template_path;						///< 模板图片路径

	double charMatch(cv::Mat, cv::Mat);				///< 单次尝试匹配模板和输入图片函数
	char charRecognizer(cv::Mat, int);				///< 匹配单个字符图片函数

public:
	Recognizer(std::vector<cv::Mat>&, std::string);	
	RecognizeResult recognize();					///< 字符图像集识别函数
};