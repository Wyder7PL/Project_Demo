#pragma once

#include "Utility.hpp"

namespace Demo
{

class Accuracy
{
public:
	Accuracy(const unsigned int& acc = 10);
	virtual ~Accuracy();
	
	void SetAccuracy(const unsigned int& acc);
	
	unsigned int GetAccuracyPrecise();
	unsigned int GetAccuracy();
	
	void RandomizePreciseHitCount();
	
private:
	
	unsigned int accuracy;
	unsigned int preciseHitCount = 0;
};

}

