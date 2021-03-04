#include "PlayerController.hpp"
#include "../PawnList.hpp"
#include "../graphic/Button.hpp"
#include "../graphic/DefaultAbilityDamageDisplay.hpp"

#include "../effects/DefaultDamageAdsorption.hpp"
#include "../effects/DefaultDamageBooster.hpp"
#include "../effects/Poison.hpp"

Demo::PlayerController::PlayerController(BattleData* data,std::weak_ptr<Input> input)
:Controller(data),inputHandle(input),arrowBegin(0,0),arrowIsUsed(false),attackPawn("",PointU(0,0)),justPressed(true)
{
	data->mousepos.SetPosition(PointU(100,20));
	
	BattlePawn TESTPawn;
	PawnList::GetInstance().OverridePawn("OtterPaladin",TESTPawn);
	TESTPawn.SetAbilityDisplayMode(Ability::DisplayMode::DrawAndActivate);
	data->floors["PlayerFloor"].AddPawnAtLocation(std::move(TESTPawn),PointU(0,0));
	
	BattlePawn TESTPawn1;
	PawnList::GetInstance().OverridePawn("OtterCrossbowman",TESTPawn1);
	TESTPawn1.SetAbilityDisplayMode(Ability::DisplayMode::DrawAndActivate);
	data->floors["PlayerFloor"].AddPawnAtLocation(std::move(TESTPawn1),PointU(1,0));
	
	BattlePawn TESTPawn2;
	PawnList::GetInstance().OverridePawn("OtterBalista",TESTPawn2);
	TESTPawn2.SetAbilityDisplayMode(Ability::DisplayMode::DrawAndActivate);
	data->floors["PlayerFloor"].AddPawnAtLocation(std::move(TESTPawn2),PointU(1,1));

	BattlePawn TESTPawn3;
	PawnList::GetInstance().OverridePawn("OtterApprentice",TESTPawn3);
	TESTPawn3.SetAbilityDisplayMode(Ability::DisplayMode::DrawAndActivate);
	data->floors["PlayerFloor"].AddPawnAtLocation(std::move(TESTPawn3),PointU(0,1));
	
	std::pair<std::string,Button> pair = std::make_pair("BattleQuit",std::move(Button(PointU(300,80))));
	pair.second.UpdateText("Quit");
	pair.second.SetPosition(PointU(1000,600));
	pair.second.SetSize(PointU(300,80));
	pair.second.SetBorderColor(200,0,0,255);
	pair.second.SetColor(100,100,100,255);
	data->buttons.insert(std::move(pair));
	
}

Demo::PlayerController::~PlayerController()
{
}

void Demo::PlayerController::Update(double delta)
{
	if(firstFrame)
	{
		firstFrame = false;
		Location pawn = Location("Enemy",PointU(1,1));
		if(!GetPawn(pawn).IsEmptySpace())
		{
			std::unique_ptr<Poison> ptr = std::make_unique<Poison>();
			GetPawn(pawn).AddEffect(std::move(ptr));
		}
	}
	
	
	if(auto input = inputHandle.lock())
	{
		PointU p((*input).MousePos().x,(*input).MousePos().y);
		
		if(arrowIsUsed)
		{
			if(!(*input).MouseLeft())
			{
				arrowIsUsed = false;
				battleDataPtr->arrow.SetVisibility(false);
				PerformAttack(p);
			}
			else
				UpdateArrow(input);
		}
		
		std::stringstream str;
		str << p.x << " " << p.y;
		
		if(battleDataPtr->buttons["BattleQuit"].IsMouseOnButton(p))
		{
			str << " hovered ";
			if((*input).MouseLeft() && justPressed)
				battleDataPtr->exitBattle = true;
		}
		
		bool mouseOnFloor,mouseOnPawn;
		unsigned int aIndex = 0;
		Location pawn = GetLocationOfPoint(p,mouseOnFloor,mouseOnPawn,aIndex);
		
		if(mouseOnFloor)
		{
			str << " " << pawn.GetFloor();
			if(mouseOnPawn)
			{
				PointU index = pawn.GetLocationPoint();
				str << "\npawn (" << index.x << "," << index.y << ")" << " id: " << GetPawn(pawn).GetID();
					
				str << " " << GetPawn(pawn).GetPosition().x << " " << GetPawn(pawn).GetPosition().y;
					
				if(!arrowIsUsed && (*input).MouseLeft() && !GetPawn(pawn).IsEmptySpace())
				{
					abilityIndex = aIndex;
					attackPawn = pawn;
					if(GetPawn(pawn).IsAbilityDraggable(abilityIndex))
					{
						arrowIsUsed = true;
						battleDataPtr->arrow.SetVisibility(true);
						arrowBegin = GetPawn(pawn).GetPosition(abilityIndex);
						UpdateArrow(input);
					}
					else
						PerformAttack(p);
				}
			}
		}
		battleDataPtr->mousepos.UpdateText(str.str());
		if((*input).MouseLeft() && justPressed)
			justPressed = false;
		if(!(*input).MouseLeft() && !justPressed)
			justPressed = true;
	}
}

void Demo::PlayerController::AttachNewInput(std::weak_ptr<Input> input)
{
	inputHandle = input;
}

void Demo::PlayerController::UpdateArrow(std::shared_ptr<Input> input)
{
	PointU newPosition(0,0);
	newPosition.x = ((*input).MousePos().x + arrowBegin.x)/2;
	newPosition.y = ((*input).MousePos().y + arrowBegin.y)/2;
	battleDataPtr->arrow.SetPosition(newPosition);
	
	PointU newDimensions(0,30);
	newDimensions.x = powl( powl((*input).MousePos().x - (int)arrowBegin.x , 2) + powl((*input).MousePos().y - (int)arrowBegin.y , 2) , 0.5);
	battleDataPtr->arrow.SetSize(newDimensions);
	
	double angle = atan2l(((*input).MousePos().y - (int)arrowBegin.y),((*input).MousePos().x - (int)arrowBegin.x));
	battleDataPtr->arrow.SetRotation(angle / 3.14159 * 180);
}

void Demo::PlayerController::PerformAttack(const PointU& attackLocation)
{
	bool mouseOnFloor,mouseOnPawn;
	unsigned int aIndex = 0;
	Location location = GetLocationOfPoint(attackLocation,mouseOnFloor,mouseOnPawn,aIndex);
		
	if(mouseOnFloor && mouseOnPawn)
	{
		BattlePawn& attacker = GetPawn(attackPawn);
		if(location.GetFloor() == attacker.GetLocation().GetFloor())
		{
			if(location.GetLocationPoint() == attacker.GetLocation().GetLocationPoint())
			{
				if(attacker.IsAbilityDraggable(abilityIndex))
					return;
				attacker.OrderAction(abilityIndex,location,true);
			}
			
			if(GetPawn(location).IsEmptySpace() && abilityIndex == 0)
				attacker.OrderToMove(location);
			else 
				attacker.OrderAction(abilityIndex,location,false);
		}
		else if(!GetPawn(location).IsEmptySpace())
		{
			if(attacker.GetAbilityRange(abilityIndex) != ActionRange::Frontline || location.GetLocationPoint().x == 0)
				attacker.OrderAction(abilityIndex,location,true);
		}
	}
}
