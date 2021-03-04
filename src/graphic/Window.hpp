#pragma once

#include "Canvas.hpp"
#include "../Utility.hpp"

#include <SFML/Graphics.hpp>

namespace Demo
{

class Window
{
public:
	Window(const Demo::PointU& size, const std::string& title);
	~Window();
	
	Demo::Canvas CreateCanvas();
	
	void Resize(const Demo::PointU& newSize);
	Demo::PointU GetSize();
	
	void Clear();
	void Display();
	
	private:
	friend class PlayerInput;
	sf::RenderWindow window;
};

}

