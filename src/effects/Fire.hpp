#pragma once

#include "BattleEffect.hpp" 


namespace Demo
{
// name: Fire
// args:
// 1. Poison Strenght
// Deals damage over time, 
class Fire : public Demo::BattleEffect
{
public:
	Fire(int strenght = 10);
	virtual ~Fire();
	
	virtual void Update(double delta);
	virtual void BattleUpdate(double delta, BattlePawn* );
	
	virtual BattleEffect* Clone();
	virtual Effect* Clone(const std::vector<double>& args) override;
	
	virtual void MergeEffect(Effect*) override;
	
	virtual void ModifyInDamage(DamageModificationInfo& );
	virtual void ModifyOutDamage(DamageModificationInfo& );
	
	int GetFireStrenght();
private:
	int fireStrenght;
	double durationLeft;
	
	const double timeMultiplayer = 3.0;
};

}


