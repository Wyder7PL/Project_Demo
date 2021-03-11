#include "Distraction.hpp"
#include "../graphic/TextureHolder.hpp"

Demo::Distraction::Distraction(unsigned int amount, double effectDuration)
:	BattleEffect("Distraction")
	,timeLeft(effectDuration)
	,damageIncrease(amount)
{
	Sprite* s = new Sprite();
	TextureHolder::GetInstance().SetSpriteTexture(*s,"Distraction");
	effectGraphic.reset(s);
	
	using EpT = Effect::PersistenceType;
	Effect::SetFlag(EpT::Timed, true); 
	using BeT = BattleEffect::EffectType;
	SetFlag(BeT::AttackInputModifier,true);
}

Demo::Distraction::~Distraction()
{
}

void Demo::Distraction::BattleUpdate(double delta, BattlePawn* )
{
	if(!activated)
		activated = true;
	if(used || timeLeft <= 0.0)
	{
		using EpT = Effect::PersistenceType;
		Effect::SetFlag(EpT::Destroyed, true); 
	}
	timeLeft -= delta;
}

Demo::BattleEffect* Demo::Distraction::Clone()
{
	Distraction* ptr = new Distraction(this->damageIncrease,this->timeLeft);
	ptr->activated = this->activated;
	ptr->used = this->used;
	return ptr;
}

Demo::Effect* Demo::Distraction::Clone(const std::vector<double>& args)
{
	unsigned int str = 1;
	double dur = 3.0;
	for(unsigned int i = 0; i < args.size(); ++i)
	{
		switch(i)
		{
			case 0: str = abs(round(args[i])); break;
			case 1: dur = abs(args[i]); break;
			default: break;
		}
	}
	return new Distraction(str,dur);
}

void Demo::Distraction::ModifyInDamage(DamageModificationInfo& info)
{
	if(activated && !used && !info.neutralized && !info.heal)
	{
		info.preAddition.push_back((double)damageIncrease);
		used = true;
	}
}

void Demo::Distraction::ModifyOutDamage(DamageModificationInfo& )
{
}

void Demo::Distraction::Update(double delta)
{
}

void Demo::Distraction::MergeEffect(Effect* )
{
	used = false;
}
