#pragma once

#include "BattleEffect.hpp"

namespace Demo
{

class Shield : public Demo::BattleEffect
{
public:
	Shield(unsigned int amount = 1, double percentage = 1.0, double effectDuration = 3.0);
	virtual ~Shield();

	virtual void BattleUpdate(double delta, BattlePawn* );
	virtual void Update(double delta);
	
	virtual BattleEffect* Clone();
	virtual Effect* Clone(const std::vector<double>& args) override;
	virtual void MergeEffect(Effect*) override;
	
	virtual void ModifyInDamage(DamageModificationInfo& );
	virtual void ModifyOutDamage(DamageModificationInfo& );
	
private:
	double timeLeft;
	unsigned int damageDecrease;
	double percentageDecrease;
};

}

