#include "Dodge.hpp"

Demo::Dodge::Dodge(const unsigned int& maxDodge, const unsigned int& initialDodge)
:dodge(maxDodge)
,untilDodge(initialDodge)
{
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
	return untilDodge >= dodge;
}

void Demo::Dodge::ResetDodge()
{
	if(CanDodge())
		untilDodge -= dodge;
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
