/**@file		preprocessor.h
* @brief		图片预处理器类头文件
* @details		图片预处理器类头文件
* @author		al_1suyan
* @date			2024-3-12
* @version		V0.1.0
*
**********************************************************************************
*/

#pragma once

#include "args.h"

#include <opencv2\opencv.hpp>
#include <windows.h>
#include <utility>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>

/**@struct info
* @brief Coordinate \n
* 坐标
*/
struct Coordinate {
	int x;					///< x坐标
	int y;					///< y坐标
};

/**@struct info
* @brief RangeStruct \n
* cv::Range的起始信息
*/
struct RangeStruct {
	int start;				///< Range起始
	int end;				///< Range终点
};

/**@struct info
* @brief RangeStructWithID \n
* 一组带有ID的cv::Range的起始信息，用于区分多个Range
*/
struct RangeStructWithID {
	int id;					///< Range起始信息ID
	RangeStruct range;		///< Range起始信息
};

/**
* @brief Preprocessor \n
* 图片预处理器类，处理单个ISBN标签图片并分割为一组字符图像供Recognizer类识别处理，同时提供了许多可复用的实用函数，并支持保存每一步处理结果图像
*/
class Preprocessor{
private:
	// 分步处理的结果
	cv::Mat raw_image;
	cv::Mat resized_image;
	cv::Mat gray_image;
	cv::Mat fitlered_image;
	cv::Mat threshold_image;
	cv::Mat rectified_image;
	cv::Mat flood_filled_image;
	cv::Mat ROI_image_y;
	std::vector<cv::Mat> processed_image_set;

	bool charImgCheck(cv::Mat);

	// 分步的图像处理函数
	int sortMid(int val[], int);					///< 获取数组的中值 
	cv::Mat resize(cv::Mat&, double);				///< 得到等比例转换为指定宽度的图像
	cv::Mat gray(cv::Mat&);							///< 获得灰度化图像
	cv::Mat fitler(cv::Mat&);						///< 获得滤波降噪的图像
	cv::Mat threshold(cv::Mat&);					///< 获得二值化图像
	cv::Mat rectify(cv::Mat&);						///< 获得调整角度后的竖直图像
	cv::Mat floodFill(cv::Mat&);					///< 获得水漫法去除白边的图像
	cv::Mat getROIYImage(cv::Mat&);					///< 得到竖直方向的ROI区域图像
	std::vector<cv::Mat> getROIX(cv::Mat&);			///< 对竖直方向ROI切割处理得到ISBN的字符图像集

public:
	// 供外部使用的接口
	Preprocessor(cv::Mat);							///< 构造函数，接受一个输入图像
	void preprocess();								///< 执行处理流程
	void dbgSave(std::string, std::string);						///< 保存所有处理后的文件
	std::vector<cv::Mat> getPreprocessResult();	///< 获取处理结果
};