#pragma once

#include <iostream>

void show_progerss_bar(int progress, int total) {
	std::cout << "\033[2J\033[H";
	int cur = ((double)progress / (double)total) * 50;

	std::cout << "Progress: [";
	for (int k = 0; k < cur; ++k)
		std::cout << "#";
	for (int l = cur; l < 50; ++l)
		std::cout << " ";
	std::cout << "]";
	std::cout << " (" << progress << "/" << total << ")" << std::endl;
}