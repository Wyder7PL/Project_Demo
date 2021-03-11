#include "WatcherEffect.hpp"

Demo::WatcherEffect::WatcherEffect(BattleData* data, const std::string& effectName)
:battleDataPtr(data)
,destination("",PointU(0,0))
,creator("",PointU(0,0))
,name(effectName)
{
}

Demo::WatcherEffect::~WatcherEffect()
{
}

bool Demo::WatcherEffect::ToDestroy()
{
	return toDestroy;
}

void Demo::WatcherEffect::SetDestination(const Location& l)
{
	destination = l;
}

void Demo::WatcherEffect::SetCreatorLocation(const Location& l)
{
	creator = l;
}

void Demo::WatcherEffect::SetAccuracy(const unsigned int& acc)
{
	accuracy = acc;
}

void Demo::WatcherEffect::SetToDestroy()
{
	toDestroy = true;
}