#pragma once

#include "BattleEffect.hpp"

namespace Demo
{
// name: Electricity
// args:
// 1. Voltage
// 2. Intensity
// It increases cooldown of it's target, Voltage determines how long this effect last
// Intensity determines how strong this effect is

class Electricity : public Demo::BattleEffect
{
public:
	Electricity(int voltage = 10, int intensity = 3);
	virtual ~Electricity();

	virtual void Update(double delta);
	virtual void BattleUpdate(double delta, BattlePawn* );
	
	virtual BattleEffect* Clone();
	virtual Effect* Clone(const std::vector<double>& args) override;
	
	virtual void MergeEffect(Effect*) override;
	
	virtual void ModifyInDamage(DamageModificationInfo& );
	virtual void ModifyOutDamage(DamageModificationInfo& );
	
private:
	
	int electricityVoltage;
	int electricityIntensity;
	double durationLeft;
	
	int pawnSize = 3;
};

}