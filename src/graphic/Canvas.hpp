#pragma once

#include <SFML/Graphics.hpp>

namespace Demo
{

class Canvas
{
	friend class Window;
	explicit Canvas(sf::RenderWindow&);
public:
	~Canvas();

	
	template <typename T> // Chirdlren of the Graphic
	void DrawOnCanvas(T begin, T end)
	{
		for(auto i = begin; i != end; i++)
			(*i)->Draw(*this);
	}
	
private:
	friend class Graphic;
	sf::RenderWindow& windowReference;
	sf::View view;
};

}

