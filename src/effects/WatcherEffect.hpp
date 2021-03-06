#pragma once

#include "../Location.hpp"
#include "../BattleData.hpp"

namespace Demo
{

class WatcherEffect
{
public:
	WatcherEffect(BattleData* data, const std::string& effectName);
	~WatcherEffect();

	virtual void WatcherUpdate(const double& delta) = 0;
	
	bool ToDestroy();
	
	void SetDestination(const Location&);
	void SetCreatorLocation(const Location&);
protected:
	
	void SetToDestroy();
	
	BattleData* battleDataPtr;

	Location destination;
	Location creator;
private:
	const std::string name;
	bool toDestroy = false;
};

}

