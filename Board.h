#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using namespace std;

struct Board{
	int windowWidth;
	int windowHeight;
	int mineCount;
	int tileCount;


	Board() {

	}
	Board(int width, int height, int mCount) {
		windowWidth = width * 32;
		windowHeight = (height * 32) + 88;
		mineCount = mCount;
		tileCount = width * height;
	}

};

