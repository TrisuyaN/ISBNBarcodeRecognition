/**@file		opencv_test.h
* @brief		OpenCV测试
* @details		OpenCV测试头文件
* @author		al_1suyan
* @date			2024-3-12
* @version		V0.1.0
*
**********************************************************************************
*/

#pragma once

//#define OPENCVTEST				///< OpenCV测试，注释此宏来禁用测试

#ifdef OPENCVTEST
#define OPENCV_CAMERA_TEST		///< 测试1：打开摄像头
#define OPENCV_IMAGE_TEST		///< 测试2：打开 .\img\test.jpg 并保存为 test.bmp。最后删除bmp文件
#define OPENCV_GLOB_TEST		///< 测试2：读取 .\img\glod\*.jpg 并转换为 cv::Mat，最后依次打开
#endif

#ifdef OPENCVTEST
#include <opencv2\opencv.hpp>
#include <iostream>
#include <cstdio>
#include <vector>


void opencv_test();

int opencv_camera_test();
int opencv_img_test();
std::vector<cv::Mat> opencv_glob_readfile_test(std::string);
int opencv_glob_show_test(std::vector<cv::Mat>&& images);
#endif