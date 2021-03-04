#include "BattleEffect.hpp"

Demo::BattleEffect::BattleEffect(const std::string& effectName)
:Effect(effectName),effectTypeFlags(0)
{
	Effect::SetFlag(Effect::PersistenceType::BattleAffecting,true);
}

Demo::BattleEffect::~BattleEffect()
{
}

bool Demo::BattleEffect::GetFlag(EffectType flag)
{
	return effectTypeFlags[flag];
}

void Demo::BattleEffect::SetFlag(EffectType flag, bool value)
{
	effectTypeFlags[flag] = value;
}

void Demo::BattleEffect::ModifyInDamage(DamageModificationInfo& )
{
	
}

void Demo::BattleEffect::ModifyOutDamage(DamageModificationInfo& )
{
	
}
