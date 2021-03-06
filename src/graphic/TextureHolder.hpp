#pragma once

#include "Sprite.hpp"

#include <unordered_map>

#include <SFML/Graphics.hpp>


namespace Demo
{

// Singleton that hold textures
class TextureHolder
{
public:
	static TextureHolder& GetInstance();
	
	TextureHolder(TextureHolder const&) = delete;
	void operator=(TextureHolder const&) = delete;
	
	void LoadDefault();
	
	void SetSpriteTexture(Sprite& sprite,const std::string& textureName);
	
private:
	TextureHolder(){};
	std::unordered_map<std::string,sf::Texture> textures;
};

}

