/**@file		recognizer.cpp
 * @brief		ͼƬʶ������Դ�ļ�
 * @details		ͼƬʶ������Դ�ļ�
 * @author		al_1suyan
 * @date		2024-3-12
 * @version		V0.1.0
 *
 **********************************************************************************
 */

#include "recognizer.h"

using namespace std;

/**@brief ���γ���ƥ��ģ�������ͼƬ���������ߵ�absdiffͼ�������ֵ�ܺͣ���Ϊ���ߵ��ܲ�ֵ
 * @param[in] input_image    ԴͼƬ
 * @param[in] template_image ģ��ͼƬ
 * @return ͼƬ��ģ���absdiff�����ܲ�ֵ
 */
double Recognizer::charMatch(cv::Mat input_image, cv::Mat template_image) {
	cv::Mat diff_image/*, demo_diff_image*/;
	cv::resize(input_image, input_image, cv::Size(template_image.cols, template_image.rows));
	cv::absdiff(template_image, input_image, diff_image);
	//cv::matchTemplate(input_image, cur_template_image, demo_diff_image, cv::TM_SQDIFF_NORMED);    // ����OpenCV��ģ��ƥ�亯��
	
	double nums = 0;
	int temp = 0;
	for (int i = 0; i < diff_image.rows; i++){
		for (int j = 0; j < diff_image.cols; j++){
			if (diff_image.ptr(i)[j] != 0){
				nums += diff_image.ptr(i)[j];
				temp++;
			}
		}
	}
	return nums / (diff_image.rows * diff_image.cols);
}

/**@brief �Զ���ƥ����CharMatchResult��ıȽϹ���ʵ��STL�㷨��������diffֵ��С��������
 * @param[in] a CharMatchResult����
 * @param[in] b CharMatchResult����
 * @return �Ƚ�ֵ
 */
bool cmpCharMatchResult(CharMatchResult a, CharMatchResult b){
	return a.diff < b.diff;
}

//// ��齫ƥ���ͼƬ��С�Ƿ������ֵ �� ͼƬ������Ƿ����
//bool Recognizer::charImgCheck(cv::Mat input_image) {
//	if (input_image.rows * input_image.cols < S_INPUT_CHAR_IMAGE_THRESHOLD) {
//		return false;
//	}
//	else if ((double)input_image.rows / input_image.cols < RC_RATIO_INPUT_CHAR_IMAGE_THRESHOLD) {
//		return false;
//	}
//
//	return true;
//}

/**@brief ����ģ�������ͼƬ�Ĳ�ֵ��С��ƥ�䵥���ַ�ͼƬ������ʶ������ַ�
 * @param[in] input_image    Դ�ַ�ͼƬ
 * @param[in] index_in_set   Դ�ַ�ͼƬ���ַ�ͼƬ���е�����
 * @return ʶ������ַ�
 */
char Recognizer::charRecognizer(cv::Mat input_image, int index_in_set) {
	
	//// ��齫ƥ���ͼƬ��С�Ƿ������ֵ �� ͼƬ������Ƿ����
	//if (!charImgCheck(input_image)) {
	//	return ' ';
	//}
	
	// ��ȡģ��
	string pattern = template_path + "*";
	vector<cv::String> templates_file_name;
	vector<cv::Mat> template_images;
	cv::glob(pattern, templates_file_name);
	int template_image_total = templates_file_name.size();

	// ���ģ������������N_TEMPLATE�Ƿ�ƥ�䣬����ƥ��ᵼ��ģ���ļ��������ַ��޷���Ӧ
	if (template_image_total / 14 != N_TEMPLATE) {
		cerr << "ģ���ļ�������������ƥ��" << endl;
		exit(-1);
	}
	
	// ����ȡģ���ļ��Ƿ�ɹ�
	if (templates_file_name.size() == 0) {
		cout << "�Ҳ����ļ�����" + template_path << endl;
		exit(-1);
	}
	
	// ���ģ�峢��ƥ��
	vector<CharMatchResult> match_results;
	match_results.resize(template_image_total);

	int index = 0;

	for (int template_index = 0; template_index < template_image_total; template_index++) {
		cv::Mat cur_template_image = cv::imread(templates_file_name[template_index], 0);
		match_results[template_index].template_index = template_index;
		match_results[template_index].diff = charMatch(input_image, cur_template_image);

		// ĩλ�������У�ΪXģ��ƥ������Ȩ�أ�Ϊ8��7��СȨ��
		if (index_in_set == image_set.size() - 1) {
			if (template_index % N_TEMPLATE_IMAGES == 14) {
				match_results[template_index].diff /= (double)BUFF;
			}
			else if (template_index % N_TEMPLATE_IMAGES == 8) {
				match_results[template_index].diff *= (double)DEBUFF;
			}
			else if (template_index % N_TEMPLATE_IMAGES == 7) {
				match_results[template_index].diff *= (double)DEBUFF;
			}
		}
		// ��ʼ�����������У�ΪBģ��ƥ������Ȩ�أ�Ϊ8��СȨ��
		else if (index_in_set <= 3) {
			if (template_index % N_TEMPLATE_IMAGES == 12) {
				match_results[template_index].diff /= (double)BUFF;
			}
			else if (template_index % N_TEMPLATE_IMAGES == 8) {
				match_results[template_index].diff *= (double)DEBUFF;
			}
		}
	}

	// ѡ���ֵ��С��ģ��
	sort(match_results.begin(), match_results.end(), cmpCharMatchResult);
	int min_diff_template_index = 0;

	// �����ַ�ͼƬ�ڼ����е������ֳ����ֺ���ĸ���Ӷ���̭ƥ���ʸߵ����Ͳ�����ģ�壬**���ܴ�̶��������ַ����и��Ƿ��׼���Ƿ��ж���**
	for (int i = 0; i < template_image_total; ++i) {
		// �ַ�ͼƬ����С��4��˵����ISBN��ĸ����ô����Ӧģ�������ֵ�ʱ��Ӧ�÷������ƥ�䣨��ʹ�����ϣ�������Ѱ�ң�ֱ������϶�Ӧģ��Ϊ��ĸ��ISBN�ĸ���ĸ֮һ��
		if (index_in_set < 4) {
			while (min_diff_template_index < match_results.size() &&
				match_results[min_diff_template_index].template_index % N_TEMPLATE_IMAGES < 10 || 
				match_results[min_diff_template_index].template_index % N_TEMPLATE_IMAGES == 14 ) {
				min_diff_template_index++;
			}
		}
		// ͬ�� �ַ�ͼƬ�������ڵ���4��˵�������ֻ�X����ô����Ӧģ������ĸ��ʱ��Ӧ�÷������ƥ�䣨��ʹ�����ϣ�������Ѱ�ң�ֱ������϶�Ӧģ��Ϊ���ֻ�X
		else if (index_in_set >= 4) {
			while (min_diff_template_index < match_results.size() &&
				match_results[min_diff_template_index].template_index % N_TEMPLATE_IMAGES >= 10 &&
				match_results[min_diff_template_index].template_index % N_TEMPLATE_IMAGES != 14) {
				min_diff_template_index++;
			}
		}
	}
	index = match_results[min_diff_template_index].template_index % N_TEMPLATE_IMAGES;
	
	switch (index) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		return index + '0';
	case 10:
		return 'I';
	case 11:
		return 'S';
	case 12:
		return 'B';
	case 13:
		return 'N';
	case 14:
		return 'X';
	default:
		return ' ';
	}
}



/**@brief Recognizer�๹�캯��
 * @param[in] input_image_set        Դ�ַ�ͼƬ������
 * @param[in] input_template_path    ģ��ͼƬ·��
 */
Recognizer::Recognizer(std::vector<cv::Mat>& input_image_set, string input_template_path) {
	image_set = input_image_set;
	template_path = input_template_path;
}

/**@brief �ַ�ͼ��ʶ����
 * @return ʶ����RecognizeResult����
 */
RecognizeResult Recognizer::recognize() {
	
	string res = "";
	for (int i = 0; i < image_set.size(); i++) {
		res.append(1, charRecognizer(image_set[i], i));
	}
	
	return {
		res
	};
}
