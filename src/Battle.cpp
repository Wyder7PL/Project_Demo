#include "Battle.hpp"
#include "controllers/PlayerController.hpp"

#include "effects/EffectList.hpp"
#include "PawnList.hpp"

#include <algorithm>

Demo::Battle::Battle()
:fps(30)
,updateCooldown(0.032)
,lastTick(0.0)
,lastUpdateCall(0.0)
,drawRate(30)
{
	data.mousepos.UpdateText("NO TEXT");
	
	data.arrow.SetVisibility(false);
	data.arrow.SetColor(20,255,20);
}

Demo::Battle::~Battle()
{
	
}

void Demo::Battle::Update(double delta)
{
	
	lastUpdateCall += delta;
	if(lastUpdateCall > lastTick + updateCooldown)
	{
		double updateDelta = lastUpdateCall-lastTick;
		
		for(auto& i : controllers)
			(*i).Update(updateDelta);
			
		for(auto i = data.floors.begin();i != data.floors.end();i++)
			(*i).second.Update(updateDelta);
		
		UpdateWatchers(delta);
		
		UpdateDamageDisplay(updateDelta);
		
		ExtractActionsFromPawns();
		ManageActions();
		RemoveDestroyedPawns();
		
		lastTick += updateCooldown;
	}
}

void Demo::Battle::Draw(const Canvas& canvas) const
{
	for(auto& i : data.backgroundGraphics)
		i.second->Draw(canvas);
	
	for(auto i = data.floors.cbegin();i != data.floors.cend();i++)
		(*i).second.Draw(canvas);
	
	for(auto& i : data.buttons)
		i.second.Draw(canvas);
	
	for(auto& i : data.messages)
		i.second.Draw(canvas);
	
	for(auto& i : data.graphics)
		i.second->Draw(canvas);
	
	for(auto& i : data.damageDisplay)
		i.Draw(canvas);
	
	data.mousepos.Draw(canvas);
	data.arrow.Draw(canvas);
}

void Demo::Battle::SetFramerate(const unsigned int& f)
{
	fps = f;
	if(f != 0)
		updateCooldown = 1.0 / (double)f;
	else
		updateCooldown = 0.0;
}
void Demo::Battle::AddPawn(BattlePawn pawn,const Location& location)
{
	std::string loc = location.GetFloor();
	data.floors.at(loc).AddPawnAtLocation(std::move(pawn),location.GetLocationPoint());
}

Demo::BattlePawn& Demo::Battle::GetPawn(const Location& location)
{
	return data.floors.at(location.GetFloor()).GetPawnAtLocation(location.GetLocationPoint());
}

void Demo::Battle::CreatePlayerController(std::weak_ptr<Input> ptr)
{
	controllers.push_back(std::make_unique<PlayerController>(&data,ptr));
}

bool Demo::Battle::BattleEnded()
{
	return data.exitBattle;
}

void Demo::Battle::UpdateDamageDisplay(const double& delta)
{
	std::deque<DamageDisplay>& dd = data.damageDisplay;
	
	for(auto& i : data.floors)
	{
		for(auto& yy : i.second.GetDamageToDisplay())
		{
			dd.push_back(yy);
		}
	}
	
	for(auto& i : dd){
		i.Update(delta);
	}
	while(dd.size() > 0 && dd.front().ToDestroy())
		dd.pop_front();
}

void Demo::Battle::UpdateWatchers(const double& delta)
{
	for(auto i = watchers.begin(); i != watchers.end();)
	{
		if((*i)->ToDestroy())
			i = watchers.erase(i);
		else
		{
			(*i)->WatcherUpdate(delta);
			++i;
		}
	}
}

void Demo::Battle::ExtractActionsFromPawns()
{
	for(auto& i : data.floors)
	{
		for(auto& j : i.second.ExtractActionsFromAllPawns())
			pawnActions.push_back(std::move(j));
	}
}

void Demo::Battle::ManageActions()
{
	for(auto& i : pawnActions)
	{
		for(auto j = i.begin(); j != i.end(); ++j)
		{
			Action& action = (*j);
			if( action.aRange == ActionRange::Watcher)
				CreateWatcherFromAction(action);
			
			if( action.aRange == ActionRange::Point || action.aRange == ActionRange::Frontline || action.aRange == ActionRange::Self)
				ManageSingleAction(action);
			
			else if(IsActionAreaEffect(action))
				ManageAreaEffectActions(action);
			
			else if( action.aRange == ActionRange::AllOnFloorExceptCaster || action.aRange == ActionRange::AllOnFloor)
			{
				if(data.floors.count(action.creatorLocation.GetFloor()) > 0)
				{
					Floor& flo = data.floors[action.creatorLocation.GetFloor()];
					for(unsigned int y = 0; y < flo.GetDimensions().y; ++y)
						for(unsigned int x = 0; x < flo.GetDimensions().x; ++x)
						{
							PointU tempLocation = PointU(x,y);
							if(action.aRange == ActionRange::AllOnFloorExceptCaster 
								&& action.creatorLocation.GetLocationPoint() == tempLocation)
									continue;
							Action temp = action;
							temp.location = Location(action.creatorLocation.GetFloor(),tempLocation);
							ManageSingleAction(temp);
						}
				}
			}
		}
	}
	pawnActions.clear();
}

void Demo::Battle::ManageSingleAction(const Action& action)
{
	switch(action.aType)
	{
		case Action::ActionType::Default:
		{
			if(action.aIntention != ActionIntentions::Support)
				DealDamageToPawn(action.actionData,action.location);
			else
				GiveSupportToPawn(action.actionData,action.location);
			break;
		}
		case Action::ActionType::ApplyEffect:
		{
			if( (action.aIntention == ActionIntentions::Support) == (action.location.GetFloor() == action.creatorLocation.GetFloor()))
				ApplyEffectToPawn(action.actionData,action.location);
			break;
		}
		case Action::ActionType::Move:
		{
			MovePawn(action.actionData,action.creatorLocation,action.location);
			break;
		}
		case Action::ActionType::Create:
		{
			if(!CreatePawnAction(action.actionData,action.location))
			{
				BattlePawn* p = data.floors[action.creatorLocation.GetFloor()].GetPawnById(action.creatorIndex);
				if(p != nullptr)
					p->SendAbilityCallBack(CallBack::Fail,action.actionData);
			}
		}
		default: 
			break;
		
	}
}

void Demo::Battle::ManageAreaEffectActions(const Action& action)
{
	std::vector<PointI> points;
	PointU shiftedPoint(0,0);
	
	if(IsAreaEffectHittingPointedLocation(action))
		ManageSingleAction(action);
	
	switch(action.aRange)
	{
		case ActionRange::Plus:
		{
			points = { PointI(-1,0), PointI(0,-1), PointI(1,0), PointI(0,1) };
			break;
		}
		case ActionRange::X:
		{
			points = { PointI(-1,-1), PointI(1,-1), PointI(1,1), PointI(-1,1) };
			break;
		}
		case ActionRange::Line:
		{
			for(unsigned int i = 0;i < data.floors[action.location.GetFloor()].GetDimensions().x; ++i)
				points.push_back(PointI(i, action.location.GetY()));
			break;
		}
		default:
			break;
	}
	for(auto& i : points)
	{
		if( IsInFloorRange(action.location,i,shiftedPoint) )
		{
			Action temp = action;
			temp.location = Location(action.location.GetFloor(), PointU(shiftedPoint.x,shiftedPoint.y));
			ManageSingleAction(temp);
		}
	}
}

void Demo::Battle::CreateWatcherFromAction(const Action& action)
{
	std::istringstream stream;
	stream.str(action.actionData);
	
	std::string watcherName;
	std::vector<double> args;
	
	char separator = '|';
	while(stream.good() && !stream.eof() && separator == '|')
	{
		stream >> watcherName;
		
		char ch = ' ';
		while(stream.get(ch))
		{
			if(ch == ' ') continue;
			if(isdigit(ch) || ch == '.')
			{
				stream.putback(ch);
				double d;
				stream >> d;
				args.push_back(std::move(d));
				continue;
			}
			if(ch == separator)
				break;
		}
	}
	std::vector<std::unique_ptr<WatcherEffect>> vec = EffectList::GetInstance().CreateWatcherEffect(watcherName,&data,args);
	for(auto& i : vec)
	{
		i->SetDestination(action.location);
		i->SetCreatorLocation(action.creatorLocation);
		watchers.push_back(std::move(i));
	}
}

void Demo::Battle::RemoveDestroyedPawns()
{
	for(auto& i : data.floors)
		i.second.RemoveDestroyedPawns();
}


bool Demo::Battle::IsActionAreaEffect(const Action& action)
{
	switch(action.aRange)
	{
		case ActionRange::Plus: case ActionRange::X: case ActionRange::Line:
			return true;
		default:
			return false;
	}
	return false;
}

bool Demo::Battle::IsAreaEffectHittingPointedLocation(const Action& action)
{
	switch(action.aRange)
	{
		case ActionRange::Plus: case ActionRange::X:
			return true;
		default:
			return false;
	}
	return false;
}

bool Demo::Battle::IsInFloorRange(const Location& actionLocation, const PointI& shift, PointU& newPoint)
{
	PointU floorDim = data.floors[actionLocation.GetFloor()].GetDimensions();
	PointI res((int)actionLocation.GetX() + shift.x, (int)actionLocation.GetY() + shift.y);
	if( res.x >= 0 && res.x < (int)floorDim.x && res.y >= 0 && res.y < (int)floorDim.y )
	{
		newPoint.x = res.x;
		newPoint.y = res.y;
		return true;
	}
	return false;
}

void Demo::Battle::DealDamageToPawn(const std::string& attackData, const Location& attackLocation)
{
	BattlePawn& pawn = GetPawn(attackLocation);
	std::istringstream stream;
	stream.str(attackData);
	
	std::string hpType;
	int amount;
	char separator = '|';
	while(stream.good() && !stream.eof() && separator == '|')
	{
		stream >> hpType >> amount >> separator;
		pawn.DealDamage(hpType,amount);
	}
}

void Demo::Battle::GiveSupportToPawn(const std::string& supportData, const Location& supportLocation)
{
	BattlePawn& pawn = GetPawn(supportLocation);
	std::istringstream stream;
	stream.str(supportData);
	
	std::string hpType, supportFlag;
	int amount;
	char separator = '|';
	while(stream.good() && !stream.eof() && separator == '|')
	{
		stream >> hpType >> amount >> separator;
		pawn.GiveSupport(hpType,amount);
	}
}

void Demo::Battle::ApplyEffectToPawn(const std::string& effectData, const Location& effectLocation)
{
	BattlePawn& pawn = GetPawn(effectLocation);
	std::istringstream stream;
	stream.str(effectData);
	
	std::string effectType;
	std::vector<double> args;
	char separator = '|';
	while(stream.good() && !stream.eof() && separator == '|')
	{
		stream >> effectType;
		char ch = ' ';
		while(stream.get(ch))
		{
			if(ch == ' ') continue;
			if(isdigit(ch) || ch == '.')
			{
				stream.putback(ch);
				double d;
				stream >> d;
				args.push_back(std::move(d));
				continue;
			}
			if(ch == separator)
				break;
		}
	}
	std::vector<std::unique_ptr<Demo::Effect>> vec = EffectList::GetInstance().CreateEffect(effectType,args);
	for(auto& i : vec)
	{
		BattleEffect* be = dynamic_cast<BattleEffect*>(i.release());
		std::unique_ptr<BattleEffect> bep(be);
		pawn.AddEffect(std::move(bep));
		
	}
}

void Demo::Battle::MovePawn(const std::string& data, const Location& from, const Location& to, bool forced)
{
	if(!GetPawn(from).IsEmptySpace() && GetPawn(to).IsEmptySpace())
	{
		BattlePawn pawn = GetPawn(from);
		AddPawn(std::move(pawn),to);
		GetPawn(from).Destroy();
	}
}

bool Demo::Battle::CreatePawnAction(const std::string& data, const Location& creationLocation)
{
	if(GetPawn(creationLocation).IsEmptySpace())
	{
		std::istringstream stream;
		stream.str(data);
		std::string pawnName;
		stream >> pawnName;
		
		BattlePawn pawn;
		PawnList::GetInstance().OverridePawn(pawnName,pawn);
		if(!pawn.IsEmptySpace())
		{
			AddPawn(std::move(pawn),creationLocation);
			return true;
		}
	}
	return false;
}
