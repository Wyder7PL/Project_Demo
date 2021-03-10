#pragma once

#include "../Battle.hpp"

#include "../controllers/ControllerV1.hpp"

namespace Demo
{

class AlphaBattle : public Demo::Battle
{
public:
	AlphaBattle();
	virtual ~AlphaBattle();

	virtual void Update(double delta);
};

}

