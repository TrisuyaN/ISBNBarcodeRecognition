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

Tester::Tester(std::string test_images_path_input, std::string template_images_path_input) {
	test_images_path = test_images_path_input;
	template_images_path = template_images_path_input;
	ISBN_test_total = 0;
	ISBN_accuracy = 0;
	char_accuracy = 0;

	std::string pattern_jpg = test_images_path + "*";

	cv::glob(pattern_jpg, image_files);

	if (image_files.size() == 0) {
		std::cerr << "找不到文件在 " + test_images_path << std::endl;
		exit(FILE_READ_FAILURE);
	}

	for (auto imgf : image_files) {
		std::cout << "正在读取 " << imgf << std::endl;
		cv::Mat image = cv::imread(imgf, cv::IMREAD_COLOR);

		if (image.empty()) {
			std::cerr << "无法打开 " << imgf << std::endl;
			exit(FILE_READ_FAILURE);
		}

		images.push_back(image);
	}
}

void Tester::test(bool save_preprocessed_images, std::string preprocessed_images_savepath = "preprocessed_images_save\\") {
	for (int i = 0; i < images.size(); i++) {
		Preprocessor p(images[i]);

		p.preprocess();

		if (save_preprocessed_images) {
			std::string fname = image_files[i].substr(image_files[i].rfind('\\'));
			p.dbg_save(fname, test_images_path + preprocessed_images_savepath);
		}

		std::vector<cv::Mat> preprocessed_image_set = p.get_preprocess_result();

		Recognizer r(preprocessed_image_set, template_images_path);
		RecognizeResult res = r.recognize();
		std::cout << res.res << std::endl;
	}
}
