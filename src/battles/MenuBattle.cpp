#include "MenuBattle.hpp"

#include "TutorialBattle.hpp"
#include "AlphaBattle.hpp"

Demo::MenuBattle::MenuBattle()
:controllerIsSet(false)
,battleSet(false)
{
	
}

Demo::MenuBattle::~MenuBattle()
{
}

void Demo::MenuBattle::Update(double delta)
{
	if(!battleSet)
	{
		Battle::Update(delta);
		if(controllerIsSet)
			menuCon->Update(delta);
		SetNewBattle();
	}
	else
		actualBattle->Update(delta);
}

void Demo::MenuBattle::Draw(const Canvas& canvas) const
{
	if(battleSet)
		actualBattle->Draw(canvas);
	else
		Battle::Draw(canvas);
}

void Demo::MenuBattle::CreatePlayerController(std::weak_ptr<Input> ptr)
{
	inputPtr = ptr;
	menuCon = std::make_unique<MenuController>(&data,inputPtr);
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


bool Demo::MenuBattle::BattleEnded()
{
	if(battleSet)
		return actualBattle->BattleEnded();
	return false;
}

void Demo::MenuBattle::SetNewBattle()
{
	if(!battleSet && Choice() != Demo::BattleChoice::None)
	{
		if(Choice() == Demo::BattleChoice::Alpha)
			actualBattle = std::make_unique<AlphaBattle>();
		else if(Choice() == Demo::BattleChoice::Tutorial)
			actualBattle = std::make_unique<TutorialBattle>();
		
		actualBattle->CreatePlayerController(inputPtr);
		actualBattle->SetFramerate(60);
		battleSet = true;
	}
}
