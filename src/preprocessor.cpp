/**@file		preprocessor.cpp
 * @brief		图片预处理器类源文件
 * @details		图片预处理器类，处理单个ISBN标签图片并分割为一组字符图像供Recognizer类识别处理，同时提供了许多可复用的实用函数，并支持保存每一步处理结果图像
 * @author		al_1suyan
 * @date		2024-3-12
 * @version		V0.1.0
 *
 **********************************************************************************
 */

#include "preprocessor.h"

using namespace std;

double Preprocessor::imgPixelAverage(cv::Mat input_image) {
	double nums = 0;
	for (int i = 0; i < input_image.rows; i++) {
		for (int j = 0; j < input_image.cols; j++) {
			if (input_image.ptr(i)[j] != 0) {
				nums += input_image.ptr(i)[j];
			}
		}
	}
	return nums / (input_image.rows * input_image.cols);
}

double Preprocessor::imgWhitePixelRatio(cv::Mat input_image, int threshold) {
	double num = 0;
	for (int i = 0; i < input_image.rows; i++) {
		for (int j = 0; j < input_image.cols; j++) {
			if (input_image.ptr(i)[j] > threshold) {
				num += 1;
			}
		}
	}
	return num / (input_image.rows * input_image.cols);
}

/**@brief 提取图像上半部分
 * @param[in] input_image 源图片
 * @return 处理后的图像
 */
cv::Mat Preprocessor::extractUpperHalf(cv::Mat input_image) {
	// 检查输入图像是否为空
	if (input_image.empty()) {
		std::cerr << "Input image is empty." << std::endl;
		return cv::Mat();
	}

	int height = input_image.rows;
	int width = input_image.cols;
	cv::Rect upperHalfRegion(0, 0, width, height * UPPER_EXTRACT_RATIO);
	cv::Mat uhm = input_image(upperHalfRegion);

	return resize(uhm, 1200);
}

/**@brief 得到等比例转换为指定宽度的图像
 * @param[in] input_image 源图片
 * @param[in] width 指定宽度
 * @return 处理后的图像
 */
cv::Mat Preprocessor::resize(cv::Mat input_image, double width) {
	cv::Mat res_image;
	//double width = 1200;
	double height = width * (double)input_image.rows / input_image.cols;
	cv::resize(input_image, res_image, cv::Size(width, height));
	return res_image;
}

void Preprocessor::solveBlackBackground(cv::Mat& input_image) {
	std::cout << imgPixelAverage(input_image) << " " << imgWhitePixelRatio(input_image, 127) << std::endl;
	if (imgPixelAverage(input_image) < BLACK_BG_THRESHOLD) {
		cv::bitwise_not(input_image, input_image);
		//cv::imshow("a", input_image);
		//cv::waitKey(0);
	}
}

/**@brief 获得灰度化图像
 * @param[in] input_image 源图片引用
 * @return 处理后的图像
 */
cv::Mat Preprocessor::gray(cv::Mat input_image) {
	cv::Mat res_image;
	cvtColor(input_image, res_image, cv::COLOR_RGB2GRAY);
	return res_image;
}

/**@brief 获得调整角度后的竖直图像
 * @param[in] input_image 源图片引用
 * @return 处理后的图像
 */
cv::Mat Preprocessor::rectify(cv::Mat input_image) {
	cv::Mat res_image;
	cv::Mat pic_edge;

	// 使用Sobel算子对输入图像进行边缘检测，结果存储在pic_edge中
	cv::Sobel(input_image, pic_edge, -1, 0, 1, 5);
	
	// 使用霍夫变换（Hough Transform）在边缘检测后的图像中检测所有直线
	vector<cv::Vec2f> Line;
	HoughLines(pic_edge, Line, 1, CV_PI / 180, 180, 0, 0);
	
	double Angle = 0;
	int LineCnt = 0;

	// 遍历Line中检测到的所有直线，计算它们的角度，并将这些角度累加
	// 这里的角度是以弧度为单位的，如果直线的角度小于1.2或大于1.8（大约在68.2度和103.1度之间），则忽略这条直线
	for (int i = 0; i < Line.size(); i++) {
		if (Line[i][1] < 1.2 || Line[i][1] > 1.8) continue;
		Angle += Line[i][1];
		LineCnt++;
	}

	// 计算所有直线角度的平均值，并将其转换为度
	// 如果没有检测到任何直线，那么角度被设置为90度（即CV_PI / 2）
	if (LineCnt == 0) Angle = CV_PI / 2;
	else Angle /= LineCnt;
	Angle = 180 * Angle / CV_PI - 90;

	// 使用Angle和图像的中心点创建一个旋转矩阵
	cv::Mat pic_tmp = cv::getRotationMatrix2D(cv::Point(input_image.cols / 2, input_image.rows / 2), Angle, 1);
	cv::Size src_size = cv::Size(input_image.cols * 1.42, input_image.rows);
	
	// 使用cv::warpAffine函数和旋转矩阵对图像进行旋转
	cv::warpAffine(input_image, res_image, pic_tmp, src_size);
	//cv::warpAffine(this->src_copy_image, this->src_copy_image, pic_tmp, src_size);	// 绘制直线到复制的原始图片
	
	return res_image;
}



/**@brief 获取数组的中值
 * @param[in] val int类型数组
 * @param[in] n 数组长度
 * @return 数组中值
 */
int Preprocessor::sortMid(int val[], int n){
	sort(val, val + n - 1);
	return val[n/2];
}

/**@brief 获得滤波降噪的图像
 * @param[in] input_image 源图片引用
 * @return 处理后的图像
 */
cv::Mat Preprocessor::denoise(cv::Mat input_image) {
	// 获得结构元素滑动窗口用于膨胀腐蚀
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	// 膨胀操作 增强图像中的白色区域（前景）。使二值图像白色区域变得更加大，黑色区域变得更小
	cv::dilate(input_image, input_image, element);
	// 腐蚀操作 使白色区域变得更小，黑色区域变得更大
	cv::erode(input_image, input_image, element);

	// 转换到8位无符号单通道图像
	cv::Mat res_image = cv::Mat(input_image.rows, input_image.cols, CV_8UC1);
	int dx[] = { 0,-1,0,1,-1,1,-1,0,1 };
	int dy[] = { 0,1,1,1,0,0,-1,-1,-1 };
	int neighbor[9], mid_val;
	
	// 中值滤波 去除图像中的椒盐噪声
	for (int i = 0; i < input_image.rows; i++) {
		for (int j = 0; j < input_image.cols; j++){
			if (i == 0 || j == 0 || i == input_image.rows - 1 || j == input_image.cols - 1){
				res_image.at<uchar>(i, j) = input_image.at<uchar>(i, j);
				continue;
			}
			for (int k = 0; k < 9; k++) {
				neighbor[k] = input_image.at<uchar>(i + dx[k], j + dy[k]);
			}
			mid_val = sortMid(neighbor, 9);
			res_image.at<uchar>(i, j) = mid_val;
		}
	}
	
	return res_image;
}

/**@brief 获得二值化图像
 * @param[in] input_image 源图片引用
 * @return 处理后的图像
 */
cv::Mat Preprocessor::threshold(cv::Mat input_image) {
	cv::Mat res_image;

	// 自适应阈值处理
	cv::adaptiveThreshold(
		input_image,
		res_image,
		255,								///< 最大值，表示如果像素值大于（小于，取决于阈值类型）阈值，那么这个像素的值会被设置为这个最大值
		cv::ADAPTIVE_THRESH_GAUSSIAN_C,		///< 自适应方法，表示阈值是每个像素的邻域值的加权和，权重是一个高斯窗口
		cv::THRESH_BINARY_INV,				///< 阈值类型，如果像素值大于阈值，那么值会被设置为0，否则被设置为最大值
		159,								///< 计算阈值的邻域的大小
		18									///< 从计算得到的阈值中减去的常数，调整阈值大小。
	);

	return res_image;
}

/**@brief 获得水漫法去除白边的图像
 * @param[in] input_image 源图片引用
 * @return 处理后的图像
 */
cv::Mat Preprocessor::floodFill(cv::Mat input_image) {
	cv::Mat res_image = input_image;
	int dx[] = { -1,0,1,-1,1,-1,0,1 };
	int dy[] = { 1,1,1,0,0,-1,-1,-1 };

	// 将图像边缘的非零像素的坐标加入到队列中
	queue<Coordinate> q;
	for (int i = 0; i < res_image.cols; i++)
		for (int j = 0; j < FLOOD_FILL_DEPTH; j++)
			if (res_image.at<uchar>(j, i) != 0) q.push({ j,i });
	for (int i = 0; i < res_image.cols; i++)
		for (int j = res_image.rows - 1; j >= res_image.rows - FLOOD_FILL_DEPTH; j--)
			if (res_image.at<uchar>(j, i) != 0) q.push({ j,i });
	for (int i = 0; i < res_image.rows; i++)
		for (int j = 0; j < FLOOD_FILL_DEPTH; j++)
			if (res_image.at<uchar>(i, j) != 0) q.push({ i,j });
	for (int i = 0; i < res_image.rows; i++)
		for (int j = res_image.cols - 1; j >= res_image.cols - FLOOD_FILL_DEPTH; j--)
			if (res_image.at<uchar>(i, j) != 0) q.push({ i,j });

	// 遍历队列中像素8个邻居
	while (!q.empty()) {
		Coordinate cur_pixel_coordinate = q.front(); q.pop();
		int cur_pixel_x = cur_pixel_coordinate.x, cur_pixel_y = cur_pixel_coordinate.y;
		for (int i = 0; i < 8; i++) {
			int neighbor_pixel_x = cur_pixel_x + dx[i];
			int neighbor_pixel_y = cur_pixel_y + dy[i];
			
			if (neighbor_pixel_x < 0 || neighbor_pixel_y < 0 || neighbor_pixel_x >= res_image.rows || neighbor_pixel_y >= res_image.cols) continue;
			if (res_image.at<uchar>(neighbor_pixel_x, neighbor_pixel_y) != 0) {
				res_image.at<uchar>(neighbor_pixel_x, neighbor_pixel_y) = 0;
				q.push({ neighbor_pixel_x, neighbor_pixel_y });
			}
		}
	}

	return res_image;
}

/**@brief 得到竖直方向的ROI区域图像
 * @param[in] input_image 源图片引用
 * @return 处理后的图像
 */
cv::Mat Preprocessor::getROIYImage(cv::Mat input_image) {
	
	std::vector<int> rows_white_pixels;				// 存储行白色像素数
	std::vector<cv::Point> histogram_points;		// 用于表示每一行的白像素值大于阈值（100）的数量
	std::vector<RangeStructWithID> ROI_range_tmp;	// 暂时存储ROI的编号和起始
	
	// 从上到下遍历输入图片的行
	for (int i = 0; i < input_image.rows; i++) {
		int while_pixel_sum = 0;					// sum 计数每行符合条件（？）的像素点个数
		uchar* cur_row_ptr = input_image.ptr(i);

		// 遍历当前行像素点
		for (int j = 0; j < input_image.cols; j++) {
			if (*(cur_row_ptr + j) >= ROI_Y_VALID_P_THRESHOLD) while_pixel_sum++;					// 计数偏白色的像素点（取ROI_Y_VALID_P_THRESHOLD为阈值，已二值化后的图像，即为计数白像素点）
		}

		rows_white_pixels.push_back(while_pixel_sum);												// 保存计数
		histogram_points.push_back(cv::Point(while_pixel_sum, i));									// 保存？中点？points也许反映的不是输入图片上的点而是“直方图”，而sum除以2是为了缩减直方图长度
		

		cv::line(histogram_dbg_image, cv::Point(ROI_Y_VALID_NWP_THRESHOLD, i), cv::Point(ROI_Y_VALID_NWP_THRESHOLD, i), cv::Scalar(255, 0, 0), 2);	// 绘制直方图到矫正二值化图副本
		if (i) cv::line(histogram_dbg_image, histogram_points[max(0, i - 1)], histogram_points[i], cv::Scalar(255, 0, 0), 2);						// 绘制直方图到矫正二值化图副本
	}
	
	int idx = -1;
	// 遍历原始图片行白像素数值
	for (int i = 0; i < rows_white_pixels.size() / 2; i++) {		// 只读取半数，减少计算量和干扰
		// 取白色像素数量ROI_Y_VALID_NWP_THRESHOLD为阈值
		if (rows_white_pixels[i] >= ROI_Y_VALID_NWP_THRESHOLD) {
			RangeStructWithID item = { ++idx, {i, 0} };				// 设置区域id，始点，终点暂记为0
			ROI_range_tmp.push_back(item);
			int idx = i;
			// 跳过连续的符合条件的行
			while (rows_white_pixels[idx] >= ROI_Y_VALID_NWP_THRESHOLD) {
				idx++;
			}
			ROI_range_tmp[item.id].range.end = idx;					// 设置区域终点
			i = ++idx;												// 同步循环的索引（受到while的影响）
		}
	}
	// 获取倒数第二个候选ROI的起始和终点（倒数第一个是条码，倒数第二个是数字，从而规避图片顶部边缘噪点）
	int ROI_range_y_begin = -1, ROI_range_y_end = -1;
	if (!ROI_range_tmp.empty()) {
		ROI_range_y_begin = ROI_range_tmp[max(0, (int)ROI_range_tmp.size() - 2)].range.start;
		ROI_range_y_end = ROI_range_tmp[max(0, (int)ROI_range_tmp.size() - 2)].range.end;
	}
	
	
	// 若区域过小或过大（需要进一步切分），重新获取ROI
	if (ROI_range_y_end - ROI_range_y_begin >= ROI_Y_RANGE_MAX_THERSHOLD || ROI_range_y_end - ROI_range_y_begin <= ROI_Y_RANGE_MIN_THERSHOLD) {
		ROI_range_tmp.clear();		// 清除ROI区域缓存
		
		idx = -1;
		priority_queue<double> heap;	// 最大堆，维护最大的行白像素值
		for (int i = 0; i < rows_white_pixels.size() / 2; i++) {
			// 若行白色像素超过阈值，且和目前存在的最大的行白像素值的差小于阈值（保证区域内行白像素数接近）
			if (rows_white_pixels[i] >= ROI_Y_VALID_P_THRESHOLD && (!heap.size() || heap.top() - rows_white_pixels[i] <= ROI_Y_CONTIUOUS_NWPD_THRESHOLD)) {
				// 则加入该行
				heap.push(rows_white_pixels[i]); 
				RangeStructWithID item = { ++idx, {i, 0} };
				ROI_range_tmp.push_back(item);
				
				int idx = i;
				// 处理连续的满足同样条件的行（使其作为ROI的一部分）
				while (rows_white_pixels[idx] >= ROI_Y_VALID_NWP_THRESHOLD && heap.top() - rows_white_pixels[idx] <= ROI_Y_CONTIUOUS_NWPD_THRESHOLD) {
					heap.push(rows_white_pixels[idx++]);
				}
				// 设置ROI终点
				ROI_range_tmp[item.id].range.end = idx;
				i = ++idx;
			}
			// 否则说明ROI区域结束，应该断开。清空heap，继续处理余下的行
			else {
				while (heap.size()) heap.pop();
			}
		}
		
		// 重新设置新的ROI
		if (!ROI_range_tmp.empty()) {
			ROI_range_y_begin = ROI_range_tmp[max(0, (int)ROI_range_tmp.size() - 2)].range.start;
			ROI_range_y_end = ROI_range_tmp[max(0, (int)ROI_range_tmp.size() - 2)].range.end;
		}
	}
	// 如果没有提取到导致开始小于结尾，或者太大，直接返回空Mat
	if (ROI_range_y_begin >= ROI_range_y_end || ROI_range_y_begin > 114514) return cv::Mat();
	
	cv::Range ROI_range_y = cv::Range(ROI_range_y_begin, ROI_range_y_end);
	cv::Mat ROI_image_y = input_image(ROI_range_y, cv::Range::all());
	
	return ROI_image_y;
}


/**@brief 检查字符图像是否符合要求
 * @param[in] input_image 源图片
 * @return 是否符合要求
 */
bool Preprocessor::charImgCheck(cv::Mat input_image) {
	if (input_image.rows * input_image.cols < S_INPUT_CHAR_IMAGE_THRESHOLD) {
		return false;
	}
	else if ((double)input_image.rows / input_image.cols < RC_RATIO_INPUT_CHAR_IMAGE_THRESHOLD) {
		return false;
	}

	return true;
}

/**@brief 对竖直方向ROI切割处理得到ISBN的字符图像集，类似get_ROI_y的方法继续对（输入的）结果区域逐列处理，获得ROI_range_x
 * @param[in] input_image 源图片
 * @return 切割处理得到的字符图像集
 */
std::vector<cv::Mat> Preprocessor::getROIX(cv::Mat input_image) {
	std::vector<int> num_area;
	std::vector<RangeStruct> num_ranges;
	std::vector<cv::Mat> result_image_set;
	cv::Range ROI_range_x;
	
	// 从左到右遍历输入图片的列，计算每一列的有效像素数量
	for (int i = 0; i < input_image.cols; i++) {
		int num = 0;
		for (int j = 0; j < input_image.rows; j++) {
			uchar* ch = input_image.ptr(j);
			if (*(ch + i) >= ROI_X_VALID_P_THRESHOLD) num++;
		}
		num_area.push_back(num);
	}
	
	// 找出有效列的连续区域
	for (int i = 0; i < num_area.size(); i++) {
		if (num_area[i] >= 2) {
			RangeStruct item = { max(i - 1, 0), 0 };
			int idx = i;
			while (num_area[idx]) idx++;
			item.end = idx;
			i = idx;
			num_ranges.push_back(item);
		}
	}
	
	if (!num_ranges.empty()) {
		ROI_range_x.start = num_ranges[0].start;
		ROI_range_x.end = num_ranges[num_ranges.size() - 1].end;
	}
	
	for (int i = 0; i < num_ranges.size(); i++) {
		cv::Mat item_image = input_image(cv::Range::all(), cv::Range(num_ranges[i].start, num_ranges[i].end));
		
		int head = 0;
		for (int j = 0; j < item_image.rows; j++) {
			auto ff = item_image.ptr(j);
			int num = 0;
			for (int k = 0; k < item_image.cols; k++) {
				num += ff[k];
			}
			if (num) {
				head = j;
				break;
			}
		}
		int end = 0;
		for (int j = item_image.rows - 1; j >= 0; j--) {
			auto ff = item_image.ptr(j);
			int num = 0;
			for (int k = 0; k < item_image.cols; k++) {
				num += ff[k];
			}
			if (num) {
				end = j;
				break;
			}
		}

		// 截取并验证字符图片有效性
		item_image = item_image(cv::Range(head, end), cv::Range::all());
		if (charImgCheck(item_image)) {
			result_image_set.push_back(item_image);
		}
		
	}
	return result_image_set;
}



/**@brief Preprocessor类构造函数
 * @param[in] input_image 待处理的图片
 */
Preprocessor::Preprocessor(cv::Mat input_image) {
	raw_image = input_image;
}

/**@brief 图像预处理
 * @details 依次进行提取上半部分、灰度化、滤波降噪、二值化、调整角度、水漫法去除白边、获取竖直方向ROI、对竖直方向ROI切割处理得到ISBN的字符图像集
 */
void Preprocessor::preprocess() {
	//solveBlackBackground(raw_image);
	resized_image = extractUpperHalf(raw_image).clone();
	gray_image = gray(resized_image).clone();
	denoised_image = denoise(gray_image).clone();
	threshold_image = threshold(denoised_image).clone();
	rectified_image = rectify(threshold_image).clone();
	flood_filled_image = floodFill(rectified_image).clone();
	histogram_dbg_image = flood_filled_image.clone();
	ROI_image_y = getROIYImage(flood_filled_image).clone();
	processed_image_set = getROIX(ROI_image_y);
}

/**@brief 得到最终字符图像集
 * @return Preprocessor类处理得到的最终字符图像集
 */
std::vector<cv::Mat> Preprocessor::getPreprocessResult() {
	return processed_image_set;
}

/**@brief 保存每一步处理的结果图像，用于Debug阶段检查图像处理
 * @param[in] filename 文件名
 * @param[in] save_path 保存路径
 */
void Preprocessor::dbgSave(string filename, string save_path) {
	std::wstring save_path_wstr;
	// 创建目录
	save_path_wstr = std::wstring(save_path.begin(), save_path.end());
	if (GetFileAttributesW(save_path_wstr.c_str()) == INVALID_FILE_ATTRIBUTES) {
		std::cout << save_path << "目录不存在，正在创建" << std::endl;
		bool f = CreateDirectory(save_path_wstr.c_str(), NULL);
		if (f) std::cout << "成功创建目录" + save_path << endl;
		else {
			std::cerr << "未能创建目录" + save_path << "：目录已存在" << endl;
			return;
		}
	}
	
	// 创建子目录
	save_path = save_path + filename + "\\";
	save_path_wstr = std::wstring(save_path.begin(), save_path.end());
	LPCWSTR sw = save_path_wstr.c_str();
	bool f = CreateDirectory(sw, NULL);
	if (f) std::cout << "成功创建目录" + save_path << endl;
	else {
		std::cerr << "未能创建目录" + save_path << endl;
		return;
	}
	
	// 写入文件
	if(!resized_image.empty()) 
		imwrite(save_path + filename + "_01_resized.jpg", resized_image);
	if (!gray_image.empty()) 
		imwrite(save_path + filename + "_02_gray.jpg", gray_image);
	if (!denoised_image.empty()) 
		imwrite(save_path + filename + "_03_denoised.jpg", denoised_image);
	if (!threshold_image.empty()) 
		imwrite(save_path + filename + "_04_threshold.jpg", threshold_image);
	if (!rectified_image.empty()) 
		imwrite(save_path + filename + "_05_rectified.jpg", rectified_image);
	if (!flood_filled_image.empty()) 
		imwrite(save_path + filename + "_06_flood_filled.jpg", flood_filled_image);
	if (!histogram_dbg_image.empty())
		imwrite(save_path + filename + "_07_histogram.jpg", histogram_dbg_image);
	if (!ROI_image_y.empty()) 
		imwrite(save_path + filename + "_08_ROI_y.jpg", ROI_image_y);
	if (!processed_image_set.empty()) {
		for (int i = 0; i < processed_image_set.size(); i++) {
			if (!processed_image_set[i].empty()) imwrite(save_path + filename + "_09_spilted_" + to_string(i) + ".jpg", processed_image_set[i]);
		}
	}

	std::cout << "成功保存分步预处理图像到" + save_path << endl;
}
