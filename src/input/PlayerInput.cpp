#include "PlayerInput.hpp"

Demo::PlayerInput::PlayerInput()
{
}

Demo::PlayerInput::~PlayerInput()
{
}

void Demo::PlayerInput::UpdateInput(Window& window)
{
	sf::Event event;
	while(window.window.pollEvent(event))
	{
		if(event.type == sf::Event::MouseMoved)
		{
			mousepos.x = event.mouseMove.x;
			mousepos.y = event.mouseMove.y;
		}
		if(event.type == sf::Event::MouseButtonPressed)
		{
			if(event.mouseButton.button == sf::Mouse::Left) mouseleft = true;
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if(event.mouseButton.button == sf::Mouse::Left) mouseleft = false;
		}
		
		if(event.type == sf::Event::Closed) exit = true;
		if(event.key.code == sf::Keyboard::Escape)
		{
			if(event.type == sf::Event::KeyPressed) escape = true;
			else if (event.type == sf::Event::KeyReleased) escape = false;
		}
		if(event.key.code == sf::Keyboard::S)
		{
			if(event.type == sf::Event::KeyPressed) down = true;
			else if (event.type == sf::Event::KeyReleased) down = false;
		}
	}
}
