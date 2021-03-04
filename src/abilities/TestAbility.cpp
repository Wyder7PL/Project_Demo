#include "TestAbility.hpp"

Demo::TestAbility::TestAbility()
{
	SetSprite("Marked");
	SetMaxCooldown(10.0);
}

Demo::TestAbility::~TestAbility()
{
}

Demo::Ability* Demo::TestAbility::Clone()
{
	TestAbility* ptr = new TestAbility();
	ptr->cooldown = this->cooldown;
	ptr->maxCooldown = this->maxCooldown;
	ptr->defaultAction = this->defaultAction;
	ptr->range = this->range;
	ptr->sprite = this->sprite;
	return ptr;
}

Demo::Action Demo::TestAbility::ConstructAction(const Location& destination, bool targetingEnemy)
{
	Action n(destination);
	n.aType = Action::ActionType::ApplyEffect;
	n.actionData = "Marked 1 1.2 5.0";
	n.defaultAction = defaultAction;
	return n;
}
