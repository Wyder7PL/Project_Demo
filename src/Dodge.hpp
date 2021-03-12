#pragma once

#include "Utility.hpp"
#include "effects/DamageModificationInfo.hpp"

namespace Demo
{

class Dodge
{
public:
	Dodge(const unsigned int& maxDodge, const unsigned int& initialDodge = 0);
	virtual ~Dodge();
	
	void SetNewDodge(const unsigned int& amount);
	void SetInitialDodge(const unsigned int& amount);
	
	void IncreaseDodge(const unsigned int& amount);
	
	bool CanDodge();
	
	void ResetDodge();
	
	void RandomizeDodge();
	
	void SetFrameInviolability(const bool&);
	bool IsInviolable();
	
	
	void AddPreAddition(const unsigned int&);
	void RemovePreAddition(const unsigned int&);
	void AddMultiply(const double&);
	void RemoveMultiply(const double&);
	void AddPostAddition(const unsigned int&);
	void RemovePostAddition(const unsigned int&);
	
private:
	
	unsigned int dodge;
	unsigned int untilDodge;
	
	bool frameInviolability = false;
	DamageModificationInfo modifier;
};

}

