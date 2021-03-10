#pragma once

#include "../input/Input.hpp"
#include "Controller.hpp"

#include "../PawnList.hpp"
#include "../graphic/Button.hpp"
#include "../graphic/DefaultAbilityDamageDisplay.hpp"

#include "../effects/DefaultDamageAdsorption.hpp"
#include "../effects/DefaultDamageBooster.hpp"
#include "../effects/Poison.hpp"

#include <memory>

namespace Demo
{

class PlayerController : public Controller
{
public:
	PlayerController(BattleData* data,std::weak_ptr<Input> input);
	virtual ~PlayerController();

	virtual void Update(double delta) override;

	void AttachNewInput(std::weak_ptr<Input> input);
	
private:
	
	void UpdateArrow(std::shared_ptr<Input> input);
	void PerformAttack(const PointU& attackLocation);
	
	std::weak_ptr<Input> inputHandle;
	
	PointU arrowBegin;
	bool arrowIsUsed;
	Location attackPawn;
	unsigned int abilityIndex = 0;
	bool justPressed;
	
	bool firstFrame = true;
};

}

