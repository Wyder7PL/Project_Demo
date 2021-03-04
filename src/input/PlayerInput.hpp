#pragma once

#include "Input.hpp"
#include "../graphic/Window.hpp"

namespace Demo
{

class PlayerInput : public Input
{
public:
	PlayerInput();
	~PlayerInput();
	
	void UpdateInput(Window& window);

};

}

