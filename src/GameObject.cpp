#include "GameObject.hpp"

Demo::GameObject::GameObject()
:position(0,0),id(higestID++)
{
	
}

Demo::GameObject::~GameObject()
{
	
}

unsigned long Demo::GameObject::higestID = 1;

unsigned long Demo::GameObject::GetID()
{
	return id;
}

void Demo::GameObject::SetPosition(const PointU& pos)
{
	position = pos;
}

Demo::PointU Demo::GameObject::GetPosition()
{
	return position;
}
