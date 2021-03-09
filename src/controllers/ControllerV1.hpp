#pragma once

#include "Controller.hpp"
#include "../PawnList.hpp"

namespace Demo
{

class ControllerV1 : public Controller
{
public:
	explicit ControllerV1(BattleData* data);
	~ControllerV1();
	
	virtual void Update(double delta) override;
	
	private:
	void FindTarget(bool& find, PointU& location);
};

}

