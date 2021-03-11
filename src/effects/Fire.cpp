#include "Fire.hpp"
#include "../graphic/TextureHolder.hpp"
#include "../BattlePawn.hpp"

#include <math.h>

Demo::Fire::Fire(int strenght)
:BattleEffect("Fire")
,fireStrenght(strenght)
,durationLeft(8.0 / powf( double(fireStrenght+5) ,0.7))
{
	Sprite* s = new Sprite();
	TextureHolder::GetInstance().SetSpriteTexture(*s,"Fire");
	effectGraphic.reset(s);
	
	using EpT = Effect::PersistenceType;
	Effect::SetFlag(EpT::Timed, true); 
}

Demo::Fire::~Fire()
{
}

void Demo::Fire::BattleUpdate(double delta, BattlePawn* pawn)
{
	durationLeft -= delta;
	if(durationLeft <= 0.0)
	{
		int damage = log(fireStrenght/2+1)+1;
		if(damage > 0)
		{
			if(damage > 1)
				pawn->DealDamage("Pain",damage-1);
			pawn->DealDamage("Pain",1,0,true);
		}
		else
			SetToDestroy();
		durationLeft += 8.0 / powf( double(fireStrenght+5) ,0.7);
		fireStrenght -= damage;
	}
}

Demo::BattleEffect* Demo::Fire::Clone()
{
	Fire* ptr = new Fire(this->fireStrenght);
	ptr->durationLeft = this->durationLeft;
	return ptr;
}

Demo::Effect* Demo::Fire::Clone(const std::vector<double>& args)
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
	return new Fire(str);
}

void Demo::Fire::MergeEffect(Effect* e)
{
	Fire* ptr = dynamic_cast<Fire*>(e);
	if(ptr != nullptr)
		fireStrenght += ptr->GetFireStrenght();
}

void Demo::Fire::ModifyInDamage(DamageModificationInfo& )
{
}
void Demo::Fire::ModifyOutDamage(DamageModificationInfo& )
{
}
void Demo::Fire::Update(double delta)
{
}

int Demo::Fire::GetFireStrenght()
{
	return fireStrenght;
}
