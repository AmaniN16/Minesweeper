#pragma once
#include<string>
#include<unordered_map>
#include<SFML/Graphics.hpp>
using namespace std;
class TextureManager
{

	static unordered_map<string, sf::Texture> textures;
	static void LoadTexture(string texture);
public:
	static sf::Texture& GetTexture(string textureName);
	static void Clear();
};

