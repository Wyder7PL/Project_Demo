#include "ThornEntangle.hpp"

#include "../graphic/TextureHolder.hpp"
#include "../BattlePawn.hpp"

#include <math.h>

Demo::ThornEntangle::ThornEntangle(int strenght)
:BattleEffect("ThornEntangle")
,thornStrenght(strenght)
,durationLeft(2.0)
{
	Sprite* s = new Sprite();
	TextureHolder::GetInstance().SetSpriteTexture(*s,"ThornEntangle1");
	effectGraphic.reset(s);
	
	using EpT = Effect::PersistenceType;
	Effect::SetFlag(EpT::Timed, true); 
}

Demo::ThornEntangle::~ThornEntangle()
{
}

void Demo::ThornEntangle::BattleUpdate(double delta, BattlePawn* pawn)
{
	if(!appliedEffect)
	{
		pawn->GetDodge().AddPostAddition(15);
		appliedEffect = true;
	}
	
	durationLeft -= delta;
	if(durationLeft <= 0.0)
	{
		int damage = pow(phase,2) * pawnSize;
		if(damage > 0)
		{
			pawn->DealDamage("Pain",damage,0);
		}
		durationLeft += 2.0;
		thornStrenght -=  (int)( powf( 3, ((double)phase)/2.0 ) + powf( pawnSize, 2.0 + 0.5 * (phase - 1) ) );
		if(thornStrenght <= 0)
		{
			pawn->GetDodge().RemovePostAddition(15);
			SetToDestroy();
		}
	}
}

void Demo::ThornEntangle::Update(double delta)
{
	
}

Demo::BattleEffect* Demo::ThornEntangle::Clone()
{
	ThornEntangle* ptr = new ThornEntangle(this->thornStrenght);
	ptr->durationLeft = this->durationLeft;
	return ptr;
}

Demo::Effect* Demo::ThornEntangle::Clone(const std::vector<double>& args)
{
	unsigned int str = 1;
	for(unsigned int i = 0; i < args.size(); ++i)
	{
		switch(i)
		{
			case 0: str = abs(round(args[i])); break;
			default: break;
		}
	}
	return new ThornEntangle(str);
}

void Demo::ThornEntangle::ModifyInDamage(DamageModificationInfo& )
{
}

void Demo::ThornEntangle::ModifyOutDamage(DamageModificationInfo& )
{
}

void Demo::ThornEntangle::MergeEffect(Effect* e)
{
	ThornEntangle* ptr = dynamic_cast<ThornEntangle*>(e);
	if(ptr != nullptr)
		thornStrenght += ptr->GetThornStrenght();
}

int Demo::ThornEntangle::GetThornStrenght()
{
	return thornStrenght;
}
