#include "Critical.hpp"
#include "../graphic/TextureHolder.hpp"

Demo::Critical::Critical(unsigned int amount, double percentage, double effectDuration)
:BattleEffect("Critical")
,timeLeft(effectDuration)
,damageIncrease(amount)
,percentageIncrease(percentage)
{
	Sprite* s = new Sprite();
	TextureHolder::GetInstance().SetSpriteTexture(*s,"CriticalStrike");
	effectGraphic.reset(s);
	
	using EpT = Effect::PersistenceType;
	Effect::SetFlag(EpT::Timed, true); 
	using BeT = BattleEffect::EffectType;
	SetFlag(BeT::AttackOutputDefaultModifier,true);
}

Demo::Critical::~Critical()
{
}

void Demo::Critical::BattleUpdate(double delta, BattlePawn* )
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

Demo::BattleEffect* Demo::Critical::Clone()
{
	Critical* ptr = new Critical(this->damageIncrease,this->percentageIncrease,this->timeLeft);
	ptr->activated = this->activated;
	ptr->used = this->used;
	return ptr;
}

Demo::Effect* Demo::Critical::Clone(const std::vector<double>& args)
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
	return new Critical(str,per,dur);
}

void Demo::Critical::ModifyInDamage(DamageModificationInfo& )
{
}

void Demo::Critical::ModifyOutDamage(DamageModificationInfo& info)
{
	if(activated && !used)
	{
		info.preAddition += double(damageIncrease);
		info.multiply.push_back(percentageIncrease);
		used = true;
	}
}

void Demo::Critical::Update(double delta)
{
}

void Demo::Critical::MergeEffect(Effect* )
{
	used = false;
}
