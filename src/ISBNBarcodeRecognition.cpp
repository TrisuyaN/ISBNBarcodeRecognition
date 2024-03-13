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
 * @date		2024-3-13
 * @version		V0.1.0
 *
 **********************************************************************************
 */

#include <iostream>
#include "opencv_test.h"
#include "tester.h"

using namespace std;
using namespace cv;

// 在此处设置程序运行参数
const string test_images_path = "img\\test1\\";
const string template_images_path = "img\\templates\\";
const string preprocessed_images_save_path = "preprocessed_images_save\\";


int main() {
#ifdef OPENCVTEST
	opencv_test();
#endif
	Tester tester(test_images_path, template_images_path);
	tester.test(SAVE_PREPROCESSED_IMAGES, preprocessed_images_save_path);
	tester.calc_accuracy();
}
