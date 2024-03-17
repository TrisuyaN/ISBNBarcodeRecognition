/**@file		preprocessor.cpp
 * @brief		ͼƬԤ��������Դ�ļ�
 * @details		ͼƬԤ�������࣬������ISBN��ǩͼƬ���ָ�Ϊһ���ַ�ͼ��Recognizer��ʶ����ͬʱ�ṩ�����ɸ��õ�ʵ�ú�������֧�ֱ���ÿһ��������ͼ��
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

/**@brief ��ȡͼ���ϰ벿��
 * @param[in] input_image ԴͼƬ
 * @return ������ͼ��
 */
cv::Mat Preprocessor::extractUpperHalf(cv::Mat input_image) {
	// �������ͼ���Ƿ�Ϊ��
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

/**@brief �õ��ȱ���ת��Ϊָ����ȵ�ͼ��
 * @param[in] input_image ԴͼƬ
 * @param[in] width ָ�����
 * @return ������ͼ��
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

/**@brief ��ûҶȻ�ͼ��
 * @param[in] input_image ԴͼƬ����
 * @return ������ͼ��
 */
cv::Mat Preprocessor::gray(cv::Mat input_image) {
	cv::Mat res_image;
	cvtColor(input_image, res_image, cv::COLOR_RGB2GRAY);
	return res_image;
}

/**@brief ��õ����ǶȺ����ֱͼ��
 * @param[in] input_image ԴͼƬ����
 * @return ������ͼ��
 */
cv::Mat Preprocessor::rectify(cv::Mat input_image) {
	cv::Mat res_image;
	cv::Mat pic_edge;

	// ʹ��Sobel���Ӷ�����ͼ����б�Ե��⣬����洢��pic_edge��
	cv::Sobel(input_image, pic_edge, -1, 0, 1, 5);
	
	// ʹ�û���任��Hough Transform���ڱ�Ե�����ͼ���м������ֱ��
	vector<cv::Vec2f> Line;
	HoughLines(pic_edge, Line, 1, CV_PI / 180, 180, 0, 0);
	
	double Angle = 0;
	int LineCnt = 0;

	// ����Line�м�⵽������ֱ�ߣ��������ǵĽǶȣ�������Щ�Ƕ��ۼ�
	// ����ĽǶ����Ի���Ϊ��λ�ģ����ֱ�ߵĽǶ�С��1.2�����1.8����Լ��68.2�Ⱥ�103.1��֮�䣩�����������ֱ��
	for (int i = 0; i < Line.size(); i++) {
		if (Line[i][1] < 1.2 || Line[i][1] > 1.8) continue;
		Angle += Line[i][1];
		LineCnt++;
	}

	// ��������ֱ�߽Ƕȵ�ƽ��ֵ��������ת��Ϊ��
	// ���û�м�⵽�κ�ֱ�ߣ���ô�Ƕȱ�����Ϊ90�ȣ���CV_PI / 2��
	if (LineCnt == 0) Angle = CV_PI / 2;
	else Angle /= LineCnt;
	Angle = 180 * Angle / CV_PI - 90;

	// ʹ��Angle��ͼ������ĵ㴴��һ����ת����
	cv::Mat pic_tmp = cv::getRotationMatrix2D(cv::Point(input_image.cols / 2, input_image.rows / 2), Angle, 1);
	cv::Size src_size = cv::Size(input_image.cols * 1.42, input_image.rows);
	
	// ʹ��cv::warpAffine��������ת�����ͼ�������ת
	cv::warpAffine(input_image, res_image, pic_tmp, src_size);
	//cv::warpAffine(this->src_copy_image, this->src_copy_image, pic_tmp, src_size);	// ����ֱ�ߵ����Ƶ�ԭʼͼƬ
	
	return res_image;
}



/**@brief ��ȡ�������ֵ
 * @param[in] val int��������
 * @param[in] n ���鳤��
 * @return ������ֵ
 */
int Preprocessor::sortMid(int val[], int n){
	sort(val, val + n - 1);
	return val[n/2];
}

/**@brief ����˲������ͼ��
 * @param[in] input_image ԴͼƬ����
 * @return ������ͼ��
 */
cv::Mat Preprocessor::denoise(cv::Mat input_image) {
	// ��ýṹԪ�ػ��������������͸�ʴ
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	// ���Ͳ��� ��ǿͼ���еİ�ɫ����ǰ������ʹ��ֵͼ���ɫ�����ø��Ӵ󣬺�ɫ�����ø�С
	cv::dilate(input_image, input_image, element);
	// ��ʴ���� ʹ��ɫ�����ø�С����ɫ�����ø���
	cv::erode(input_image, input_image, element);

	// ת����8λ�޷��ŵ�ͨ��ͼ��
	cv::Mat res_image = cv::Mat(input_image.rows, input_image.cols, CV_8UC1);
	int dx[] = { 0,-1,0,1,-1,1,-1,0,1 };
	int dy[] = { 0,1,1,1,0,0,-1,-1,-1 };
	int neighbor[9], mid_val;
	
	// ��ֵ�˲� ȥ��ͼ���еĽ�������
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

/**@brief ��ö�ֵ��ͼ��
 * @param[in] input_image ԴͼƬ����
 * @return ������ͼ��
 */
cv::Mat Preprocessor::threshold(cv::Mat input_image) {
	cv::Mat res_image;

	// ����Ӧ��ֵ����
	cv::adaptiveThreshold(
		input_image,
		res_image,
		255,								///< ���ֵ����ʾ�������ֵ���ڣ�С�ڣ�ȡ������ֵ���ͣ���ֵ����ô������ص�ֵ�ᱻ����Ϊ������ֵ
		cv::ADAPTIVE_THRESH_GAUSSIAN_C,		///< ����Ӧ��������ʾ��ֵ��ÿ�����ص�����ֵ�ļ�Ȩ�ͣ�Ȩ����һ����˹����
		cv::THRESH_BINARY_INV,				///< ��ֵ���ͣ��������ֵ������ֵ����ôֵ�ᱻ����Ϊ0����������Ϊ���ֵ
		159,								///< ������ֵ������Ĵ�С
		18									///< �Ӽ���õ�����ֵ�м�ȥ�ĳ�����������ֵ��С��
	);

	return res_image;
}

/**@brief ���ˮ����ȥ���ױߵ�ͼ��
 * @param[in] input_image ԴͼƬ����
 * @return ������ͼ��
 */
cv::Mat Preprocessor::floodFill(cv::Mat input_image) {
	cv::Mat res_image = input_image;
	int dx[] = { -1,0,1,-1,1,-1,0,1 };
	int dy[] = { 1,1,1,0,0,-1,-1,-1 };

	// ��ͼ���Ե�ķ������ص�������뵽������
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

	// ��������������8���ھ�
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

/**@brief �õ���ֱ�����ROI����ͼ��
 * @param[in] input_image ԴͼƬ����
 * @return ������ͼ��
 */
cv::Mat Preprocessor::getROIYImage(cv::Mat input_image) {
	
	std::vector<int> rows_white_pixels;				// �洢�а�ɫ������
	std::vector<cv::Point> histogram_points;		// ���ڱ�ʾÿһ�еİ�����ֵ������ֵ��100��������
	std::vector<RangeStructWithID> ROI_range_tmp;	// ��ʱ�洢ROI�ı�ź���ʼ
	
	// ���ϵ��±�������ͼƬ����
	for (int i = 0; i < input_image.rows; i++) {
		int while_pixel_sum = 0;					// sum ����ÿ�з������������������ص����
		uchar* cur_row_ptr = input_image.ptr(i);

		// ������ǰ�����ص�
		for (int j = 0; j < input_image.cols; j++) {
			if (*(cur_row_ptr + j) >= ROI_Y_VALID_P_THRESHOLD) while_pixel_sum++;					// ����ƫ��ɫ�����ص㣨ȡROI_Y_VALID_P_THRESHOLDΪ��ֵ���Ѷ�ֵ�����ͼ�񣬼�Ϊ���������ص㣩
		}

		rows_white_pixels.push_back(while_pixel_sum);												// �������
		histogram_points.push_back(cv::Point(while_pixel_sum, i));									// ���棿�е㣿pointsҲ��ӳ�Ĳ�������ͼƬ�ϵĵ���ǡ�ֱ��ͼ������sum����2��Ϊ������ֱ��ͼ����
		

		cv::line(histogram_dbg_image, cv::Point(ROI_Y_VALID_NWP_THRESHOLD, i), cv::Point(ROI_Y_VALID_NWP_THRESHOLD, i), cv::Scalar(255, 0, 0), 2);	// ����ֱ��ͼ��������ֵ��ͼ����
		if (i) cv::line(histogram_dbg_image, histogram_points[max(0, i - 1)], histogram_points[i], cv::Scalar(255, 0, 0), 2);						// ����ֱ��ͼ��������ֵ��ͼ����
	}
	
	int idx = -1;
	// ����ԭʼͼƬ�а�������ֵ
	for (int i = 0; i < rows_white_pixels.size() / 2; i++) {		// ֻ��ȡ���������ټ������͸���
		// ȡ��ɫ��������ROI_Y_VALID_NWP_THRESHOLDΪ��ֵ
		if (rows_white_pixels[i] >= ROI_Y_VALID_NWP_THRESHOLD) {
			RangeStructWithID item = { ++idx, {i, 0} };				// ��������id��ʼ�㣬�յ��ݼ�Ϊ0
			ROI_range_tmp.push_back(item);
			int idx = i;
			// ���������ķ�����������
			while (rows_white_pixels[idx] >= ROI_Y_VALID_NWP_THRESHOLD) {
				idx++;
			}
			ROI_range_tmp[item.id].range.end = idx;					// ���������յ�
			i = ++idx;												// ͬ��ѭ�����������ܵ�while��Ӱ�죩
		}
	}
	// ��ȡ�����ڶ�����ѡROI����ʼ���յ㣨������һ�������룬�����ڶ��������֣��Ӷ����ͼƬ������Ե��㣩
	int ROI_range_y_begin = -1, ROI_range_y_end = -1;
	if (!ROI_range_tmp.empty()) {
		ROI_range_y_begin = ROI_range_tmp[max(0, (int)ROI_range_tmp.size() - 2)].range.start;
		ROI_range_y_end = ROI_range_tmp[max(0, (int)ROI_range_tmp.size() - 2)].range.end;
	}
	
	
	// �������С�������Ҫ��һ���з֣������»�ȡROI
	if (ROI_range_y_end - ROI_range_y_begin >= ROI_Y_RANGE_MAX_THERSHOLD || ROI_range_y_end - ROI_range_y_begin <= ROI_Y_RANGE_MIN_THERSHOLD) {
		ROI_range_tmp.clear();		// ���ROI���򻺴�
		
		idx = -1;
		priority_queue<double> heap;	// ���ѣ�ά�������а�����ֵ
		for (int i = 0; i < rows_white_pixels.size() / 2; i++) {
			// ���а�ɫ���س�����ֵ���Һ�Ŀǰ���ڵ������а�����ֵ�Ĳ�С����ֵ����֤�������а��������ӽ���
			if (rows_white_pixels[i] >= ROI_Y_VALID_P_THRESHOLD && (!heap.size() || heap.top() - rows_white_pixels[i] <= ROI_Y_CONTIUOUS_NWPD_THRESHOLD)) {
				// ��������
				heap.push(rows_white_pixels[i]); 
				RangeStructWithID item = { ++idx, {i, 0} };
				ROI_range_tmp.push_back(item);
				
				int idx = i;
				// ��������������ͬ���������У�ʹ����ΪROI��һ���֣�
				while (rows_white_pixels[idx] >= ROI_Y_VALID_NWP_THRESHOLD && heap.top() - rows_white_pixels[idx] <= ROI_Y_CONTIUOUS_NWPD_THRESHOLD) {
					heap.push(rows_white_pixels[idx++]);
				}
				// ����ROI�յ�
				ROI_range_tmp[item.id].range.end = idx;
				i = ++idx;
			}
			// ����˵��ROI���������Ӧ�öϿ������heap�������������µ���
			else {
				while (heap.size()) heap.pop();
			}
		}
		
		// ���������µ�ROI
		if (!ROI_range_tmp.empty()) {
			ROI_range_y_begin = ROI_range_tmp[max(0, (int)ROI_range_tmp.size() - 2)].range.start;
			ROI_range_y_end = ROI_range_tmp[max(0, (int)ROI_range_tmp.size() - 2)].range.end;
		}
	}
	// ���û����ȡ�����¿�ʼС�ڽ�β������̫��ֱ�ӷ��ؿ�Mat
	if (ROI_range_y_begin >= ROI_range_y_end || ROI_range_y_begin > 114514) return cv::Mat();
	
	cv::Range ROI_range_y = cv::Range(ROI_range_y_begin, ROI_range_y_end);
	cv::Mat ROI_image_y = input_image(ROI_range_y, cv::Range::all());
	
	return ROI_image_y;
}


/**@brief ����ַ�ͼ���Ƿ����Ҫ��
 * @param[in] input_image ԴͼƬ
 * @return �Ƿ����Ҫ��
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

/**@brief ����ֱ����ROI�и��õ�ISBN���ַ�ͼ�񼯣�����get_ROI_y�ķ��������ԣ�����ģ�����������д������ROI_range_x
 * @param[in] input_image ԴͼƬ
 * @return �и��õ����ַ�ͼ��
 */
std::vector<cv::Mat> Preprocessor::getROIX(cv::Mat input_image) {
	std::vector<int> num_area;
	std::vector<RangeStruct> num_ranges;
	std::vector<cv::Mat> result_image_set;
	cv::Range ROI_range_x;
	
	// �����ұ�������ͼƬ���У�����ÿһ�е���Ч��������
	for (int i = 0; i < input_image.cols; i++) {
		int num = 0;
		for (int j = 0; j < input_image.rows; j++) {
			uchar* ch = input_image.ptr(j);
			if (*(ch + i) >= ROI_X_VALID_P_THRESHOLD) num++;
		}
		num_area.push_back(num);
	}
	
	// �ҳ���Ч�е���������
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

		// ��ȡ����֤�ַ�ͼƬ��Ч��
		item_image = item_image(cv::Range(head, end), cv::Range::all());
		if (charImgCheck(item_image)) {
			result_image_set.push_back(item_image);
		}
		
	}
	return result_image_set;
}



/**@brief Preprocessor�๹�캯��
 * @param[in] input_image �������ͼƬ
 */
Preprocessor::Preprocessor(cv::Mat input_image) {
	raw_image = input_image;
}

/**@brief ͼ��Ԥ����
 * @details ���ν�����ȡ�ϰ벿�֡��ҶȻ����˲����롢��ֵ���������Ƕȡ�ˮ����ȥ���ױߡ���ȡ��ֱ����ROI������ֱ����ROI�и��õ�ISBN���ַ�ͼ��
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

/**@brief �õ������ַ�ͼ��
 * @return Preprocessor�ദ��õ��������ַ�ͼ��
 */
std::vector<cv::Mat> Preprocessor::getPreprocessResult() {
	return processed_image_set;
}

/**@brief ����ÿһ������Ľ��ͼ������Debug�׶μ��ͼ����
 * @param[in] filename �ļ���
 * @param[in] save_path ����·��
 */
void Preprocessor::dbgSave(string filename, string save_path) {
	std::wstring save_path_wstr;
	// ����Ŀ¼
	save_path_wstr = std::wstring(save_path.begin(), save_path.end());
	if (GetFileAttributesW(save_path_wstr.c_str()) == INVALID_FILE_ATTRIBUTES) {
		std::cout << save_path << "Ŀ¼�����ڣ����ڴ���" << std::endl;
		bool f = CreateDirectory(save_path_wstr.c_str(), NULL);
		if (f) std::cout << "�ɹ�����Ŀ¼" + save_path << endl;
		else {
			std::cerr << "δ�ܴ���Ŀ¼" + save_path << "��Ŀ¼�Ѵ���" << endl;
			return;
		}
	}
	
	// ������Ŀ¼
	save_path = save_path + filename + "\\";
	save_path_wstr = std::wstring(save_path.begin(), save_path.end());
	LPCWSTR sw = save_path_wstr.c_str();
	bool f = CreateDirectory(sw, NULL);
	if (f) std::cout << "�ɹ�����Ŀ¼" + save_path << endl;
	else {
		std::cerr << "δ�ܴ���Ŀ¼" + save_path << endl;
		return;
	}
	
	// д���ļ�
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

	std::cout << "�ɹ�����ֲ�Ԥ����ͼ��" + save_path << endl;
}
