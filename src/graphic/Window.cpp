#include "Window.hpp"

Demo::Window::Window(const Demo::Point<unsigned int>& size, const std::string& title = "")
:window(sf::VideoMode(size.x,size.y), title)
{
}

Demo::Window::~Window()
{
}

Demo::Canvas Demo::Window::CreateCanvas()
{
	Canvas gc(window);
	return gc;
}

void Demo::Window::Resize(const Demo::Point<unsigned int>& newSize)
{
	window.setSize(sf::Vector2u(newSize.x,newSize.y));
}

Demo::Point<unsigned int> Demo::Window::GetSize()
{
	return Point<unsigned int>(window.getSize().x,window.getSize().y);
}

void Demo::Window::Clear()
{
	window.clear();
}

void Demo::Window::Display()
{
	window.display();
}
