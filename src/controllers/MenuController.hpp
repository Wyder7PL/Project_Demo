#pragma once

#include "Controller.hpp"
#include "../input/Input.hpp"

namespace Demo
{

enum class BattleChoice{
	None,
	Alpha,
	Tutorial
};

class MenuController : public Demo::Controller
{
public:
	MenuController(BattleData* data,std::weak_ptr<Input> input);
	virtual ~MenuController();

	virtual void Update(double delta);
	void AttachNewInput(std::weak_ptr<Input> input);
	
	BattleChoice Choice();
private:
	
	std::weak_ptr<Input> inputHandle;
	
	bool justPressed;
	bool firstFrame;
	BattleChoice choice;
};

}

