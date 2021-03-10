#include "TextureHolder.hpp"

#if defined(__linux__)
	#include <filesystem>
#elif defined(_WIN32)
	#include <windows.h>
#endif

Demo::TextureHolder& Demo::TextureHolder::GetInstance()
{
	static TextureHolder result;
	return result;
}

void Demo::TextureHolder::LoadDefault()
{
	#if defined(__linux__)
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
	
	#elif defined(_WIN32)
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = FindFirstFile("resources\\graphic\\*.png", &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE) {
			std::cout << "Could not open FirstFile in graphic directory\n";
			return;
		}
		do {
			std::string name = FindFileData.cFileName;
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;
			else if(name.length() > 4)
			{
				std::string ext = name.substr(name.length()-4,4);
				if( ext == ".png")
				{
					std::string nameNoExt = name.substr(0,name.length()-4);
					if(textures.count(name) > 0)
					{
						// TODO error/waring texture alredy exist
						continue;
					}
					sf::Texture txt;
					txt.loadFromFile("resources\\graphic\\"+name);
					txt.setSmooth(true);
					textures[nameNoExt] = std::move(txt);
				}
			}
		} while (FindNextFile(hFind, &FindFileData) != 0);
		FindClose(hFind);
	#endif
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
