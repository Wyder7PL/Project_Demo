#pragma once

#include "WatcherEffect.hpp"

namespace Demo
{

class ElectricChain : public Demo::WatcherEffect
{
public:
	ElectricChain(BattleData* data, const std::vector<double>& args);
	virtual ~ElectricChain();

	virtual void WatcherUpdate(const double& delta) override;
	
private:
	void SetPotentialTargets();
	void ApplyElectricityToPawn(BattlePawn& target);
	
	int electricityVoltage;
	int electricityIntensity;
	unsigned int targetsLeft;
	int initialDamage;
	
	std::vector<PointU> potentialTargets;
	std::vector<PointU> hittedTargets;
};

}
