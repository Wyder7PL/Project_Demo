#pragma once

#include "Utility.hpp"

namespace Demo
{

class Location
{
public:
	Location(const std::string& floor, const unsigned int& x, const unsigned int& y);
	Location(const std::string& floor, const PointU& xy);
	~Location();

	std::string GetFloor() const;
	unsigned int GetX() const;
	unsigned int GetY() const;
	PointU GetLocationPoint() const;
	
	private:
	std::string floorIndex;
	PointU onFloorLocation;
};

}

