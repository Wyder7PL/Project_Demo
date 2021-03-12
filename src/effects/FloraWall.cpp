#include "FloraWall.hpp"

#include "../graphic/TextureHolder.hpp"
#include "../BattlePawn.hpp"

Demo::FloraWall::FloraWall(int strenght, int thickness, double penality)
:BattleEffect("FloraWall")
,floraStrenght(strenght)
,floraThickness(thickness)
,dodgePenality(penality)
,durationLeft(0.0)
{
	Sprite* s = new Sprite();
	TextureHolder::GetInstance().SetSpriteTexture(*s,"FloraWall");
	effectGraphic.reset(s);
	
	using EpT = Effect::PersistenceType;
	Effect::SetFlag(EpT::Timed, true); 
	SetFlag(BattleEffect::EffectType::AttackInputModifier,true);
	
}

Demo::FloraWall::~FloraWall()
{
}

void Demo::FloraWall::BattleUpdate(double delta, BattlePawn* pawn)
{
	if(!appliedEffect)
	{
		pawn->GetDodge().AddMultiply(dodgePenality);
		appliedEffect = true;
	}
	if(durationLeft <= 0.0)
	{
		durationLeft += 1.0;
		floraStrenght -= floraThickness;
	}
	if(floraStrenght <= 0)
	{
		pawn->GetDodge().RemoveMultiply(dodgePenality);
		SetToDestroy();
	}
}

Demo::BattleEffect* Demo::FloraWall::Clone()
{
	FloraWall* ptr = new FloraWall(this->floraStrenght,this->floraThickness,this->dodgePenality);
	ptr->durationLeft = this->durationLeft;
	return ptr;
}
void Demo::FloraWall::ModifyInDamage(DamageModificationInfo& info)
{
	if(info.damageType == "Pain" && !info.heal)
	{
		unsigned int toAdsorb = 4 * floraThickness;
		
		if(info.amount >= toAdsorb)
		{
			info.preAddition.push_back(-((int)toAdsorb));
			floraStrenght -= toAdsorb;
		}
		else
		{
			floraStrenght -= info.amount;
			info.amount = 0;
		}
	}
}
Demo::Effect* Demo::FloraWall::Clone(const std::vector<double>& args)
{
	unsigned int str = 30;
	unsigned int thi = 1;
	double pen = 1.0;
	for(unsigned int i = 0; i < args.size(); ++i)
	{
		switch(i)
		{
			case 0: str = abs(round(args[i])); break;
			case 1: thi = abs(round(args[i])); break;
			case 2: pen = abs(args[i]); break;
			default: break;
		}
	}
	return new FloraWall(str,thi,pen);
}

void Demo::FloraWall::MergeEffect(Effect* e)
{
	FloraWall* ptr = dynamic_cast<FloraWall*>(e);
	if(ptr != nullptr)
	{
		floraStrenght += ptr->floraStrenght;
		if(floraThickness == ptr->floraThickness)
			floraThickness += 1;
		
		else if(ptr->floraThickness > floraThickness)
			floraThickness = ptr->floraThickness;
	}
}

void Demo::FloraWall::ModifyOutDamage(DamageModificationInfo& )
{
}
void Demo::FloraWall::Update(double delta)
{
}
