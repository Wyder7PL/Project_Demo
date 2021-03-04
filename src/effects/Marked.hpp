#pragma once

#include "BattleEffect.hpp" // Base class: Demo::BattleEffect

namespace Demo
{

class Marked : public Demo::BattleEffect
{
public:
	Marked(unsigned int amount = 1, double percentage = 1.0, double lastingTime = 5.0);
	virtual ~Marked();

	virtual void BattleUpdate(double delta, BattlePawn* );
	virtual BattleEffect* Clone();
	virtual Effect* Clone(const std::vector<double>& args) override;
	virtual void MergeEffect(Effect*) override;
	
	virtual void ModifyInDamage(DamageModificationInfo& );
	virtual void ModifyOutDamage(DamageModificationInfo& );
	virtual void Update(double delta);
	
private:
	unsigned int decreaseAmount;
	double percentageDecrease;
	double durationLeft;
	double currentDuration;
};

}
