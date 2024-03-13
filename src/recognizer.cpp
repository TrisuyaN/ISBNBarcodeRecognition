/**@file		recognizer.cpp
 * @brief		图片识别器类源文件
 * @details		图片识别器类源文件
 * @author		al_1suyan
 * @date		2024-3-12
 * @version		V0.1.0
 *
 **********************************************************************************
 */

#include "recognizer.h"

using namespace std;

/**@brief 单次尝试匹配模板和输入图片，返回两者的absdiff图像的像素值总和，即为两者的总差值
 * @param[in] input_image    源图片
 * @param[in] template_image 模板图片
 * @return 图片和模板的absdiff像素总差值
 */
double Recognizer::charMatch(cv::Mat input_image, cv::Mat template_image) {
	cv::Mat diff_image/*, demo_diff_image*/;
	cv::resize(input_image, input_image, cv::Size(template_image.cols, template_image.rows));
	cv::absdiff(template_image, input_image, diff_image);
	//cv::matchTemplate(input_image, cur_template_image, demo_diff_image, cv::TM_SQDIFF_NORMED);    // 尝试OpenCV的模板匹配函数
	
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

/**@brief 自定义匹配结果CharMatchResult类的比较规则，实现STL算法或容器按diff值从小到大排序
 * @param[in] a CharMatchResult对象
 * @param[in] b CharMatchResult对象
 * @return 比较值
 */
bool cmpCharMatchResult(CharMatchResult a, CharMatchResult b){
	return a.diff < b.diff;
}

//// 检查将匹配的图片大小是否大于阈值 或 图片长宽比是否合适
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

/**@brief 根据模板和输入图片的差值大小，匹配单个字符图片并返回识别出的字符
 * @param[in] input_image    源字符图片
 * @param[in] index_in_set   源字符图片在字符图片集中的索引
 * @return 识别出的字符
 */
char Recognizer::charRecognizer(cv::Mat input_image, int index_in_set) {
	
	//// 检查将匹配的图片大小是否大于阈值 或 图片长宽比是否合适
	//if (!charImgCheck(input_image)) {
	//	return ' ';
	//}
	
	// 读取模板
	string pattern = template_path + "*";
	vector<cv::String> templates_file_name;
	vector<cv::Mat> template_images;
	cv::glob(pattern, templates_file_name);
	int template_image_total = templates_file_name.size();

	// 检查模板套数与宏参数N_TEMPLATE是否匹配，若不匹配会导致模板文件索引和字符无法对应
	if (template_image_total / 14 != N_TEMPLATE) {
		cerr << "模板文件数量与宏参数不匹配" << endl;
		exit(-1);
	}
	
	// 检查读取模板文件是否成功
	if (templates_file_name.size() == 0) {
		cout << "找不到文件，在" + template_path << endl;
		exit(-1);
	}
	
	// 逐个模板尝试匹配
	vector<CharMatchResult> match_results;
	match_results.resize(template_image_total);

	int index = 0;

	for (int template_index = 0; template_index < template_image_total; template_index++) {
		cv::Mat cur_template_image = cv::imread(templates_file_name[template_index], 0);
		match_results[template_index].template_index = template_index;
		match_results[template_index].diff = charMatch(input_image, cur_template_image);

		// 末位数字特判，为X模板匹配增加权重，为8和7减小权重
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
		// 开始区域数字特判，为B模板匹配增加权重，为8减小权重
		else if (index_in_set <= 3) {
			if (template_index % N_TEMPLATE_IMAGES == 12) {
				match_results[template_index].diff /= (double)BUFF;
			}
			else if (template_index % N_TEMPLATE_IMAGES == 8) {
				match_results[template_index].diff *= (double)DEBUFF;
			}
		}
	}

	// 选择差值最小的模板
	sort(match_results.begin(), match_results.end(), cmpCharMatchResult);
	int min_diff_template_index = 0;

	// 根据字符图片在集合中的索引分成数字和字母，从而淘汰匹配率高但类型不符的模板，**但很大程度上依赖字符集切割是否标准，是否有多余**
	for (int i = 0; i < template_image_total; ++i) {
		// 字符图片索引小于4，说明是ISBN字母，那么当对应模板是数字的时候应该放弃这个匹配（即使更符合），继续寻找，直到其符合对应模板为字母（ISBN四个字母之一）
		if (index_in_set < 4) {
			while (min_diff_template_index < match_results.size() &&
				match_results[min_diff_template_index].template_index % N_TEMPLATE_IMAGES < 10 || 
				match_results[min_diff_template_index].template_index % N_TEMPLATE_IMAGES == 14 ) {
				min_diff_template_index++;
			}
		}
		// 同理 字符图片索引大于等于4，说明是数字或X，那么当对应模板是字母的时候应该放弃这个匹配（即使更符合），继续寻找，直到其符合对应模板为数字或X
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



/**@brief Recognizer类构造函数
 * @param[in] input_image_set        源字符图片集引用
 * @param[in] input_template_path    模板图片路径
 */
Recognizer::Recognizer(std::vector<cv::Mat>& input_image_set, string input_template_path) {
	image_set = input_image_set;
	template_path = input_template_path;
}

/**@brief 字符图像集识别函数
 * @return 识别结果RecognizeResult对象
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
