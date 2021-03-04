#include "DefaultDamageBooster.hpp"

Demo::DefaultDamageBooster::DefaultDamageBooster(const unsigned int& boost)
:BattleEffect("DefaultBoost"),booost(boost)
{
	SetFlag(BattleEffect::EffectType::AttackOutputDefaultModifier,true);
}

Demo::DefaultDamageBooster::~DefaultDamageBooster()
{
}

void Demo::DefaultDamageBooster::BattleUpdate(double delta, BattlePawn* )
{
	
}

Demo::BattleEffect* Demo::DefaultDamageBooster::Clone()
{
	DefaultDamageBooster* ptr = new DefaultDamageBooster();
	ptr->booost = this->booost;
	return ptr;
}

Demo::Effect* Demo::DefaultDamageBooster::Clone(const std::vector<double>& args)
{
	unsigned int b = 0;
	if(args.size() > 0)
		b = args[0];
	return new DefaultDamageBooster(b);
}

void Demo::DefaultDamageBooster::ModifyOutDamage(DamageModificationInfo& info)
{
	if(info.damageType == "Pain" && !info.heal)
		info.preAddition += booost;
}

void Demo::DefaultDamageBooster::MergeEffect(Effect* )
{
}