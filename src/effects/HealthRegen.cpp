#include "HealthRegen.hpp"
#include "../graphic/TextureHolder.hpp"

#include <math.h>

Demo::HealthRegen::HealthRegen(unsigned int strenght, double lastingTime, double repeatDelay)
:BattleEffect("HealthRegen")
,regenStrenght(strenght)
,durationLeft(lastingTime)
,currentDuration(0.0)
,delay(repeatDelay)
{
	Sprite* s = new Sprite();
	TextureHolder::GetInstance().SetSpriteTexture(*s,"HealthRegen");
	effectGraphic.reset(s);
	
	using EpT = Effect::PersistenceType;
	Effect::SetFlag(EpT::Timed, true); 
}

Demo::HealthRegen::~HealthRegen()
{
}

void Demo::HealthRegen::Update(double delta)
{
	
}

void Demo::HealthRegen::BattleUpdate(double delta, BattlePawn* pawn)
{
	currentDuration += delta;
	durationLeft -= delta;
	if(currentDuration >= delay)
	{
		pawn->GiveSupport("Pain",regenStrenght);
		currentDuration -= delay;
	}
	if(durationLeft <= 0)
		SetToDestroy();
}

Demo::BattleEffect* Demo::HealthRegen::Clone()
{
	HealthRegen* ptr = new HealthRegen(this->regenStrenght,this->durationLeft,this->currentDuration);
	ptr->delay = this->delay;
	return ptr;
}

Demo::Effect* Demo::HealthRegen::Clone(const std::vector<double>& args)
{
	unsigned int str = 1;
	double dur = 10.0;
	double del = 2.0;
	for(unsigned int i = 0; i < args.size(); ++i)
	{
		switch(i)
		{
			case 0: str = abs(round(args[i])); break;
			case 1: dur = abs(args[i]); break;
			case 2: del = abs(args[i]); break;
			default: break;
		}
	}
	return new HealthRegen(str,dur,del);
}

void Demo::HealthRegen::MergeEffect(Effect* )
{
	
}

void Demo::HealthRegen::ModifyInDamage(DamageModificationInfo& )
{
}
void Demo::HealthRegen::ModifyOutDamage(DamageModificationInfo& )
{
}
