#include "GenericAbility.hpp"

#include <sstream>

Demo::GenericAbility::GenericAbility()
{
}

Demo::GenericAbility::~GenericAbility()
{
}

Demo::Ability* Demo::GenericAbility::Clone()
{
	GenericAbility* ptr = new GenericAbility();
	ptr->cooldown = this->cooldown;
	ptr->maxCooldown = this->maxCooldown;
	ptr->defaultAction = this->defaultAction;
	ptr->range = this->range;
	ptr->sprite = this->sprite;
	
	ptr->abilityAttacks = this->abilityAttacks;
	ptr->abilityEffects = this->abilityEffects;
	return ptr;
}

Demo::Action Demo::GenericAbility::ConstructAction(const Location& destination, bool targetingEnemy)
{
	Action action(destination);
	if(targetingEnemy)
		action.aIntention = ActionIntentions::Attack;
	else
		action.aIntention = ActionIntentions::Support;
	action.aType = Action::ActionType::Default;
	bool first = true;
	for(unsigned int i = 0; i < abilityAttacks.size(); ++i)
	{
		ActionIntentions inten = GetActionIntentions(abilityAttacks[i]);
		bool support = (inten == ActionIntentions::Support);
		
		if(inten == ActionIntentions::Berserk || support == !targetingEnemy)
		{
			if(!first)
				action.actionData += " | ";
			action.actionData += GetRidOfActionIntentionFlag(abilityAttacks[i]);
			first = false;
		}
	}
	action.defaultAction = defaultAction;
	action.aRange = range;
	action.aAccuracy = accuracy.GetAccuracyPrecise();
	
	for(auto& i : abilityEffects)
	{
		ActionIntentions inten = GetActionIntentions(i);
		bool support = (inten == ActionIntentions::Support);
		
		if(inten == ActionIntentions::Berserk || support == !targetingEnemy)
		{
			Action effectAction(destination);
			effectAction.aType = Action::ActionType::ApplyEffect;
			effectAction.actionData = GetRidOfActionIntentionFlag(i);
			effectAction.defaultAction = defaultAction;
			effectAction.aRange = range;
			effectAction.aIntention = inten;
			action.AddSubaction(effectAction);
		}
	}
	return action;
}

void Demo::GenericAbility::AddDamageToAbility(const std::string& damage)
{
	abilityAttacks.push_back(damage);
}

void Demo::GenericAbility::AddEffectToAbility(const std::string& effect)
{
	abilityEffects.push_back(effect);
}

std::vector<std::pair<std::string,int>> Demo::GenericAbility::GetAbilityDamageData(const bool& isSupportive)
{
	std::vector<std::pair<std::string,int>> result;
	for(unsigned int i = 0; i < abilityAttacks.size(); ++i)
	{
		ActionIntentions inten = GetActionIntentions(abilityAttacks[i]);
		if( (inten == ActionIntentions::Support) == isSupportive)
		{
			std::istringstream stream;
			stream.str(GetRidOfActionIntentionFlag(abilityAttacks[i]));
			
			std::string damageType;
			int amount;
			stream >> damageType >> amount;
			result.push_back(std::make_pair(damageType,amount));
		}
	}
	return result;
}