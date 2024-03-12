#include "recognizer.h"

using namespace std;

// 单次尝试匹配模板和输入图片，返回两者的absdiff总差值
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

// 自定义匹配结果的比较
bool cmpCharMatchResult(CharMatchResult a, CharMatchResult b){
    return a.diff < b.diff;
}

// 匹配单个字符图片并返回识别出的字符
char Recognizer::charRecognizer(cv::Mat input_image, int index_in_set) {

    // 检查将匹配的图片大小是否大于阈值
    int input_size = input_image.rows * input_image.cols;
    if (input_size <= 630 && (double)input_image.rows / input_image.cols < 1) {
        return ' ';
    }

    // 读取模板
    string pattern = template_path + "*";
    vector<cv::String> templates_file_name;
    vector<cv::Mat> template_images;
    cv::glob(pattern, templates_file_name);
    int template_image_total = templates_file_name.size();

    // 检查读取模板文件是否成功
    if (templates_file_name.size() == 0) {
        cout << "No image in " + template_path << endl;
        exit(1);
    }

    // 逐个模板尝试匹配
    CharMatchResult* match_results = new CharMatchResult[template_image_total];
    for (int template_index = 0; template_index < template_image_total; template_index++) {
        cv::Mat cur_template_image = cv::imread(templates_file_name[template_index], 0);
        match_results[template_index].template_index = template_index;
        match_results[template_index].diff = charMatch(input_image, cur_template_image);
    }

    // 选择差值最小的模板
    sort(match_results, match_results + template_image_total, cmpCharMatchResult);
    int min_diff_template_index = 0;

    for (int i = 0; i < template_image_total; ++i) {
        if (index_in_set > 5) {
            while (match_results[min_diff_template_index].template_index / 3 >= 10 || match_results[min_diff_template_index].template_index == 14) {
                min_diff_template_index++;
            }
        }
        else if (index_in_set < 2) {
            while (match_results[min_diff_template_index].template_index / 3 <= 9 && match_results[min_diff_template_index].template_index != 14) {
                min_diff_template_index++;
            }
        }
    }

    int index = match_results[min_diff_template_index].template_index / 3;
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
            return 'B';
        case 11:
            return 'I';
        case 12:
            return 'N';
        case 13:
            return 'S';
        case 14:
            return 'X';
        default:
            return ' ';
    }
}




Recognizer::Recognizer(std::vector<cv::Mat>& input_image_set, string input_template_path) {
    image_set = input_image_set;
    template_path = input_template_path;
}

RecognizeResult Recognizer::recognize() {

    string res = "";
    for (int i = 0; i < image_set.size(); i++) {
        res.append(1, charRecognizer(image_set[i], i));
    }
    cout << res << endl;

    return {
        res
    };
}