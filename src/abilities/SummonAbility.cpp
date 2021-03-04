#include "SummonAbility.hpp"

Demo::SummonAbility::SummonAbility()
{
	range = ActionRange::Point;
	SetSprite("Summon");
}

Demo::SummonAbility::~SummonAbility()
{
}

Demo::Ability* Demo::SummonAbility::Clone()
{
	SummonAbility* ptr = new SummonAbility();
	ptr->cooldown = this->cooldown;
	ptr->maxCooldown = this->maxCooldown;
	ptr->defaultAction = this->defaultAction;
	ptr->range = this->range;
	ptr->sprite = this->sprite;
	
	ptr->pawnName = this->pawnName;
	return ptr;
}

Demo::Action Demo::SummonAbility::ConstructAction(const Location& destination, bool targetingEnemy)
{
	Action summon(destination);
	summon.aType = Action::ActionType::Create;
	summon.actionData = pawnName;
	summon.defaultAction = defaultAction;
	return summon;
}

bool Demo::SummonAbility::WasItYourAction(const CallBack& whatHappened, const std::string& actionData)
{
	if(actionData == pawnName)
	{
		cooldown = 0.0;
		return true;
	}
	return false;
}

void Demo::SummonAbility::SetSummonName(const std::string& name)
{
	pawnName = name;
}
