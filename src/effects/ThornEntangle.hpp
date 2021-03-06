#pragma once

#include "BattleEffect.hpp"
#include "../graphic/MultiframeSprite.hpp"

namespace Demo
{

class ThornEntangle : public Demo::BattleEffect
{
public:
	ThornEntangle(int strenght = 10);
	virtual ~ThornEntangle();

	virtual void Update(double delta);
	virtual void BattleUpdate(double delta, BattlePawn* );
	
	virtual BattleEffect* Clone();
	virtual Effect* Clone(const std::vector<double>& args) override;
	
	virtual void MergeEffect(Effect*) override;
	
	virtual void ModifyInDamage(DamageModificationInfo& );
	virtual void ModifyOutDamage(DamageModificationInfo& );
	
	int GetThornStrenght();
	
private:
	
	void ChangePhase(BattlePawn* pawn);
	
	int thornStrenght;
	double durationLeft;
	
	bool appliedEffect = false;
	unsigned int phase = 1;
	unsigned int pawnSize = 1;
	
	std::shared_ptr<MultiframeSprite> multiSprite;
};

}
