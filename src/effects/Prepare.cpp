#include "Prepare.hpp"
#include "../graphic/TextureHolder.hpp"
#include "../BattlePawn.hpp"

Demo::Prepare::Prepare(unsigned int amount, double percentage, double effectDuration)
:BattleEffect("Prepare")
,timeLeft(effectDuration)
,damageDecrease(amount)
,percentageDecrease(percentage)
{
	Sprite* s = new Sprite();
	TextureHolder::GetInstance().SetSpriteTexture(*s,"Prepare");
	effectGraphic.reset(s);
	
	using EpT = Effect::PersistenceType;
	Effect::SetFlag(EpT::Timed, true); 
	using BeT = BattleEffect::EffectType;
	SetFlag(BeT::AttackInputModifier,true);
}

Demo::Prepare::~Prepare()
{
}

void Demo::Prepare::BattleUpdate(double delta, BattlePawn* )
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

Demo::BattleEffect* Demo::Prepare::Clone()
{
	Prepare* ptr = new Prepare(this->damageDecrease,this->percentageDecrease,this->timeLeft);
	ptr->activated = this->activated;
	ptr->used = this->used;
	return ptr;
}

Demo::Effect* Demo::Prepare::Clone(const std::vector<double>& args)
{
	unsigned int str = 1;
	double per = 3.0;
	double dur = 3.0;
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
	return new Prepare(str,per,dur);
}

void Demo::Prepare::ModifyInDamage(DamageModificationInfo& info)
{
	if(activated && !used && !info.neutralized)
	{
		info.preAddition.push_back(-(double)(damageDecrease));
		info.multiply.push_back(percentageDecrease);
		used = true;
	}
}

void Demo::Prepare::ModifyOutDamage(DamageModificationInfo& )
{
}
void Demo::Prepare::Update(double delta)
{
}

void Demo::Prepare::MergeEffect(Effect* )
{
	used = false;
}
