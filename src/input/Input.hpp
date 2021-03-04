#pragma once

#include "../Utility.hpp"

#include <SFML/Window.hpp>

namespace Demo
{

class Input
{
public:
	Input();
	~Input();
	
	PointI MousePos();
	bool MouseLeft();
	
	bool Escape();
	bool Exit();
	
	bool Down();
	
protected:
	PointI mousepos;
	bool mouseleft;

	bool escape;
	bool exit;
	
	bool down;
	
private:
};

}

