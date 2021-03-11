#pragma once

#include "BattleEffect.hpp"

#include "../BattlePawn.hpp"

namespace Demo
{
// name: HealthRegen
// args:
// 1. Regen Strenght
// 2. Regen Duration
// 3. Regen Restore Delay
// Regenerate HP over time
// Overall HP restoration from this effect is: RegenStrenght * RegenDuration / RegenRestoreDelay
class HealthRegen : public Demo::BattleEffect
{
public:
	HealthRegen(unsigned int strenght = 1, double lastingTime = 10.0, double repeatDelay = 2.0);
	virtual ~HealthRegen();

	virtual void BattleUpdate(double delta, BattlePawn* );
	virtual void Update(double delta);
	
	virtual BattleEffect* Clone() override;
	virtual Effect* Clone(const std::vector<double>& args) override;
	
	virtual void MergeEffect(Effect*) override;
	
	virtual void ModifyInDamage(DamageModificationInfo& );
	virtual void ModifyOutDamage(DamageModificationInfo& );
	
private:
	unsigned int regenStrenght;
	double durationLeft;
	double currentDuration;
	double delay;
};

}
