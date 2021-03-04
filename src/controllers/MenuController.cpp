#include "MenuController.hpp"

Demo::MenuController::MenuController(BattleData* data,std::weak_ptr<Demo::Input> input)
:Controller(data)
,inputHandle(input)
,justPressed(true)
,firstFrame(true)
,choice(BattleChoice::None)
{
	std::pair<std::string,Button> alpha = std::make_pair("AlphaBattle",std::move(Button(PointU(300,80))));
	alpha.second.UpdateText("Alpha Battle");
	alpha.second.SetPosition(PointU(1000,400));
	alpha.second.SetSize(PointU(300,80));
	alpha.second.SetBorderColor(200,0,0,255);
	alpha.second.SetColor(100,100,100,255);
	data->buttons.insert(std::move(alpha));
	
	std::pair<std::string,Button> tutorial = std::make_pair("Tutorial",std::move(Button(PointU(300,80))));
	tutorial.second.UpdateText("Tutorial");
	tutorial.second.SetPosition(PointU(400,400));
	tutorial.second.SetSize(PointU(300,80));
	tutorial.second.SetBorderColor(200,0,0,255);
	tutorial.second.SetColor(100,100,100,255);
	data->buttons.insert(std::move(tutorial));
}

Demo::MenuController::~MenuController()
{
}

void Demo::MenuController::Update(double delta)
{
	if(auto input = inputHandle.lock())
	{
		PointU p((*input).MousePos().x,(*input).MousePos().y);
		
		if(battleDataPtr->buttons["AlphaBattle"].IsMouseOnButton(p))
		{
			if((*input).MouseLeft() && justPressed && choice == BattleChoice::None)
				choice = BattleChoice::Alpha;
		}
		if(battleDataPtr->buttons["Tutorial"].IsMouseOnButton(p))
		{
			if((*input).MouseLeft() && justPressed && choice == BattleChoice::None)
				choice = BattleChoice::Tutorial;
		}
		if((*input).MouseLeft() && justPressed)
			justPressed = false;
		if(!(*input).MouseLeft() && !justPressed)
			justPressed = true;
	}
}

void Demo::MenuController::AttachNewInput(std::weak_ptr<Demo::Input> input)
{
	inputHandle = input;
}

Demo::BattleChoice Demo::MenuController::Choice()
{
	return choice;
}
