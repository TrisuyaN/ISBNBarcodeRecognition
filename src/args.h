// 程序运行的数值参数

#pragma once

/// OpenCV测试参数
//#define OPENCVTEST				///< OpenCV测试，注释此宏来禁用测试
#ifdef OPENCVTEST
#define OPENCV_CAMERA_TEST		///< 测试1：打开摄像头
#define OPENCV_IMAGE_TEST		///< 测试2：打开 .\img\test.jpg 并保存为 test.bmp。最后删除bmp文件
#define OPENCV_GLOB_TEST		///< 测试2：读取 .\img\glod\*.jpg 并转换为 cv::Mat，最后依次打开
#endif


/// ROI提取阈值参数
#define ROI_Y_VALID_P_THRESHOLD			100			///< 提取y方向ROI感兴趣行时的像素点灰度值阈值
#define ROI_Y_VALID_NWP_THRESHOLD		35			///< 提取y方向ROI感兴趣行时的行白色像素数（n_white_pixels）阈值
#define ROI_Y_RANGE_MIN_THERSHOLD		40			///< y方向ROI的最小长度阈值
#define ROI_Y_RANGE_MAX_THERSHOLD		400			///< y方向ROI的最大长度阈值
#define ROI_Y_CONTIUOUS_NWPD_THRESHOLD	300			///< 判断y方向ROI行为连续区域时的行白像素值的最大差值（n_white_pixels_difference）阈值
#define ROI_X_VALID_P_THRESHOLD			100			///< 提取x方向ROI感兴趣行时的像素点灰度值阈值


/// 字符模板参数
#define N_TEMPLATE						4			///< 模板图片套数，依此参数和模板图片索引计算得到对应字符
#define N_TEMPLATE_IMAGES				15			///< 每套模板字符图片数量，依此参数和模板图片索引计算得到对应字符


/// 字符图片筛选阈值参数
#define S_INPUT_CHAR_IMAGE_THRESHOLD	500			///< 字符图片大小阈值，小于此阈值则拒绝识别返回空格字符
#define RC_RATIO_INPUT_CHAR_IMAGE_THRESHOLD 0.9		///< 字符图片行列比值的阈值，小于此阈值则拒绝识别返回空格字符


/// 区域特判字符加权
#define BUFF		1.1
#define DEBUFF		1.1