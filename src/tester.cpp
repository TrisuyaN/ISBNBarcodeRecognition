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

std::string Tester::digit_filter(std::string string_input) {
	std::string res = "";
	for (auto c : string_input) {
		if (isdigit(c)) {
			res.append(1, c);
		}
	}
	return res;
}

void Tester::read_images() {
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
		show_progerss_bar(i + 1, image_files.size());
	}
}

void Tester::get_isbn_answers() {
	for (auto imgf : image_files) {
		isbn_answers.push_back(digit_filter(imgf.substr(imgf.rfind('\\') + 1)));
	}
}

Tester::Tester(std::string test_images_path_input, std::string template_images_path_input) {
	test_images_path = test_images_path_input;
	template_images_path = template_images_path_input;

	read_images();
	get_isbn_answers();

	ISBN_test_total = images.size();
	ISBN_test_accurate = 0;
	ISBN_accuracy = 0;
	char_total = 0;
	char_accurate = 0;
	char_accuracy = 0;
}

void Tester::test(bool save_preprocessed_images, std::string preprocessed_images_savepath = "preprocessed_images_save\\") {
	
	for (int i = 0; i < images.size(); i++) {
		Preprocessor p(images[i]);
		p.preprocess();
		if (save_preprocessed_images) {
			std::string fname = image_files[i].substr(image_files[i].rfind('\\')+1);
			p.dbg_save(fname, test_images_path + preprocessed_images_savepath);
		}

		std::vector<cv::Mat> preprocessed_image_set = p.get_preprocess_result();
		Recognizer r(preprocessed_image_set, template_images_path);
		RecognizeResult res = r.recognize();

		isbn_recognize_results.push_back(digit_filter(res.res));

		show_progerss_bar(i+1, images.size());
	}
}

void Tester::calc_accuracy() {
	int n = images.size();
	for (int i = 0; i < n; i++) {
		std::string cur_isbn_answer = isbn_answers[i];
		std::string cur_isbn_recognize_result = isbn_recognize_results[i];

		std::cout << cur_isbn_answer << " " << cur_isbn_recognize_result << std::endl;
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
	}

	ISBN_accuracy = (double)ISBN_test_accurate / (double)ISBN_test_total;
	char_accuracy = (double)char_accurate / (double)char_total;
	std::cout << "\033[2J\033[H"
		<< std::fixed << std::setprecision(2)
		<< "ISBN准确率：" << ISBN_accuracy * 100 << "%" << std::endl
		<< "数字字符准确率：" << char_accuracy * 100 << "%" << std::endl << std::endl
		<< "按任意键结束...";
	getchar();
}
