#pragma once

#include "Effect.hpp"
#include "DamageModificationInfo.hpp"

#include <vector>

namespace Demo
{

class BattlePawn;

class BattleEffect : public Effect
{
public:

	enum EffectType : unsigned int
	{
		AttackInputModifier = 0,
		AttackOutputDefaultModifier,
		AttackOutputAbilityModifier
	};
	
	explicit BattleEffect(const std::string& effectName);
	virtual ~BattleEffect();
	
	virtual BattleEffect* Clone() = 0;
	
	virtual void Update(double delta) override {}
	virtual void BattleUpdate(double delta, BattlePawn*) = 0;
	
	bool GetFlag(EffectType flag);
	
	virtual void ModifyInDamage(DamageModificationInfo&);
	virtual void ModifyOutDamage(DamageModificationInfo&);
	
protected:
	void SetFlag(EffectType flag, bool value);
	
private:
	std::bitset<2> effectTypeFlags;
};


}

