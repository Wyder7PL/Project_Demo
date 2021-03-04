#include "Marked.hpp"
#include "../graphic/TextureHolder.hpp"

Demo::Marked::Marked(unsigned int amount, double percentage, double lastingTime)
:BattleEffect("Marked")
,decreaseAmount(amount)
,percentageDecrease(percentage)
,durationLeft(lastingTime)
{
	Sprite* s = new Sprite();
	TextureHolder::GetInstance().SetSpriteTexture(*s,"Marked");
	effectGraphic.reset(s);
	
	using EpT = Effect::PersistenceType;
	Effect::SetFlag(EpT::Timed, true); 
	using BeT = BattleEffect::EffectType;
	SetFlag(BeT::AttackInputModifier,true);
}

Demo::Marked::~Marked()
{
}

void Demo::Marked::BattleUpdate(double delta, BattlePawn* )
{
	durationLeft -= delta;
	if(durationLeft <= 0)
		SetToDestroy();
}

Demo::BattleEffect* Demo::Marked::Clone()
{
	Marked* ptr = new Marked(this->decreaseAmount,this->percentageDecrease,this->currentDuration);
	ptr->durationLeft = this->durationLeft;
	return ptr;
}

Demo::Effect* Demo::Marked::Clone(const std::vector<double>& args)
{
	unsigned int str = 1;
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
	return new Marked(str,per,dur);
}

void Demo::Marked::MergeEffect(Effect* )
{
}
void Demo::Marked::ModifyInDamage(DamageModificationInfo& info)
{
	if(info.damageType == "Pain" && !info.heal)
	{
		info.postAddition += decreaseAmount;
		info.multiply.push_back(percentageDecrease);
	}
}

void Demo::Marked::ModifyOutDamage(DamageModificationInfo& )
{
}
void Demo::Marked::Update(double delta)
{
}
