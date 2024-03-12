/**@mainpage		ISBNBarcodeRecognition
* <table>
* <tr><th>Project	<td>ISBNBarcodeRecognition
* <tr><th>Author	<td>al_1suyan (trisuyan@gmail.com)
* <tr><th>Source	<td>https://github.com/TrisuyaN/ISBNBarcodeRecognition
* </table>
* @section			项目详细描述
* ISBNBarcodeRecognition
* 基于C++和OpenCV的ISBN条码识别程序，可以从目录下批量读取待识别的图片，使用模板匹配法进行字符匹配。
*
* @section			功能描述
*
* @section			用法描述
* 
**********************************************************************************
*/

/**@file		ISBNBarcodeRecognition.cpp
* @brief		项目主函数文件
* @details		main函数入口
* @author		al_1suyan
* @date			2024-3-12
* @version		V0.1.0
*
**********************************************************************************
*/

#include <iostream>
#include "opencv_test.h"
#include "preprocessor.h"
#include "recognizer.h"

using namespace std;
using namespace cv;

void test();

int main() {
#ifdef OPENCVTEST
	opencv_test();
#endif
	test();
}

void test() {
	Mat image1 = imread("img\\test.jpg", IMREAD_COLOR);
	Preprocessor p(image1);
	p.preprocess();
	p.dbg_save("img\\test\\");
	vector<Mat> vm = p.get_preprocess_result();

	Recognizer r(vm, "img\\templates\\");
	r.recognize();
}