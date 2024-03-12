/**@file		recognizer.cpp
* @brief		ͼƬʶ������ͷ�ļ�
* @details		ͼƬʶ������ͷ�ļ�
* @author		al_1suyan
* @date			2024-3-12
* @version		V0.1.0
*
**********************************************************************************
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

/**@struct info
* @brief CharMatchResult \n
* �洢ģ��������charMatch������ʶ����
*/
struct CharMatchResult {
	int template_index;	///< ʹ�õ�ģ������
	double diff;		///< charMatch������ʶ����
};

/**@struct info
* @brief RecognizeResult \n
* �洢recognize����ʶ����������ʶ����ַ�����������������Ϣ
*/
struct RecognizeResult {
	std::string res;	///< recognize����ʶ�����ַ���
};

/**
* @brief Recognizer \n
* ͼƬʶ������
*/
class Recognizer {
private:
	std::vector<cv::Mat> image_set;					///< ��ʶ���ͼƬ����
	std::string template_path;						///< ģ��ͼƬ·��

	double charMatch(cv::Mat, cv::Mat);				///< ���γ���ƥ��ģ�������ͼƬ����
	char charRecognizer(cv::Mat, int);				///< ƥ�䵥���ַ�ͼƬ����

public:
	Recognizer(std::vector<cv::Mat>&, std::string);	
	RecognizeResult recognize();					///< �ַ�ͼ��ʶ����
};