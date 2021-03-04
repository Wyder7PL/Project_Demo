#pragma once

#include "BattleEffect.hpp" // Base class: Demo::BattleEffect

namespace Demo
{
// name: Critical
// args:
// 1. Damage Increase Value
// 2. Damage Percentage Increase
// 3. Effect Duration
// This effect will increase damage of one next pawn attack
class Critical : public Demo::BattleEffect
{
public:
	Critical(unsigned int amount = 0, double percentage = 1.0, double effectDuration = 5.0);
	virtual ~Critical();
	
	virtual void Update(double delta);

	virtual void BattleUpdate(double delta, BattlePawn* );
	virtual void MergeEffect(Effect*) override;
	
	virtual BattleEffect* Clone();
	virtual Effect* Clone(const std::vector<double>& args) override;
	
	virtual void ModifyInDamage(DamageModificationInfo& );
	virtual void ModifyOutDamage(DamageModificationInfo& );
	
private:
	double timeLeft;
	unsigned int damageIncrease;
	double percentageIncrease;
	bool used = false;
	bool activated = false;
};

}

