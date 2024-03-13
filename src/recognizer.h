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

#define N_TEMPLATE 4								///< ģ��ͼƬ���������˲�����ģ��ͼƬ��������õ���Ӧ�ַ�
#define N_TEMPLATE_IMAGES 14						///< ÿ��ģ���ַ�ͼƬ���������˲�����ģ��ͼƬ��������õ���Ӧ�ַ�
#define S_INPUT_CHAR_IMAGE_THRESHOLD 900			///< �ַ�ͼƬ��С��ֵ��С�ڴ���ֵ��ܾ�ʶ�𷵻ؿո��ַ�
#define RC_RATIO_INPUT_CHAR_IMAGE_THRESHOLD 1.2		///< �ַ�ͼƬ���б�ֵ����ֵ��С�ڴ���ֵ��ܾ�ʶ�𷵻ؿո��ַ�

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
* �洢recognize����ʶ����������ʶ����ַ��������Ը���������Ϣ
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
	bool charImgCheck(cv::Mat);
	char charRecognizer(cv::Mat, int);				///< ƥ�䵥���ַ�ͼƬ����

public:
	Recognizer(std::vector<cv::Mat>&, std::string);	
	RecognizeResult recognize();					///< �ַ�ͼ��ʶ����
};