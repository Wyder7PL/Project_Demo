#pragma once

#include "../Ability.hpp"

#include <vector>

namespace Demo
{

class SummonAbility : public Demo::Ability
{
public:
	SummonAbility();
	virtual ~SummonAbility();

	virtual Ability* Clone();
	virtual Action ConstructAction(const Location& destination, bool targetingEnemy);
	
	virtual bool WasItYourAction(const CallBack& whatHappened, const std::string& actionData) override;
	
	void SetSummonName(const std::string& name);
private:
	std::string pawnName;
};

}

