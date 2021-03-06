#pragma once

#include "WatcherEffect.hpp" // Base class: Demo::WatcherEffect

namespace Demo
{

class ElectricChain : public Demo::WatcherEffect
{
public:
	ElectricChain();
	virtual ~ElectricChain();

public:
	virtual
void watcherUpdate(const double& delta);
};

}

