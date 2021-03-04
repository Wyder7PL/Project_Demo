#include "Floor.hpp"

Demo::Floor::Floor(const PointU& dimension)
:dimensions(dimension)
,location(PointU(0,0))
,size(PointU(0,0))
,offset(PointU(0,0))
,spacing(PointU(0,0))
,pawnSize(PointU(0,0))
,background(PointU(0,0))
,rightOriented(false)
,frontline(true)
,movePawnsAutomatically(true)
{
	SetDimensions(dimension);
	background.SetColor(255,0,0);
}

Demo::Floor::~Floor()
{
}
#include <iostream>
void Demo::Floor::Update(double delta)
{
	damageToDisplay.clear();
	pawnCount = 0;
	frontlinePawnCount = 0;
	bool frontlinePawn;
	for(auto& i : pawns)
	{
		frontlinePawn = true;
		for(auto& j : i)
		{
			if(!j.IsEmptySpace())
			{
				j.Update(delta);
				
				std::vector<int>& pawnDamage = j.GetDamageToDisplay();
				for(int& k : pawnDamage)
				{
					DamageDisplay dd(std::to_string(k));
					PointU newPos = j.GetPosition();
					newPos.x += RandomInt<unsigned int>(0,pawnSize.x) - pawnSize.x/2; 
					dd.SetPosition(newPos);
					damageToDisplay.push_back(std::move(dd));
					
				}
				pawnDamage.clear();
				
				++pawnCount;
				if(frontlinePawn)
					++frontlinePawnCount;
			}
			frontlinePawn = false;
		}
	}
	if(movePawnsAutomatically)
		MovePawnsTowardFront();
}

void Demo::Floor::Draw(const Canvas& canvas) const
{
	background.Draw(canvas);
	for(auto& i : pawns)
	{
		for(auto& j : i)
		{
			if(!j.IsEmptySpace())
				j.Draw(canvas);
		}
	}
}

void Demo::Floor::SetName(const std::string& name)
{
	floorName = name;
}

std::string Demo::Floor::GetName()
{
	return floorName;
}

void Demo::Floor::SetDimensions(const PointU& d)
{
	dimensions = d;
	pawns.resize(d.y);
	for(auto& a : pawns)
	{
		a.resize(d.x);
	}
	UpdatePawnSizeAndPosition();
}
Demo::PointU Demo::Floor::GetDimensions()
{
	return dimensions;
}

void Demo::Floor::AddPawnAtLocation(BattlePawn pawn, const PointU& location)
{
	pawns.at(location.y).at(location.x) = std::move(pawn);
	pawns.at(location.y).at(location.x).SetLocation(Location(GetName(),location));
	UpdatePawnSizeAndPosition();
}

Demo::BattlePawn& Demo::Floor::GetPawnAtLocation(const PointU& location)
{
	return pawns.at(location.y).at(location.x);
}

Demo::BattlePawn* Demo::Floor::GetPawnById(const unsigned long& pawnID)
{
	for(auto& i : pawns)
	{
		for(auto& j : i)
		{
			if(j.GetID() == pawnID)
			{
				return &j;
			}
		}
	}
	return nullptr;
}

Demo::BattlePawn* Demo::Floor::GetRandomPawn()
{
	if(pawnCount == 0)
		return nullptr;
	unsigned int x = RandomInt<unsigned int>(0,dimensions.x);
	unsigned int y = RandomInt<unsigned int>(0,dimensions.y);
	unsigned int overloopControl = 0;
	do{
		BattlePawn& pawn = GetPawnAtLocation(PointU(x,y));
		if(!pawn.IsEmptySpace())
			return &pawn;
		++overloopControl;
	}while(overloopControl < 5000);
	return nullptr;
}

Demo::BattlePawn* Demo::Floor::GetRandomPawnOnFrontline()
{
	if(frontline == false)
		return GetRandomPawn();
	
	if(frontlinePawnCount == 0)
		return nullptr;
	unsigned int y = RandomInt<unsigned int>(0,dimensions.y);
	unsigned int overloopControl = 0;
	do{
		BattlePawn& pawn = GetPawnAtLocation(PointU(0,y));
		if(!pawn.IsEmptySpace())
			return &pawn;
		++overloopControl;
	}while(overloopControl < 5000);
	return nullptr;
}

std::vector<Demo::Action> Demo::Floor::ExtractActionsFromAllPawns()
{
	std::vector<Demo::Action> result;
	for(auto& i : pawns)
	{
		for(auto& j : i)
		{
			auto actions = j.GetActions();
			for(auto& k : actions)
				result.push_back(std::move(k));
		}
	}
	return result;
}

std::vector<Demo::DamageDisplay>& Demo::Floor::GetDamageToDisplay()
{
	return damageToDisplay;
}

void Demo::Floor::SetLocation(const PointU& val)
{
	location = val;
	background.SetPosition(PointU(val.x,val.y));
	UpdatePawnSizeAndPosition();
}
void Demo::Floor::SetSize(const PointU& val)
{
	size = val;
	background.SetSize(PointU(size.x,size.y));
	SetLocation(location);
	UpdatePawnSizeAndPosition();
}
void Demo::Floor::SetOffset(const PointU& val)
{
	offset = val;
	UpdatePawnSizeAndPosition();
}
void Demo::Floor::SetSpacing(const PointU& val)
{
	spacing = val;
	UpdatePawnSizeAndPosition();
}

void Demo::Floor::UpdatePawnSize()
{
	pawnSize.x = ( size.x - 2*offset.x - spacing.x*(dimensions.x - 1) ) / dimensions.x;
	pawnSize.y = ( size.y - 2*offset.y - spacing.y*(dimensions.y - 1) ) / dimensions.y;
	for(auto& i : pawns)
		for(auto& j : i)
			j.SetSize(pawnSize);
}

void Demo::Floor::UpdatePawnPosition()
{
	unsigned int y = 0;
	for(auto& i : pawns)
	{
		unsigned int x = 0;
		for(auto& j : i)
		{
			if(rightOriented)
			{
				PointU p00(location.x + size.x/2 - offset.x,location.y - size.y/2 + offset.y);
				PointU newPosition(p00.x - x*(pawnSize.x+spacing.x) - pawnSize.x/2 , p00.y + y*(pawnSize.y+spacing.y) + pawnSize.y/2 );
				j.SetPosition(newPosition);
			}
			else{
				PointU p00(location.x - size.x/2 + offset.x,location.y - size.y/2 + offset.y);
				PointU newPosition(p00.x + x*(pawnSize.x+spacing.x) + pawnSize.x/2 , p00.y + y*(pawnSize.y+spacing.y) + pawnSize.y/2 );
				j.SetPosition(newPosition);
			}
			x++;
		}
		y++;
	}
}
void Demo::Floor::UpdatePawnSizeAndPosition()
{
	UpdatePawnSize();
	UpdatePawnPosition();
}

bool Demo::Floor::IsPointOnFloor(const PointU& point)
{
	if(point.x >= location.x-size.x/2 && point.x <= location.x+size.x/2)
		if(point.y >= location.y-size.y/2 && point.y <= location.y+size.y/2)
			return true;
	return false;
}

bool Demo::Floor::IsPointOnPawn(const PointU& point)
{
	if(IsPointOnFloor(point))
	{
		PointI clear(point.x,point.y);
		clear.x -= offset.x+location.x;
		clear.y -= offset.y+location.y;
		
		clear.x += size.x/2;
		clear.y += size.y/2;
		
		if(pawnSize.x+spacing.x == 0 || pawnSize.y+spacing.y == 0)
			return false;
		
		if(clear.x < 0 || clear.y < 0 || clear.x > (int)(size.x-2*offset.x) || clear.y > (int)(size.y-2*offset.y))
			return false;
		
		if(clear.x % (pawnSize.x+spacing.x) <= pawnSize.x)
			if(clear.y % (pawnSize.y+spacing.y) <= pawnSize.y)
				return true;
	}
	return false;
}

bool Demo::Floor::IsPointOnAbility(const PointU& point,unsigned int& abilityIndex)
{
	abilityIndex = 0;
	if(IsPointOnFloor(point))
	{
		for(auto& i : pawns)
			for(auto& j : i)
			{
				unsigned int index = 0;
				bool onAbility = j.IsPointOnAbility(point,index);
				if(onAbility)
				{
					abilityIndex = index;
					return true;
				}
			}
	}
	return false;
}

Demo::PointU Demo::Floor::PawnIndexOnPoint(const PointU& point)
{
	PointI clear(point.x,point.y);
	clear.x -= offset.x+location.x;
	clear.y -= offset.y+location.y;
	
	clear.x += size.x/2;
	clear.y += size.y/2;
	
	if(pawnSize.x+spacing.x == 0 || pawnSize.y+spacing.y == 0)
		return PointU(0,0);
	
	if(rightOriented)
		return PointU(dimensions.x-1 - clear.x/(pawnSize.x+spacing.x), clear.y/(pawnSize.y+spacing.y));
	
	return PointU(clear.x/(pawnSize.x+spacing.x),clear.y/(pawnSize.y+spacing.y));
}

void Demo::Floor::SetRightOriented(const bool& side)
{
	rightOriented = side;
	UpdatePawnSizeAndPosition();
}

void Demo::Floor::SetMovePawnsFrontAutomatically(const bool& yes)
{
	movePawnsAutomatically = yes;
}

void Demo::Floor::SetFrontline(const bool& fl)
{
	frontline = fl;
}

bool Demo::Floor::IsFrontline()
{
	return frontline;
}

void Demo::Floor::RemoveDestroyedPawns()
{
	for(auto& i : pawns)
	{
		for(auto& j : i)
		{
			if(j.ToDestroy())
				j.Destroy();
		}
	}
}

void Demo::Floor::DestroyAllPawns()
{
	for(auto& i : pawns)
	{
		for(auto& j : i)
			j.Destroy();
	}
}

void Demo::Floor::MovePawnsTowardFront()
{
	for(unsigned int y = 0; y < dimensions.y; ++y)
	{
		for(unsigned int x = 1; x < dimensions.x; ++x)
		{
			BattlePawn& pawnToMove = GetPawnAtLocation(PointU(x,y));
			BattlePawn& spaceToMove = GetPawnAtLocation(PointU(x-1,y));
			if(!pawnToMove.IsEmptySpace() && spaceToMove.IsEmptySpace())
				pawnToMove.OrderToMove(Location(floorName, PointU(x-1,y)),true);
		}
	}
}
