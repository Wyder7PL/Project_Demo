#pragma once

#include "BattleEffect.hpp"

namespace Demo
{

class Prepare : public BattleEffect
{
public:
	Prepare(unsigned int amount = 1, double percentage = 1.0, double effectDuration = 3.0);
	virtual ~Prepare();

	virtual void BattleUpdate(double delta, BattlePawn* );
	virtual BattleEffect* Clone();
	virtual Effect* Clone(const std::vector<double>& args) override;
	virtual void MergeEffect(Effect*) override;
	virtual void ModifyInDamage(DamageModificationInfo& );
	virtual void ModifyOutDamage(DamageModificationInfo& );
	virtual void Update(double delta);
	
private:
	double timeLeft;
	unsigned int damageDecrease;
	double percentageDecrease;
	bool used = false;
	bool activated = false;
};

}

