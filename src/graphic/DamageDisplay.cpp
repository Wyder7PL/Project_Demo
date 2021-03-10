#include "DamageDisplay.hpp"

#include <sstream>
#include <cassert>

Demo::DamageDisplay::DamageDisplay(std::string message, bool proportionalSize, unsigned int size, double lifetime)
:text(message,size)
,lifetimeLeft(lifetime)
,initialLifetime(lifetime)
{
	assert(initialLifetime > 0);
	if(proportionalSize)
	{
		std::stringstream str(message);
		int number = 0;
		str >> number;
		if(str.good() && number != 0)
			text.SetFontSize(32+number/5);
	}
}

Demo::DamageDisplay::~DamageDisplay()
{
}
void Demo::DamageDisplay::Update(double delta)
{
	lifetimeLeft -= delta;
	
	PointU tempPos = GetPosition();
	tempPos.y = tempPos.y - text.GetFontSize() * (1 - lifetimeLeft / initialLifetime) * 2;
	text.SetPosition(tempPos);
}

void Demo::DamageDisplay::Draw(const Canvas& canvas) const
{
	text.Draw(canvas);
}

void Demo::DamageDisplay::SetPosition(const PointU& pos)
{
	GameObject::SetPosition(pos);
	text.SetPosition(GetPosition());
}

bool Demo::DamageDisplay::ToDestroy()
{
	return lifetimeLeft <= 0;
}

#include <iostream>
void Demo::DamageDisplay::TESTPrintStat()
{
	std::cout << "\n" << text.GetPosition().x << " " << lifetimeLeft << " " << initialLifetime;
}

void Demo::DamageDisplay::SetText(std::string message)
{
	text.UpdateText(std::move(message));
}
