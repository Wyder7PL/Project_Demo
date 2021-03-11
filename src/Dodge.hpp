#pragma once

#include "Utility.hpp"

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
	
private:
	
	unsigned int dodge;
	unsigned int untilDodge;

};

}

