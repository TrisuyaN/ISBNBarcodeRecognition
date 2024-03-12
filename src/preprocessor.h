/**@file		preprocessor.h
* @brief		ͼƬԤ��������ͷ�ļ�
* @details		ͼƬԤ��������ͷ�ļ�
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
* @brief Coordinate \n
* ����
*/
struct Coordinate {
	int x;					///< x����
	int y;					///< y����
};

/**@struct info
* @brief RangeStruct \n
* cv::Range����ʼ��Ϣ
*/
struct RangeStruct {
	int start;				///< Range��ʼ
	int end;				///< Range�յ�
};

/**@struct info
* @brief RangeStructWithID \n
* һ�����ID��cv::Range����ʼ��Ϣ���������ֶ��Range
*/
struct RangeStructWithID {
	int id;					///< Range��ʼ��ϢID
	RangeStruct range;		///< Range��ʼ��Ϣ
};

/**
* @brief Preprocessor \n
* ͼƬԤ�������࣬������ISBN��ǩͼƬ���ָ�Ϊһ���ַ�ͼ��Recognizer��ʶ����ͬʱ�ṩ�����ɸ��õ�ʵ�ú�������֧�ֱ���ÿһ��������ͼ��
*/
class Preprocessor{
private:
	// �ֲ�����Ľ��
	cv::Mat raw_image;
	cv::Mat resized_image;
	cv::Mat gray_image;
	cv::Mat fitlered_image;
	cv::Mat threshold_image;
	cv::Mat rectified_image;
	cv::Mat flood_filled_image;
	cv::Mat ROI_image_y;
	std::vector<cv::Mat> processed_image_set;

	// �ֲ���ͼ������
	int sort_mid(int val[]);						///< ��ȡ�������ֵ 
	cv::Mat resize(cv::Mat&);						///< �õ��ȱ���ת��Ϊָ����ȵ�ͼ��
	cv::Mat gray(cv::Mat&);							///< ��ûҶȻ�ͼ��
	cv::Mat fitler(cv::Mat&);						///< ����˲������ͼ��
	cv::Mat threshold(cv::Mat&);					///< ��ö�ֵ��ͼ��
	cv::Mat rectify(cv::Mat&);						///< ��õ����ǶȺ����ֱͼ��
	cv::Mat flood_fill(cv::Mat&);					///< ���ˮ����ȥ���ױߵ�ͼ��
	cv::Mat get_ROI_y_image(cv::Mat&);				///< �õ���ֱ�����ROI����ͼ��
	std::vector<cv::Mat> get_ROI_x(cv::Mat&);		///< ����ֱ����ROI�и��õ�ISBN���ַ�ͼ��

public:
	// ���ⲿʹ�õĽӿ�
	Preprocessor(cv::Mat);							///< ���캯��������һ������ͼ��
	void preprocess();								///< ִ�д�������
	void dbg_save(std::string);						///< �������д������ļ�
	std::vector<cv::Mat> get_preprocess_result();	///< ��ȡ������
};