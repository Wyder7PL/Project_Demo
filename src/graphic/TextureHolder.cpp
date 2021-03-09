#include "TextureHolder.hpp"

Demo::TextureHolder& Demo::TextureHolder::GetInstance()
{
	static TextureHolder result;
	return result;
}

void Demo::TextureHolder::LoadDefault()
{
	namespace fs = std::filesystem;
	fs::path p = fs::current_path();
	p /= "resources";
	p /= "graphic";
	for (const auto& entry : fs::directory_iterator(p))
	{
		if(fs::is_regular_file(entry.status()))
		{
			std::string extension = entry.path().extension();
			if(extension == ".png")
			{
				std::string relativePath = entry.path().relative_path();
				std::string filepath = "resources/graphic/";
				filepath += entry.path().filename();
				std::string name = entry.path().stem();
				if(textures.count(name) > 0)
				{
					// TODO error/waring texture alredy exist
					continue;
				}
				sf::Texture txt;
				txt.loadFromFile(filepath);
				txt.setSmooth(true);
				textures[name] = std::move(txt);
			}
		}
	}
}

void Demo::TextureHolder::SetSpriteTexture(Demo::Sprite& sprite, const std::string& textureName)
{
	
	if(textures.count(textureName) > 0)
	{
		sprite.sprite.setTexture(textures[textureName],true);
		sprite.UpdateSprite();
	}
	// TODO else couldn't load texture 
}
