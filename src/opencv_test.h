#pragma once


#include <opencv2\opencv.hpp>
#include <iostream>
#include <cstdio>
#include <vector>

#define OPENCVTEST


void opencv_test();

int opencv_camera_test();
int opencv_img_test();
std::vector<cv::Mat> opencv_glob_readfile_test(std::string);
int opencv_glob_show_test(std::vector<cv::Mat>&& images);