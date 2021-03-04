#pragma once

#include "Location.hpp"

#include <list>
#include <string>

namespace Demo
{

	enum class ActionRange
	{
	//	* * * * *			* * * * *			* * * * *			* * * * *
	//	* * * * *			* * O * *			* O * O *			* * * * *
	//	* * X * *			* O X O *			* * X * *			O O O O O
	//	* * * * *			* * O * *			* O * O *			* * * * *
	//	* * * * *			* * * * *			* * * * *			* * * * *
		Point,				Plus,				X,					Line,
		Frontline,
		None,
		Self,
		AllOnFloor,
		AllOnFloorExceptCaster
	};
	
	enum class ActionIntentions
	{
		Attack,
		Support,
		Berserk
	};
	
class Action
{
public:

	enum class ActionType
	{
		None,
		Default,
		ApplyEffect,
		Create,
		Move
	};
	
	class ActionIterator
	{
	public:
		ActionIterator(Action* action);
		~ActionIterator();
	
		ActionIterator& operator++();
		Action& operator*();
		bool operator==(const ActionIterator& i);
		bool operator!=(const ActionIterator& i);
		
		ActionIterator* recursionIter;
		std::list<Action>::iterator iter;
		std::list<Action>::iterator endIter;
		Action* ptr;
		bool selfPoint = true;
	};
	
	explicit Action(const Location&);
	~Action();
	
	ActionIterator begin();
	ActionIterator end();
	
	void AddSubaction(Action action);
	
	Location location;
	Location creatorLocation = Location("",PointU(0,0));
	unsigned long creatorIndex = 0;
	std::list<Action> subactions;
	ActionType aType;
	ActionIntentions aIntention;
	ActionRange aRange = ActionRange::Point;
	std::string actionData;
	bool defaultAction;
	
	
	
	
	
};

}

