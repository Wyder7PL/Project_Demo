#pragma once

#include "../BattleData.hpp"

namespace Demo
{


// 
class Controller
{
public:
	explicit Controller(BattleData* data);
	virtual ~Controller();
	
	virtual void Update(double delta);
protected:
	
	void AddPawn(BattlePawn pawn,const Location& location);
	BattlePawn& GetPawn(const Location& location);
	
	Location GetLocationOfPoint(const PointU& point, bool& onFloor, bool& onPawn, unsigned int& abilityIndex);
	
//private:
	BattleData* battleDataPtr;
private:

	unsigned int TESTint = 0;
};

}
