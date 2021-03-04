#pragma once

#include "BattleEffect.hpp" // Base class: Demo::BattleEffect

namespace Demo
{
// name: "Adsorption"
// arg:
// 1. Adsorption Value
class DefaultDamageAdsorption : public Demo::BattleEffect
{
public:
	explicit DefaultDamageAdsorption(const unsigned int& ads = 0);
	virtual ~DefaultDamageAdsorption();

	virtual void BattleUpdate(double delta, BattlePawn* ) override;
	virtual BattleEffect* Clone();
	virtual Effect* Clone(const std::vector<double>& args) override;
	
	virtual void ModifyInDamage(DamageModificationInfo&) override;
	virtual void MergeEffect(Effect*) override;
	
private:
	unsigned int adsorption;
};

}

