#pragma once

#include "BattleEffect.hpp"

namespace Demo
{
// name: DefaultBoost
// args:
// 1. Damage Boost Value
class DefaultDamageBooster : public Demo::BattleEffect
{
public:
	DefaultDamageBooster(const unsigned int& boost = 0);
	virtual ~DefaultDamageBooster();

	virtual void BattleUpdate(double delta, BattlePawn* );
	virtual BattleEffect* Clone() override;
	virtual Effect* Clone(const std::vector<double>& args) override;
	virtual void ModifyOutDamage(DamageModificationInfo& );
	virtual void MergeEffect(Effect*) override;
	private:
	unsigned int booost;
};

}

