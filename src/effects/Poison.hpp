#pragma once

#include "BattleEffect.hpp"

#include "../BattlePawn.hpp"

namespace Demo
{
// name: Poison
// args:
// 1. Poison Strenght
// 2. Poison Duration
// 3. Poison Damage Delay
// Overall damage from this effect is: PoisonStrenght * PoisonDuration / PoisonDamageDelay
class Poison : public Demo::BattleEffect
{
public:
	Poison(unsigned int strenght = 1, double lastingTime = 10.0, double repeatDelay = 2.0);
	virtual ~Poison();

public:
	virtual void BattleUpdate(double delta, BattlePawn* ) override;
	virtual BattleEffect* Clone() override;
	virtual Effect* Clone(const std::vector<double>& args) override;
	virtual void MergeEffect(Effect*) override;
	
	virtual void ModifyInDamage(DamageModificationInfo& );
	virtual void ModifyOutDamage(DamageModificationInfo& );
	virtual void Update(double delta);
	
private:
	unsigned int poisonStrenght;
	double durationLeft;
	double currentDuration;
	double delay;
	
};

}

