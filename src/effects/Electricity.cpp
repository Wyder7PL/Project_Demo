#include "Electricity.hpp"
#include "../graphic/TextureHolder.hpp"
#include "../BattlePawn.hpp"

Demo::Electricity::Electricity(int voltage, int intensity)
:BattleEffect("Electricity")
,electricityVoltage(voltage)
,electricityIntensity(intensity)
,durationLeft(0.0)
{
	Sprite* s = new Sprite();
	TextureHolder::GetInstance().SetSpriteTexture(*s,"Electricity");
	effectGraphic.reset(s);
	
	using EpT = Effect::PersistenceType;
	Effect::SetFlag(EpT::Timed, true); 
	
}

Demo::Electricity::~Electricity()
{
}

void Demo::Electricity::BattleUpdate(double delta, BattlePawn* pawn)
{
	if(durationLeft <= 0.0)
	{
		durationLeft += 1.0;
		if(electricityIntensity < pawnSize)
		{
			double cooldown = (double)(electricityIntensity) / (double)(pawnSize); 
			pawn->IncreasePawnCooldown(cooldown);
		}
		else
		{
			pawn->IncreasePawnCooldown(1.0);
			if(electricityIntensity > pawnSize)
				pawn->DealDamage("Pain",electricityIntensity-pawnSize,0,true);
		}
		electricityVoltage -= electricityIntensity;
	}
	durationLeft -= delta;
	if(electricityVoltage <= 0)
		SetToDestroy();
}

Demo::BattleEffect* Demo::Electricity::Clone()
{
	Electricity* ptr = new Electricity(this->electricityVoltage,this->electricityIntensity);
	ptr->durationLeft = this->durationLeft;
	return ptr;
}

Demo::Effect* Demo::Electricity::Clone(const std::vector<double>& args)
{
	unsigned int vol = 10;
	unsigned int inte = 3;
	for(unsigned int i = 0; i < args.size(); ++i)
	{
		switch(i)
		{
			case 0: vol = abs(round(args[i])); break;
			case 1: inte = abs(round(args[i])); break;
			default: break;
		}
	}
	return new Electricity(vol,inte);
}

void Demo::Electricity::MergeEffect(Effect* e)
{
	Electricity* ptr = dynamic_cast<Electricity*>(e);
	if(ptr != nullptr)
	{
		electricityVoltage += ptr->electricityVoltage;
		if(electricityIntensity == ptr->electricityIntensity)
			electricityIntensity += 1;
		
		else if(ptr->electricityIntensity > electricityIntensity)
			electricityIntensity = ptr->electricityIntensity;
	}
}

void Demo::Electricity::ModifyInDamage(DamageModificationInfo& )
{
}
void Demo::Electricity::ModifyOutDamage(DamageModificationInfo& )
{
}
void Demo::Electricity::Update(double delta)
{
}


