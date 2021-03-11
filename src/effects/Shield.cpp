#include "Shield.hpp"
#include "../graphic/TextureHolder.hpp"

Demo::Shield::Shield(unsigned int amount, double percentage, double effectDuration)
:BattleEffect("Shield")
,timeLeft(effectDuration)
,damageDecrease(amount)
,percentageDecrease(percentage)
{
	Sprite* s = new Sprite();
	TextureHolder::GetInstance().SetSpriteTexture(*s,"Shield");
	effectGraphic.reset(s);
	
	using EpT = Effect::PersistenceType;
	Effect::SetFlag(EpT::Timed, true); 
	using BeT = BattleEffect::EffectType;
	SetFlag(BeT::AttackInputModifier,true);
}

Demo::Shield::~Shield()
{
}

void Demo::Shield::BattleUpdate(double delta, BattlePawn* )
{
	if(timeLeft <= 0.0)
	{
		using EpT = Effect::PersistenceType;
		Effect::SetFlag(EpT::Destroyed, true); 
	}
	timeLeft -= delta;
}

Demo::BattleEffect* Demo::Shield::Clone()
{
	Shield* ptr = new Shield(this->damageDecrease,this->percentageDecrease,this->timeLeft);
	return ptr;
}

Demo::Effect* Demo::Shield::Clone(const std::vector<double>& args)
{
	unsigned int str = 0;
	double per = 1.0;
	double dur = 5.0;
	for(unsigned int i = 0; i < args.size(); ++i)
	{
		switch(i)
		{
			case 0: str = abs(round(args[i])); break;
			case 1: per = abs(args[i]); break;
			case 2: dur = abs(args[i]); break;
			default: break;
		}
	}
	return new Shield(str,per,dur);
}

void Demo::Shield::ModifyInDamage(DamageModificationInfo& info)
{
	if(!info.neutralized)
	{
		info.preAddition.push_back(-(double(damageDecrease)));
		info.multiply.push_back(percentageDecrease);
	}
}

void Demo::Shield::ModifyOutDamage(DamageModificationInfo& )
{
	
}

void Demo::Shield::Update(double delta)
{
	
}

void Demo::Shield::MergeEffect(Effect* )
{
	
}
