#include "MenuBattle.hpp"

Demo::MenuBattle::MenuBattle()
:controllerIsSet(false)
{
	
}

Demo::MenuBattle::~MenuBattle()
{
}

void Demo::MenuBattle::Update(double delta)
{
	Battle::Update(delta);
	if(controllerIsSet)
		menuCon->Update(delta);
}

void Demo::MenuBattle::CreatePlayerController(std::weak_ptr<Input> ptr)
{
	menuCon = std::make_unique<MenuController>(&data,ptr);
	controllerIsSet = true;
}

Demo::BattleChoice Demo::MenuBattle::Choice()
{
	if(controllerIsSet)
	{
		return menuCon->Choice();
	}
	return BattleChoice::None;
}
