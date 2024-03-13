/**@file		opencv_test.h
* @brief		OpenCV≤‚ ‘
* @details		OpenCV≤‚ ‘Õ∑Œƒº˛
* @author		al_1suyan
* @date			2024-3-12
* @version		V0.1.0
*
**********************************************************************************
*/

#pragma once

#include "args.h"

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