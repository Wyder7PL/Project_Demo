#include "Accuracy.hpp"

Demo::Accuracy::Accuracy(const unsigned int& acc)
:accuracy(acc)
{
}

Demo::Accuracy::~Accuracy()
{
}

void Demo::Accuracy::SetAccuracy(const unsigned int& acc)
{
	accuracy = acc;
}

unsigned int Demo::Accuracy::GetAccuracyPrecise()
{
	if(preciseHitCount == accuracy)
	{
		preciseHitCount = 0;
		return 0;
	}
	else
	{
		++preciseHitCount;
		return GetAccuracy();
	}
}

unsigned int Demo::Accuracy::GetAccuracy()
{
	return accuracy;
}

void Demo::Accuracy::RandomizePreciseHitCount()
{
	preciseHitCount = RandomInt<unsigned int>(0,accuracy);
}
