// �������е���ֵ����

#pragma once

/// OpenCV���Բ���
//#define OPENCVTEST				///< OpenCV���ԣ�ע�ʹ˺������ò���
#ifdef OPENCVTEST
#define OPENCV_CAMERA_TEST			///< ����1��������ͷ
#define OPENCV_IMAGE_TEST			///< ����2���� .\img\test.jpg ������Ϊ test.bmp�����ɾ��bmp�ļ�
#define OPENCV_GLOB_TEST			///< ����2����ȡ .\img\glod\*.jpg ��ת��Ϊ cv::Mat��������δ�
#endif


/// ͼ��Ԥ�����ȡ���ϲ��ָ߶ȱ���
#define UPPER_EXTRACT_RATIO				0.45


/// floodFill���
#define FLOOD_FILL_DEPTH				5			///< ����ˮ����ʼ�ı�Ե���


/// ROI��ȡ��ֵ����
#define ROI_Y_VALID_P_THRESHOLD			100			///< ��ȡy����ROI����Ȥ��ʱ�����ص�Ҷ�ֵ��ֵ
#define ROI_Y_VALID_NWP_THRESHOLD		35			///< ��ȡy����ROI����Ȥ��ʱ���а�ɫ��������n_white_pixels����ֵ
#define ROI_Y_RANGE_MIN_THERSHOLD		40			///< y����ROI����С������ֵ
#define ROI_Y_RANGE_MAX_THERSHOLD		400			///< y����ROI����󳤶���ֵ
#define ROI_Y_CONTIUOUS_NWPD_THRESHOLD	300			///< �ж�y����ROI��Ϊ��������ʱ���а�����ֵ������ֵ��n_white_pixels_difference����ֵ
#define ROI_X_VALID_P_THRESHOLD			100			///< ��ȡx����ROI����Ȥ��ʱ�����ص�Ҷ�ֵ��ֵ


/// �ַ�ģ�����
#define N_TEMPLATE						4			///< ģ��ͼƬ���������˲�����ģ��ͼƬ��������õ���Ӧ�ַ�
#define N_TEMPLATE_IMAGES				15			///< ÿ��ģ���ַ�ͼƬ���������˲�����ģ��ͼƬ��������õ���Ӧ�ַ�


/// �ַ�ͼƬɸѡ��ֵ����
#define S_INPUT_CHAR_IMAGE_THRESHOLD	450			///< �ַ�ͼƬ��С��ֵ��С�ڴ���ֵ��ܾ�ʶ�𷵻ؿո��ַ�
#define RC_RATIO_INPUT_CHAR_IMAGE_THRESHOLD 0.9		///< �ַ�ͼƬ���б�ֵ����ֵ��С�ڴ���ֵ��ܾ�ʶ�𷵻ؿո��ַ�


/// ͼƬģ����Сdiff��������̵���ֵ
#define MAX_DIFF_THRESHOLD				70			///< ͼƬģ����Сdiff��������̵���ֵ����ƥ������ģ������Сdiff����������ֵ���ж�Ϊ�����ROI_x�����ؿո�


/// ���������ַ���Ȩ
#define BUFF		1.2								///< �������ַ���diff���Դ�ֵ���Ӷ���Сdiff������ѡ����ַ������򣬴Ӷ�ʵ�ּ�Ȩ
#define DEBUFF		1.0								///< ͬ������diff�Ӷ���С��������X��7��8������֮������в����飬��Ϊ�ᵼ��7��8Ȩ�������С������ʶ��Ϊ�������֣���˽�����ʹ��BUFF�����Ȩ