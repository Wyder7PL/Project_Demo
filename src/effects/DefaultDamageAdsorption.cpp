#include "DefaultDamageAdsorption.hpp"

Demo::DefaultDamageAdsorption::DefaultDamageAdsorption(const unsigned int& ads)
:BattleEffect("Adsorption"),adsorption(ads)
{
	SetFlag(BattleEffect::EffectType::AttackInputModifier,true);
}

Demo::DefaultDamageAdsorption::~DefaultDamageAdsorption()
{
}

void Demo::DefaultDamageAdsorption::BattleUpdate(double delta, BattlePawn* )
{
	
}

void Demo::DefaultDamageAdsorption::ModifyInDamage(DamageModificationInfo& info)
{
	if(info.damageType == "Pain" && !info.heal)
		info.preAddition.push_back(-((int)adsorption));
}

Demo::BattleEffect* Demo::DefaultDamageAdsorption::Clone()
{
	return new DefaultDamageAdsorption(this->adsorption);
}

Demo::Effect* Demo::DefaultDamageAdsorption::Clone(const std::vector<double>& args)
{
	unsigned int b = 0;
	if(args.size() > 0)
		b = args[0];
	return new DefaultDamageAdsorption(b);
}

void Demo::DefaultDamageAdsorption::MergeEffect(Effect* )
{
}
