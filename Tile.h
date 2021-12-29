#pragma once
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
using namespace std;
struct Tile:public TextureManager
{
	float xPos;
	float yPos;
	bool hidden;
	bool flagged;
	sf::Sprite sprite;

	Tile(float x, float y, bool hid) {
		if (x == 0) {
			xPos = x;
		}

		else
			xPos = x * 32;

		if (y == 0) {
			yPos = y;
		}

		else
			yPos = y * 32;
		
		hidden = hid;
		flagged = false;
		sprite.setTexture(GetTexture("tile_hidden"));
		sprite.setPosition(sf::Vector2f(xPos, yPos));
	}
	

};

