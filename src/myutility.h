#pragma once

#include <iostream>

/**@brief 显示进度条
 * @param progress 当前进度
 * @param total 总进度
 */
void showProgressBar(int progress, int total) {
	std::cout << "\033[2J\033[H";
	int cur = ((double)progress / (double)total) * 50;

	std::cout << "进度: [";
	for (int k = 0; k < cur; ++k)
		std::cout << "#";
	for (int l = cur; l < 50; ++l)
		std::cout << " ";
	std::cout << "]";
	std::cout << " (" << progress << "/" << total << ")" << std::endl;
}