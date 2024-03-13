#pragma once

#include <iostream>

void showProgressBar(int progress, int total) {
	std::cout << "\033[2J\033[H";
	int cur = ((double)progress / (double)total) * 50;

	std::cout << "½ø¶È: [";
	for (int k = 0; k < cur; ++k)
		std::cout << "#";
	for (int l = cur; l < 50; ++l)
		std::cout << " ";
	std::cout << "]";
	std::cout << " (" << progress << "/" << total << ")" << std::endl;
}