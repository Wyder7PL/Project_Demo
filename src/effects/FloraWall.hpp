#pragma once

#include "BattleEffect.hpp" 

namespace Demo
{


// name: FloraWall
// args:
// 1. Flora Strenght
// 2. Flora Thickness
// 3. Dodge Penality
// It will temporary nerf target doodge but it will consume some of incoming attacks
// Flora Strenght determines how many damage it can adsorb
// Flora Thickness is how many damage can be adsorbed from one strike * 4
// Flora Strenght is reduced by Flora Thickness every second

class FloraWall : public Demo::BattleEffect
{
public:
	FloraWall(int strenght = 30, int thickness = 1, double penality = 1.5);
	virtual ~FloraWall();

	virtual void BattleUpdate(double delta, BattlePawn* );
	virtual void Update(double delta);
	
	virtual BattleEffect* Clone() override;
	virtual Effect* Clone(const std::vector<double>& args) override;
	
	virtual void MergeEffect(Effect*) override;
	
	virtual void ModifyInDamage(DamageModificationInfo& );
	virtual void ModifyOutDamage(DamageModificationInfo& );
	
private:
	int floraStrenght;
	int floraThickness;
	double dodgePenality;
	double durationLeft;
	
	bool appliedEffect = false;
};

}