#include "Dodge.hpp"

Demo::Dodge::Dodge(const unsigned int& maxDodge, const unsigned int& initialDodge)
:dodge(maxDodge)
,untilDodge(initialDodge)
{
	modifier.heal = false;
}

Demo::Dodge::~Dodge()
{
}

void Demo::Dodge::SetNewDodge(const unsigned int& amount)
{
	dodge = amount;
}

void Demo::Dodge::SetInitialDodge(const unsigned int& amount)
{
	if(amount > dodge)
		untilDodge = dodge;
	else 
		untilDodge = amount;
}

void Demo::Dodge::IncreaseDodge(const unsigned int& amount)
{
	untilDodge += amount;
}

bool Demo::Dodge::CanDodge()
{
	modifier.amount = dodge;
	return untilDodge >= (unsigned int)modifier.GetModifiedDamage();
}

void Demo::Dodge::ResetDodge()
{
	if(CanDodge())
	{
		modifier.amount = dodge;
		untilDodge -= modifier.GetModifiedDamage();
	}
}

void Demo::Dodge::RandomizeDodge()
{
	untilDodge = RandomInt<unsigned int>(0,dodge);
}

void Demo::Dodge::SetFrameInviolability(const bool& b)
{
	frameInviolability = b;
}

bool Demo::Dodge::IsInviolable()
{
	return frameInviolability;
}



void Demo::Dodge::AddPreAddition(const unsigned int& val)
{
	modifier.preAddition.push_back(val);
}

void Demo::Dodge::RemovePreAddition(const unsigned int& val)
{
	for(auto i = modifier.preAddition.begin(); i != modifier.preAddition.end(); ++i)
	{
		if((*i) == val)
		{
			modifier.preAddition.erase(i);
			return;
		}
	}
}

void Demo::Dodge::AddMultiply(const double& val)
{
	modifier.multiply.push_back(val);
}

void Demo::Dodge::RemoveMultiply(const double& val)
{
	for(auto i = modifier.multiply.begin(); i != modifier.multiply.end(); ++i)
	{
		if((*i) == val)
		{
			modifier.multiply.erase(i);
			return;
		}
	}
}

void Demo::Dodge::AddPostAddition(const unsigned int& val)
{
	modifier.postAddition.push_back(val);
}

void Demo::Dodge::RemovePostAddition(const unsigned int& val)
{
	for(auto i = modifier.postAddition.begin(); i != modifier.postAddition.end(); ++i)
	{
		if((*i) == val)
		{
			modifier.postAddition.erase(i);
			return;
		}
	}
}
