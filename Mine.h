#pragma once
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
using namespace std;
struct Mine:public TextureManager
{
	float xPos;
	float yPos;
	bool flagged;
	sf::Sprite mine;

	Mine(float x, float y, bool out) {
		xPos = x;
		yPos = y;
		flagged = out;
		mine.setTexture(GetTexture("mine"));
		mine.setPosition(xPos, yPos);
	}

};

