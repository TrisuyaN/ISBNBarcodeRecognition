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

std::string Tester::digitFilter(std::string string_input) {
	std::string res = "";
	for (auto c : string_input) {
		if (isdigit(c) || c == 'X') {
			res.append(1, c);
		}
	}
	return res;
}

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

void Tester::getISBNAnswers() {
	for (auto imgf : image_files) {
		isbn_answers.push_back(digitFilter(imgf.substr(imgf.rfind('\\') + 1)));
	}
}

Tester::Tester(std::string test_images_path_input, std::string template_images_path_input) {
	test_images_path = test_images_path_input;
	template_images_path = template_images_path_input;

	readImages();
	getISBNAnswers();

	ISBN_test_total = images.size();
	ISBN_test_accurate = 0;
	ISBN_accuracy = 0;
	char_total = 0;
	char_accurate = 0;
	char_accuracy = 0;
}

void Tester::test(bool save_preprocessed_images, std::string preprocessed_images_savepath = "preprocessed_images_save\\") {
	
	for (int i = 0; i < images.size(); i++) {
		// 预处理
		Preprocessor p(images[i]);
		p.preprocess();
		if (save_preprocessed_images) {
			std::string fname = image_files[i].substr(image_files[i].rfind('\\')+1);
			p.dbgSave(fname, test_images_path + preprocessed_images_savepath);
		}

		// 识别
		std::vector<cv::Mat> preprocessed_image_set = p.getPreprocessResult();
		Recognizer r(preprocessed_image_set, template_images_path);
		RecognizeResult res = r.recognize();
		// 保存识别结果
		isbn_recognize_results.push_back(digitFilter(res.res));
		// 显示总进度
		showProgressBar(i + 1, images.size());
	}
}

void Tester::saveArgs(std::string save_path) {
	std::fstream res_file;
	res_file.open(save_path, std::ios::out | std::ios::app);

	res_file
		<< "=========================================================" << std::endl
		<< "args: " << std::endl
		<< "=========================================================" << std::endl
		<< "ROI_Y_VALID_P_THRESHOLD: " << ROI_Y_VALID_P_THRESHOLD << std::endl
		<< "ROI_Y_VALID_NWP_THRESHOLD: " << ROI_Y_VALID_NWP_THRESHOLD << std::endl
		<< "ROI_Y_RANGE_MIN_THERSHOLD: " << ROI_Y_RANGE_MIN_THERSHOLD << std::endl
		<< "ROI_Y_RANGE_MAX_THERSHOLD: " << ROI_Y_RANGE_MAX_THERSHOLD << std::endl
		<< "ROI_Y_CONTIUOUS_NWPD_THRESHOLD: " << ROI_Y_CONTIUOUS_NWPD_THRESHOLD << std::endl
		<< "ROI_X_VALID_P_THRESHOLD: " << ROI_X_VALID_P_THRESHOLD << std::endl
		<< "N_TEMPLATE: " << N_TEMPLATE << std::endl
		<< "N_TEMPLATE_IMAGES: " << N_TEMPLATE_IMAGES << std::endl
		<< "S_INPUT_CHAR_IMAGE_THRESHOLD: " << S_INPUT_CHAR_IMAGE_THRESHOLD << std::endl
		<< "RC_RATIO_INPUT_CHAR_IMAGE_THRESHOLD: " << RC_RATIO_INPUT_CHAR_IMAGE_THRESHOLD << std::endl
		<< "BUFF: " << BUFF << std::endl
		<< "DEBUFF: " << DEBUFF << std::endl
		<< "=========================================================" << std::endl << std::endl;
}

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

	ISBN_accuracy = (double)ISBN_test_accurate / (double)ISBN_test_total;
	char_accuracy = (double)char_accurate / (double)char_total;
	std::cout 
		<< std::endl
		<< "\033[2J\033[H"
		<< std::fixed << std::setprecision(2)
		<< "ISBN准确率：" << ISBN_accuracy * 100 << "%" << std::endl
		<< "数字字符准确率：" << char_accuracy * 100 << "%" << std::endl << std::endl
		<< "按任意键结束...";
	saveResult(save_path);
	getchar();
}
