#include "Location.hpp"

#include <string>

Demo::Location::Location(const std::string& floor, const unsigned int& x, const unsigned int& y)
:floorIndex(floor), onFloorLocation(x,y)
{
	
}
Demo::Location::Location(const std::string& floor, const PointU& xy)
:floorIndex(floor), onFloorLocation(xy)
{
	
}

Demo::Location::~Location()
{
}

std::string Demo::Location::GetFloor() const
{
	return floorIndex;
}
unsigned int Demo::Location::GetX() const
{
	return onFloorLocation.x;
}
unsigned int Demo::Location::GetY() const
{
	return onFloorLocation.y;
}
Demo::PointU Demo::Location::GetLocationPoint() const
{
	return onFloorLocation;
}
