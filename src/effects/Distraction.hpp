#pragma once

#include "BattleEffect.hpp"

namespace Demo
{
// name: Distraction
// args:
// 1. Distraction Amount
// 2. Distraction Duration
// Within the duration of this effect, it will increase
// damage from one next attack by amount
class Distraction : public BattleEffect
{
public:
	Distraction(unsigned int amount = 1, double effectDuration = 3.0);
	virtual ~Distraction();

public:
	virtual void BattleUpdate(double delta, BattlePawn* );
	virtual BattleEffect* Clone() override;
	virtual Effect* Clone(const std::vector<double>& args) override;
	virtual void ModifyInDamage(DamageModificationInfo& ) override;
	virtual void ModifyOutDamage(DamageModificationInfo& ) override;
	virtual void MergeEffect(Effect*) override;
	virtual void Update(double delta) override;
private:
	double timeLeft;
	unsigned int damageIncrease;
	bool used = false;
	bool activated = false;
};

}

