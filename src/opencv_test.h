#pragma once

// comment the line below to disable OpenCV tests
//#define OPENCVTEST

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