#pragma once

#include "../Ability.hpp"

namespace Demo
{

class TestAbility : public Ability
{
public:
	TestAbility();
	virtual ~TestAbility();

	virtual Ability* Clone();
	virtual Action ConstructAction(const Location& destination, bool targetingEnemy);
};

}

