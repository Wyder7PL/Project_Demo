#pragma once

#include "../Ability.hpp"

#include <vector>

namespace Demo
{

class GenericAbility : public Ability
{
public:
	GenericAbility();
	virtual ~GenericAbility();

public:
	virtual Ability* Clone() override;
	virtual Action ConstructAction(const Location& destination, bool targetingEnemy) override;
	
	void AddDamageToAbility(const std::string& damage);
	void AddEffectToAbility(const std::string& effect);
	
	virtual std::vector<std::pair<std::string,int>> GetAbilityDamageData(const bool& isSupportive) override;
	
private:
	std::vector<std::string> abilityAttacks;
	std::vector<std::string> abilityEffects;
};

}

