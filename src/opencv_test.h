/**@file		opencv_test.h
* @brief		OpenCV����
* @details		OpenCV����ͷ�ļ�
* @author		al_1suyan
* @date			2024-3-12
* @version		V0.1.0
*
**********************************************************************************
*/

#pragma once

//#define OPENCVTEST				///< OpenCV���ԣ�ע�ʹ˺������ò���

#ifdef OPENCVTEST
#define OPENCV_CAMERA_TEST		///< ����1��������ͷ
#define OPENCV_IMAGE_TEST		///< ����2���� .\img\test.jpg ������Ϊ test.bmp�����ɾ��bmp�ļ�
#define OPENCV_GLOB_TEST		///< ����2����ȡ .\img\glod\*.jpg ��ת��Ϊ cv::Mat��������δ�
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