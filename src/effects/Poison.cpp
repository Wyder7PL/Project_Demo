#include "Poison.hpp"
#include "../graphic/TextureHolder.hpp"

#include <math.h>

Demo::Poison::Poison(unsigned int strenght, double lastingTime, double repeatDelay)
:	BattleEffect("Poison"),
	poisonStrenght(strenght),
	durationLeft(lastingTime),
	currentDuration(0.0),
	delay(repeatDelay)
{
	Sprite* s = new Sprite();
	TextureHolder::GetInstance().SetSpriteTexture(*s,"Poison");
	effectGraphic.reset(s);
	
	using EpT = Effect::PersistenceType;
	Effect::SetFlag(EpT::Timed, true); 
}

Demo::Poison::~Poison()
{
}

void Demo::Poison::BattleUpdate(double delta, BattlePawn* pawn)
{
	currentDuration += delta;
	durationLeft -= delta;
	if(currentDuration >= delay)
	{
		pawn->DealDamage("Pain",poisonStrenght,true);
		currentDuration -= delay;
	}
	if(durationLeft <= 0)
		SetToDestroy();
}
Demo::BattleEffect* Demo::Poison::Clone()
{
	Poison* ptr = new Poison(this->poisonStrenght,this->durationLeft,this->currentDuration);
	ptr->delay = this->delay;
	return ptr;
}
Demo::Effect* Demo::Poison::Clone(const std::vector<double>& args)
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
	return new Poison(str,dur,del);
}

void Demo::Poison::MergeEffect(Effect* )
{
	
}
void Demo::Poison::ModifyInDamage(DamageModificationInfo& )
{
}
void Demo::Poison::ModifyOutDamage(DamageModificationInfo& )
{
}
void Demo::Poison::Update(double delta)
{
}
