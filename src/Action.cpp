#include "Action.hpp"

Demo::Action::Action(const Location& l)
:location(l),creatorLocation("",PointU(0,0))
{
}

Demo::Action::~Action()
{
}

Demo::Action::ActionIterator Demo::Action::begin()
{
	ActionIterator iter(this);
	return iter;
}

Demo::Action::ActionIterator Demo::Action::end()
{
	ActionIterator iter(this);
	iter.iter = iter.endIter;
	iter.selfPoint = false;
	delete iter.recursionIter;
	iter.recursionIter = nullptr;
	return iter;
}

void Demo::Action::AddSubaction(Action action)
{
	subactions.push_back(std::move(action));
}






Demo::Action::ActionIterator::ActionIterator(Action* action)
{
	iter = action->subactions.begin();
	endIter = action->subactions.end();
	ptr = action;
	if(action->subactions.size()==0)
		recursionIter = nullptr;
	else
		recursionIter = new ActionIterator(&(*iter));
	
}

Demo::Action::ActionIterator::~ActionIterator()
{
	delete recursionIter;
}

Demo::Action::ActionIterator& Demo::Action::ActionIterator::operator++()
{
	if(selfPoint)
		selfPoint = false;
	else if(recursionIter != nullptr)
	{
		if((*recursionIter) == (*iter).end())
		{
			iter++;
			if(iter == endIter)
			{
				delete recursionIter;
				recursionIter = nullptr;
			}
			else
			{
				delete recursionIter;
				recursionIter = new ActionIterator(&(*iter));
			}
		}
		else{
		++(*recursionIter);
		if(recursionIter->recursionIter == nullptr)
			++(*this);
		}
	}
	return *this;
}

Demo::Action& Demo::Action::ActionIterator::operator*()
{
	if(selfPoint)
		return *ptr;
	return **recursionIter;  
}

bool Demo::Action::ActionIterator::operator==(const ActionIterator& i)
{
	bool areRecursionsNull = (recursionIter == nullptr) || (i.recursionIter == nullptr);
	bool equalAddress = recursionIter == i.recursionIter;
	bool equalIterators = true;
	if(!areRecursionsNull)
		equalIterators = (*recursionIter) == (*(i.recursionIter));
		
	return ptr == i.ptr && iter == i.iter && selfPoint == i.selfPoint && equalAddress && equalIterators;
}

bool Demo::Action::ActionIterator::operator!=(const ActionIterator& i)
{
	return !(*this==i);
}
