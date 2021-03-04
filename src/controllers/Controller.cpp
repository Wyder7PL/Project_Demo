#include "Controller.hpp"

Demo::Controller::Controller(BattleData* data)
{
	if(data == nullptr)
		return;//TODO error
	battleDataPtr = data;
}

Demo::Controller::~Controller()
{
}

void Demo::Controller::Update(double delta)
{
	
}

void Demo::Controller::AddPawn(BattlePawn pawn,const Location& location)
{
	std::string loc = location.GetFloor();
	battleDataPtr->floors.at(loc).AddPawnAtLocation(pawn,location.GetLocationPoint());
}

Demo::BattlePawn& Demo::Controller::GetPawn(const Location& location)
{
	return battleDataPtr->floors.at(location.GetFloor()).GetPawnAtLocation(location.GetLocationPoint());
}

Demo::Location Demo::Controller::GetLocationOfPoint(const PointU& point, bool& onFloor, bool& onPawn, unsigned int& abilityIndex)
{
	onFloor = false;
	onPawn = false;
	bool onAbility = false;
	abilityIndex = 0;
	unsigned int aIndex = 0;
	std::string floorIndex = "";
	PointU pawnIndex = PointU(0,0);
	for(auto& i : battleDataPtr->floors)
	{
		if(i.second.IsPointOnFloor(point))
		{
			floorIndex = i.first;
			onFloor = true;
			onAbility = i.second.IsPointOnAbility(point,aIndex);
			if(i.second.IsPointOnPawn(point) || onAbility)
			{
				if(onAbility)
					abilityIndex = aIndex;
				pawnIndex = i.second.PawnIndexOnPoint(point);
				onPawn = true;
				break;
			}
		}
	}
	return Location(floorIndex,pawnIndex);
}
