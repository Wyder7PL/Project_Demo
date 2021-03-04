#pragma once

#include "Utility.hpp"

#include <vector>
#include <algorithm>

namespace Demo
{

class CoordinatesHolder
{
public:
	
	void Update(double delta);
	void AddCoordinate(const PointU& coordinate,double timeStamp);
	void SetSequenceRepeat(double repeat);
	PointU GetCoordinate();
	double GetCurrentTime();
	void Clear();
	
private:
	double timeLasting = 0.0;
	double repeatAfter = -1.0;
	std::vector<std::pair<double,PointU>> coordinates;
};

}

