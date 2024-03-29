/**@file		tester.cpp
 * @brief		批量测试类源文件
 * @details		批量测试类源文件
 * @author		al_1suyan
 * @date		2024-3-13
 * @version		V0.1.0
 *
 **********************************************************************************
 */
#include "tester.h"
#include "myutility.h"

/**@brief 从字符串中提取数字和X
 * @param[in] string_input 输入字符串
 * @return 提取后的字符串
 */
std::string Tester::digitFilter(std::string string_input) {
	std::string res = "";
	for (auto c : string_input) {
		if (isdigit(c) || c == 'X') {
			res.append(1, c);
		}
	}
	return res;
}

/**@brief 读取测试图片
 */
void Tester::readImages() {
	std::string pattern_jpg = test_images_path + "*";
	cv::glob(pattern_jpg, image_files);

	if (image_files.size() == 0) {
		std::cerr << "找不到文件在 " + test_images_path << std::endl;
		exit(FILE_READ_FAILURE);
	}

	for (int i = 0; i < image_files.size(); i++) {
		cv::String imgf = image_files[i];
		std::cout << "正在读取 " << imgf << std::endl;
		cv::Mat image = cv::imread(imgf, cv::IMREAD_COLOR);

		if (image.empty()) {
			std::cerr << "无法打开 " << imgf << std::endl;
			exit(FILE_READ_FAILURE);
		}

		images.push_back(image);
		showProgressBar(i + 1, image_files.size());
	}
}

/**@brief 从文件名中提取ISBN答案
 */
void Tester::getISBNAnswers() {
	for (auto imgf : image_files) {
		isbn_answers.push_back(digitFilter(imgf.substr(imgf.rfind('\\') + 1)));
	}
}

/**@brief 批量测试类构造函数
 * @param[in] save_preprocessed_images 是否保存预处理后的图片
 * @param[in] preprocessed_images_savepath 保存路径
 */
Tester::Tester(std::string test_images_path_input, std::string template_images_path_input) {
	test_images_path = test_images_path_input;
	template_images_path = template_images_path_input;
	templates_scores.resize(N_TEMPLATE_IMAGES * N_TEMPLATE, 0);

	readImages();
	getISBNAnswers();

	ISBN_test_total = images.size();
	ISBN_test_accurate = 0;
	ISBN_accuracy = 0;
	char_total = 0;
	char_accurate = 0;
	char_accuracy = 0;
}

/**@brief 批量测试函数
 * @param[in] save_preprocessed_images 是否保存预处理后的图片
 * @param[in] preprocessed_images_savepath 保存路径
 */
void Tester::test(bool save_preprocessed_images, std::string preprocessed_images_savepath = "preprocessed_images_save\\") {
	
	for (int i = 0; i < images.size(); i++) {
		// 预处理
		Preprocessor p(images[i]);
		p.preprocess();
		if (save_preprocessed_images) {
			p.dbgSave(
				image_files[i].substr(image_files[i].rfind('\\') + 1), 
				test_images_path + preprocessed_images_savepath);
		}

		// 识别
		std::vector<cv::Mat> preprocessed_image_set = p.getPreprocessResult();
		Recognizer r(preprocessed_image_set, template_images_path);
		RecognizeResult res = r.recognize();

		// 检测字符过少，判定为黑色背景
		if (digitFilter(res.res).length() < 6) {
			cv::Mat rev_img;
			cv::bitwise_not(images[i], rev_img);
			Preprocessor p(rev_img);
			p.preprocess();

			if (save_preprocessed_images) {
				p.dbgSave(
					"re_" + image_files[i].substr(image_files[i].rfind('\\') + 1),
					test_images_path + preprocessed_images_savepath);
			}

			// 再次识别
			preprocessed_image_set = p.getPreprocessResult();
			Recognizer r(preprocessed_image_set, template_images_path);
			res = r.recognize();
		}

		// 过滤、保存识别结果和模板命中数
		std::string tmp = res.res;
		if(tmp.find('N')!=std::string::npos)
		tmp = tmp.substr(tmp.find('N')+1);
		tmp = digitFilter(tmp);
		if (tmp.length() > 13) tmp = tmp.substr(0, 13);

		isbn_recognize_results.push_back(tmp);
		std::vector<int> partial_ts = r.getTemplatesScores();
		for (int i = 0; i < N_TEMPLATE_IMAGES * N_TEMPLATE; i++) {
			templates_scores[i] += partial_ts[i];
		}
		// 显示总进度
		showProgressBar(i + 1, images.size());
	}
}

/**@brief 保存参数到文件
 * @param[in] save_path 保存路径
 */
void Tester::saveArgs(std::string save_path) {
	std::fstream res_file;
	res_file.open(save_path, std::ios::out | std::ios::app);

	res_file
		<< "=========================================================" << std::endl
		<< "Arguments: " << std::endl
		<< "=========================================================" << std::endl
		<< "UPPER_EXTRACT_RATIO: " << UPPER_EXTRACT_RATIO << std::endl << std::endl
		<< "FLOOD_FILL_DEPTH: " << FLOOD_FILL_DEPTH << std::endl << std::endl
		<< "ROI_Y_VALID_P_THRESHOLD: " << ROI_Y_VALID_P_THRESHOLD << std::endl
		<< "ROI_Y_VALID_NWP_THRESHOLD: " << ROI_Y_VALID_NWP_THRESHOLD << std::endl
		<< "ROI_Y_RANGE_MIN_THERSHOLD: " << ROI_Y_RANGE_MIN_THERSHOLD << std::endl
		<< "ROI_Y_RANGE_MAX_THERSHOLD: " << ROI_Y_RANGE_MAX_THERSHOLD << std::endl
		<< "ROI_Y_CONTIUOUS_NWPD_THRESHOLD: " << ROI_Y_CONTIUOUS_NWPD_THRESHOLD << std::endl
		<< "ROI_X_VALID_P_THRESHOLD: " << ROI_X_VALID_P_THRESHOLD << std::endl << std::endl
		<< "N_TEMPLATE: " << N_TEMPLATE << std::endl
		<< "N_TEMPLATE_IMAGES: " << N_TEMPLATE_IMAGES << std::endl << std::endl
		<< "S_INPUT_CHAR_IMAGE_THRESHOLD: " << S_INPUT_CHAR_IMAGE_THRESHOLD << std::endl
		<< "RC_RATIO_INPUT_CHAR_IMAGE_THRESHOLD: " << RC_RATIO_INPUT_CHAR_IMAGE_THRESHOLD << std::endl
		<< "MAX_DIFF_THRESHOLD: " << MAX_DIFF_THRESHOLD << std::endl
		<< "BUFF: " << BUFF << std::endl
		<< "DEBUFF: " << DEBUFF << std::endl
		<< "=========================================================" << std::endl << std::endl;
}

/**@brief 保存比较结果到文件
 * @param[in] save_path 保存路径
 * @param[in] cur_isbn_answer 当前ISBN答案
 * @param[in] cur_isbn_recognize_result 当前识别结果
 * @param[in] i 当前图片索引
 */
void Tester::saveCmpResToFile(std::string save_path, std::string cur_isbn_answer, std::string cur_isbn_recognize_result, int i) {
	std::fstream res_file;
	res_file.open(save_path, std::ios::out | std::ios::app);

	if (i == 0) {
		res_file
			<< "=========================================================" << std::endl
			<< "CompareResults: " << std::endl
			<< "=========================================================" << std::endl;
	}

	res_file << image_files[i] << ":\t";
	if (cur_isbn_answer == cur_isbn_recognize_result) {
		res_file << "ok";
	}
	else {
		res_file << "err";
	}
	res_file
		<< "\t[" << cur_isbn_answer << "]"
		<< "\t[" << cur_isbn_recognize_result << "]"
		<< std::endl;

	if (i == images.size() - 1) {
		res_file
			<< "=========================================================" << std::endl << std::endl;
	}
}

/**@brief 保存结果到文件
 * @param[in] save_path 保存路径
 */
void Tester::saveResult(std::string save_path) {
	std::fstream res_file;
	res_file.open(save_path, std::ios::out | std::ios::app);
	res_file
		<< "=========================================================" << std::endl
		<< "Results: " << std::endl
		<< "=========================================================" << std::endl
		<< std::fixed << std::setprecision(2)
		<< "ISBNAccuracy: " << ISBN_accuracy * 100 << "%" << std::endl
		<< "DigitalCharAccuracy: " << char_accuracy * 100 << "%" << std::endl
		<< "=========================================================" << std::endl << std::endl;
}

/**@brief 计算准确率
 * @param[in] save_path 保存路径
 */
void Tester::calcAccuracy(std::string save_path) {
	int n = images.size();
	for (int i = 0; i < n; i++) {
		std::string cur_isbn_answer = isbn_answers[i];
		std::string cur_isbn_recognize_result = isbn_recognize_results[i];

		char_total += cur_isbn_answer.length();

		if (cur_isbn_answer == cur_isbn_recognize_result) {
			ISBN_test_accurate++;
		}

		int p = 0;
		for (auto c : cur_isbn_recognize_result) {
			if (c == cur_isbn_answer[p]) {
				char_accurate++;
				p++;
			}
			if (p >= cur_isbn_answer.length()) break;
		}

		// 逐行保存文件名、答案ISBN、识别ISBN和识别结果到文件
		saveCmpResToFile(save_path, cur_isbn_answer, cur_isbn_recognize_result, i);
	}

	// 计算准确率并输出
	ISBN_accuracy = (double)ISBN_test_accurate / (double)ISBN_test_total;
	char_accuracy = (double)char_accurate / (double)char_total;
	std::cout
		<< std::endl
		<< "\033[2J\033[H"
		<< std::fixed << std::setprecision(2)
		<< "ISBN准确率：" << ISBN_accuracy * 100 << "%" << std::endl
		<< "数字字符准确率：" << char_accuracy * 100 << "%" << std::endl << std::endl;
	saveResult(save_path);
}

void Tester::saveTemplateScores(std::string save_path) {
	std::fstream res_file;
	res_file.open(save_path, std::ios::out | std::ios::app);
	res_file
		<< "=========================================================" << std::endl
		<< "TemplateScores: " << std::endl
		<< "=========================================================" << std::endl;

	for (int i = 0; i < templates_scores.size(); i++) {
		res_file
			<< "template_" << i << ": " << templates_scores[i] << std::endl;
	}

	res_file
		<< "=========================================================" << std::endl << std::endl;
}