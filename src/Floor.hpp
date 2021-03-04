#pragma once

#include <vector>

#include "Utility.hpp"
#include "graphic/Canvas.hpp"
#include "graphic/DamageDisplay.hpp"
#include "BattlePawn.hpp"

namespace Demo
{

class Floor
{
public:
	explicit Floor(const PointU& dimension = PointU(3,3));
	virtual ~Floor();
	
	virtual void Update(double delta);
	virtual void Draw(const Canvas&) const;
	
	void SetName(const std::string&);
	std::string GetName();
	
	// Set how many pawns can this floor contain
	void SetDimensions(const PointU& d);
	
	// How many pawns can this floor contain
	PointU GetDimensions();
	
	void SetLocation(const PointU& val);
	
	// Set size of this floor in pixels
	void SetSize(const PointU& val);
	
	// Set distance between pawns and floor borders
	void SetOffset(const PointU& val);
	
	// Set Spacing between pawns
	void SetSpacing(const PointU& val);
	
	void AddPawnAtLocation(BattlePawn pawn,const PointU& location);
	
	BattlePawn& GetPawnAtLocation(const PointU& location);
	
	// Return nullptr if there is no pawn with that id
	BattlePawn* GetPawnById(const unsigned long& pawnID);
	
	// Return nullptr when every pawn has emptySpace set to true
	BattlePawn* GetRandomPawn();
	
	// When frontline is set to true, it return random pawn with x = 0 location
	// or nullptr when every pawn in frontline is set to be emptySpace,
	// otherwise return GetRandomPawn();
	BattlePawn* GetRandomPawnOnFrontline();
	
	std::vector<Action> ExtractActionsFromAllPawns();
	std::vector<DamageDisplay>& GetDamageToDisplay();
	
	bool IsPointOnFloor(const PointU& point);
	bool IsPointOnPawn(const PointU& point);
	bool IsPointOnAbility(const PointU& point,unsigned int& abilityIndex);
	PointU PawnIndexOnPoint(const PointU& point);
	
	void SetRightOriented(const bool& side);
	void SetMovePawnsFrontAutomatically(const bool& yes);
	
	void SetFrontline(const bool&);
	bool IsFrontline();
	
	void RemoveDestroyedPawns();
	void DestroyAllPawns();
	
private:
	
	void UpdatePawnSize();
	void UpdatePawnPosition();
	void UpdatePawnSizeAndPosition();
	
	void MovePawnsTowardFront();
	
	std::string floorName;
	
	std::vector<std::vector<BattlePawn>> pawns;
	PointU dimensions;
	PointU location;
	PointU size;
	PointU offset;
	PointU spacing;
	PointU pawnSize;
	
	std::vector<DamageDisplay> damageToDisplay;
	
	Bar background;
	
	bool rightOriented;
	bool frontline;
	bool movePawnsAutomatically;
	
	unsigned int pawnCount = 0;
	unsigned int frontlinePawnCount = 0;
};

}

