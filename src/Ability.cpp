#include "Ability.hpp"

#include "graphic/TextureHolder.hpp"

#include <sstream>

Demo::Ability::Ability()
:cooldown(0.0)
,maxCooldown(1.0)
,background(PointU(0,0))
,cooldownBar(PointU(0,0))
,pawnCooldownBar(PointU(0,0))
{
	background.SetColor(128,128,128,200);
	cooldownBar.SetColor(0,0,0,150);
	cooldownBar.SetRotation(270);
	pawnCooldownBar.SetColor(255,255,255,150);
}

Demo::Ability::~Ability()
{
}

void Demo::Ability::Update(float delta)
{
	ReduceCooldown(delta);
	cooldownBar.SetPercentage(cooldown * 100.0 / maxCooldown);
}

void Demo::Ability::Draw(const Canvas& canvas) const
{
	background.Draw(canvas);
	sprite.Draw(canvas);
	cooldownBar.Draw(canvas);
	pawnCooldownBar.Draw(canvas);
}

std::vector<Demo::Action> Demo::Ability::Use(const Location& destination, bool targetingEnemy)
{
	std::vector<Demo::Action> result;
	if(cooldown == 0.0 && !pawnCooldownLock)
	{
		result.push_back(ConstructAction(destination, targetingEnemy));
		cooldown += maxCooldown;
		
		sprite.SetColorMultiply(200,200,200);
		
		if(result[0].actionData == "" && result[0].subactions.size() == 0)
			cooldown -= maxCooldown;
	}
	return result;
}

void Demo::Ability::SetMaxCooldown(double limit)
{
	if(limit < 0.0) 
		return;
	maxCooldown = limit;
}

double Demo::Ability::GetMaxCooldown()
{
	return maxCooldown;
}

void Demo::Ability::SetCooldown(double amount)
{
	cooldown = amount;
}

double Demo::Ability::GetCooldown()
{
	return cooldown;
}

void Demo::Ability::SetPawnCooldownPercentage(const double& percentage)
{
	pawnCooldownBar.SetPercentage(percentage);
	pawnCooldownLock = (percentage > 0.0);
}

void Demo::Ability::ReduceCooldown(double amount)
{
	if(IsCoolingDown() && !pawnCooldownLock)
	{
		cooldown -= amount;
		if(cooldown <= 0.0)
		{
			cooldown = 0.0;
			sprite.SetColorMultiply(255,255,255);
		}
	}
}

void Demo::Ability::ResetCooldown()
{
	cooldown = 0.0;
}

void Demo::Ability::IncreaseCooldown(double amount)
{
	if(amount > 0.0)
	{
		cooldown += amount;
		sprite.SetColorMultiply(0,200,200);
	}
}

bool Demo::Ability::IsCoolingDown()
{
	return cooldown > 0.0;
}

bool Demo::Ability::CooldownStatusChanged()
{
	bool result = IsCoolingDown() != wasCoolingDown;
	wasCoolingDown = IsCoolingDown();
	return result;
}

void Demo::Ability::SetAbilityRange(const ActionRange& newRange)
{
	range = newRange;
}

Demo::ActionRange Demo::Ability::GetAbilityRange()
{
	return range;
}

bool Demo::Ability::WasItYourAction(const CallBack& whatHappened, const std::string& actionData)
{
	return false;
}

bool Demo::Ability::IsAbilityDraggable()
{
	if(range == ActionRange::None ||
		range == ActionRange::Self ||
		range == ActionRange::AllOnFloor ||
		range == ActionRange::AllOnFloorExceptCaster)
		return false;
	return true;
}

void Demo::Ability::SetDefault(bool b)
{
	defaultAction = b;
}
 
Demo::ActionIntentions Demo::Ability::GetActionIntentions(const std::string& actionData)
{
	std::istringstream str(actionData);
	char c = 0;
	while(str.good())
	{
		c = str.get();
		if(c == '*')
		{
			std::string flag;
			str >> flag;
			if(flag == "support")
				return ActionIntentions::Support;
			if(flag == "berserk")
				return ActionIntentions::Berserk;
		}
	}
	return ActionIntentions::Attack;
}

void Demo::Ability::SetDisplayPosition(const PointU& pos)
{
	background.SetPosition(pos);
	sprite.SetPosition(pos);
	cooldownBar.SetPosition(pos);
	pawnCooldownBar.SetPosition(pos);
}

Demo::PointU Demo::Ability::GetDisplayPosition()
{
	return background.GetPosition();
}

void Demo::Ability::SetDisplaySize(const PointU& size)
{
	background.SetSize(size);
	sprite.SetSize(size);
	cooldownBar.SetSize(size);
	pawnCooldownBar.SetSize(size);
}

Demo::PointU Demo::Ability::GetDisplaySize()
{
	return background.GetSize();
}

void Demo::Ability::SetSprite(const std::string& name)
{
	TextureHolder::GetInstance().SetSpriteTexture(sprite,name);
}

std::string Demo::Ability::GetRidOfActionIntentionFlag(const std::string& actionData)
{
	std::stringstream str(actionData);
	std::stringstream out;
	
	while( str.good() && !str.eof() )
	{
		std::string temp;
		str >> temp;
		if(temp.size() > 0 && temp[0] != '*')
			out << temp << " ";
	}
	return out.str();
}

std::vector<std::pair<std::string,int>> Demo::Ability::GetAbilityDamageData(const bool& isSupportive)
{
	return {};
}
