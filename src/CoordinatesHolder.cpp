#include "CoordinatesHolder.hpp"
#include <algorithm>

void Demo::CoordinatesHolder::Update(double delta)
{
	if(repeatAfter > 0.0 && timeLasting >= repeatAfter)
		timeLasting -= repeatAfter;
	timeLasting += delta;
}

inline bool SortFunction(const std::pair<double,Demo::PointU>& a, std::pair<double,Demo::PointU>& b) 
{
	return a.first < b.first;
}

void Demo::CoordinatesHolder::AddCoordinate(const PointU& coordinate, double timeStamp)
{
	std::pair<double,PointU> newCoord = std::make_pair(timeStamp,coordinate);
	coordinates.push_back(std::move(newCoord));
	
	std::sort(coordinates.begin(),coordinates.end(),SortFunction);
}

void Demo::CoordinatesHolder::SetSequenceRepeat(double repeat)
{
	repeatAfter = repeat;
}

Demo::PointU Demo::CoordinatesHolder::GetCoordinate()
{
	for(unsigned int i = 0; i < coordinates.size(); ++i)
	{
		if(coordinates[i].first <= timeLasting)
		{
			if((i+1 < coordinates.size() && coordinates[i+1].first > timeLasting) || i+1 == coordinates.size())
				return coordinates[i].second;
		}
	}
	
	return PointU(0,0);
}

double Demo::CoordinatesHolder::GetCurrentTime()
{
	return timeLasting;
}

void Demo::CoordinatesHolder::Clear()
{
	timeLasting = 0.0;
	repeatAfter = -1.0;
	coordinates.clear();
}
