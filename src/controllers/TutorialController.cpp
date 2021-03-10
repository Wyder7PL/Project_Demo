#include "TutorialController.hpp"


Demo::TutorialController::TutorialController(BattleData* data, std::weak_ptr<Input> input)
:Controller(data)
,inputHandle(input)
,tutorialPhase(TPhase::WithIsYours)
,mouseControllArrow(false)
,fakeMousePressed(false)
,justPressed(true)
,buttonLocked(false)
,arrowBegin(0,0)
,arrowIsUsed(false)
,attackPawn("",PointU(0,0))
,endOfTutorial(false)
{
	std::pair<std::string,Button> button = std::make_pair("Next",std::move(Button(PointU(300,80))));
	button.second.UpdateText("Next");
	button.second.SetPosition(PointU(1000,600));
	button.second.SetSize(PointU(300,80));
	button.second.SetBorderColor(200,0,0,255);
	button.second.SetColor(100,100,100,255);
	data->buttons.insert(std::move(button));
	
	Text message("Welcome in this simple tutorial, main goal of\n this game is to eliminate all enemy warriors, \npress \"next\" button to continue learning ",26);
	message.SetPosition(PointU(400,630));
	data->messages["Message"] = std::move(message);
	
	Text yours("Here are your warriors",32);
	yours.SetPosition(PointU(350,350));
	data->messages["Yours"] = std::move(yours);
	
	Text theirs("Here are your opponents",32);
	theirs.SetPosition(PointU(950,350));
	data->messages["Theirs"] = std::move(theirs);
	
	BattlePawn TESTPawn;
	PawnList::GetInstance().OverridePawn("OtterWarrior",TESTPawn);
	data->floors["Player"].AddPawnAtLocation(std::move(TESTPawn),PointU(0,0));
	
	BattlePawn TESTPawn1;
	PawnList::GetInstance().OverridePawn("OtterMedic",TESTPawn1);
	data->floors["Player"].AddPawnAtLocation(std::move(TESTPawn1),PointU(1,0));
	
	for( unsigned int i = 0; i < 3; ++i)
	{
		BattlePawn EnemyPawn;
			if(PawnList::GetInstance().OverridePawn("OtterArcher",EnemyPawn))
				data->floors["Enemy"].AddPawnAtLocation(std::move(EnemyPawn),PointU(i,0));
	}
	
	for( unsigned int i = 0; i < 2; ++i)
	{
		BattlePawn EnemyPawn;
			if(PawnList::GetInstance().OverridePawn("OtterSwordman",EnemyPawn))
				data->floors["Enemy"].AddPawnAtLocation(std::move(EnemyPawn),PointU(i,1));
	}
	
}

Demo::TutorialController::~TutorialController()
{
}

void Demo::TutorialController::Update(double delta)
{
	if(auto input = inputHandle.lock())
	{
		PointU p((*input).MousePos().x,(*input).MousePos().y);
		
		if(battleDataPtr->buttons["Next"].IsMouseOnButton(p))
		{
			if((*input).MouseLeft() && justPressed && !buttonLocked)
				NextPhase();
		}
		
		if(!mouseControllArrow)
			p = mouseCoords.GetCoordinate();
		
		bool mousePressed = (mouseControllArrow && (*input).MouseLeft()) || (!mouseControllArrow && fakeMousePressed);
		
		if(arrowIsUsed)
		{
			if(!mousePressed)
			{
				arrowIsUsed = false;
				battleDataPtr->arrow.SetVisibility(false);
				PerformAttack(p);
			}
			else
				UpdateArrow(p);
		}
		
		bool mouseOnFloor,mouseOnPawn;
		unsigned int aIndex = 0;
		
		
		Location pawn = GetLocationOfPoint(p,mouseOnFloor,mouseOnPawn,aIndex);
		
		if(mouseOnFloor && mouseOnPawn && pawn.GetFloor() == "Player")
		{
			if(!arrowIsUsed && mousePressed && !GetPawn(pawn).IsEmptySpace())
			{
				abilityIndex = aIndex;
				attackPawn = pawn;
				if(GetPawn(pawn).IsAbilityDraggable(abilityIndex))
				{
					arrowIsUsed = true;
					battleDataPtr->arrow.SetVisibility(true);
					arrowBegin = GetPawn(pawn).GetPosition(abilityIndex);
					UpdateArrow(p);
				}
				else
					PerformAttack(p);
			}
			
		}
		
		
		
		
		
		if((*input).MouseLeft() && justPressed)
			justPressed = false;
		if(!(*input).MouseLeft() && !justPressed)
			justPressed = true;
	}
	PhaseUpdates(delta);
}

void Demo::TutorialController::UpdateArrow(const PointU& mousePos)
{
	PointU newPosition(0,0);
	newPosition.x = (mousePos.x + arrowBegin.x)/2;
	newPosition.y = (mousePos.y + arrowBegin.y)/2;
	battleDataPtr->arrow.SetPosition(newPosition);
	
	PointU newDimensions(0,30);
	newDimensions.x = powl( powl((int)mousePos.x - (int)arrowBegin.x , 2) + powl((int)mousePos.y - (int)arrowBegin.y , 2) , 0.5);
	battleDataPtr->arrow.SetSize(newDimensions);
	
	double angle = atan2l(((int)mousePos.y - (int)arrowBegin.y),((int)mousePos.x - (int)arrowBegin.x));
	battleDataPtr->arrow.SetRotation(angle / 3.14159 * 180);
}

void Demo::TutorialController::PerformAttack(const PointU& attackLocation)
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

Demo::TPhase Demo::TutorialController::GetPhase()
{
	return tutorialPhase;
}

bool Demo::TutorialController::TutorialEnded()
{
	return endOfTutorial;
}

void Demo::TutorialController::NextPhase()
{
	ClearPhase();
	switch(tutorialPhase)
	{
		case TPhase::WithIsYours: LoadSecondPhase(); break;
		case TPhase::DragginBeggin: LoadThirdPhase(); break;
		case TPhase::NowYouTry: LoadFourthPhase(); break;
		case TPhase::Healing: LoadFifthPhase(); break;
		case TPhase::Shielding: LoadSixthPhase(); break;
		case TPhase::Positioning: LoadSeventhPhase(); break;
		case TPhase::Effecting: LoadEighthPhase(); break;
		case TPhase::AnotherTry: endOfTutorial = true; break;
		
		
	}
}

void Demo::TutorialController::ClearPhase()
{
	for(auto& i : battleDataPtr->floors)
		i.second.DestroyAllPawns();
	battleDataPtr->messages["Message"].UpdateText("");
	battleDataPtr->messages["Yours"].UpdateText("");
	battleDataPtr->messages["Theirs"].UpdateText("");
}



void Demo::TutorialController::LoadSecondPhase()
{
	tutorialPhase = TPhase::DragginBeggin;
	mouseControllArrow = false;
	
	battleDataPtr->messages["Message"].UpdateText("To perform an attack, hover mouse over your \nwarrior then press and hold LMB, then drag \nmouse over enemy warrior and release LMB button");
	
	BattlePawn TESTPawn;
	PawnList::GetInstance().OverridePawn("OtterWarrior",TESTPawn);
	battleDataPtr->floors["Player"].AddPawnAtLocation(std::move(TESTPawn),PointU(0,0));
	
	BattlePawn TESTPawn1;
	PawnList::GetInstance().OverridePawn("OtterSwordman",TESTPawn1);
	battleDataPtr->floors["Enemy"].AddPawnAtLocation(std::move(TESTPawn1),PointU(0,0));
	
	PointU pathBegin(450,150), pathEnd(800,160);
	PointI distance( pathEnd.x-pathBegin.x, pathEnd.y-pathBegin.y);
	
	mouseCoords.SetSequenceRepeat(6.0);
	mouseCoords.AddCoordinate(pathBegin,0.0);
	
	unsigned int maxi = 100;
	for(unsigned int i = 0; i < maxi; ++i)
	{
		mouseCoords.AddCoordinate( PointU(pathBegin.x+distance.x*i/maxi, pathBegin.y+distance.y*i/maxi) ,1.0 + i * 0.02);
	}
	
	mouseCoords.AddCoordinate(pathEnd,3.0);
	
	std::unique_ptr<Sprite> spr = std::make_unique<Sprite>();
	TextureHolder::GetInstance().SetSpriteTexture(*spr.get(),"Cursor");
	battleDataPtr->graphics.insert(std::make_pair("Cursor",std::move(spr)));
}

void Demo::TutorialController::LoadThirdPhase()
{
	tutorialPhase = TPhase::NowYouTry;
	mouseControllArrow = true;
	buttonLocked = true;
	arrowIsUsed = false;
	battleDataPtr->arrow.SetVisibility(false);
	
	battleDataPtr->messages["Message"].UpdateText("Now show me you get it");
	
	BattlePawn TESTPawn;
	PawnList::GetInstance().OverridePawn("OtterAxeman",TESTPawn);
	battleDataPtr->floors["Player"].AddPawnAtLocation(std::move(TESTPawn),PointU(0,0));
	
	BattlePawn TESTPawn1;
	PawnList::GetInstance().OverridePawn("OtterSwordman",TESTPawn1);
	TESTPawn1.SetHP("Pain",3);
	battleDataPtr->floors["Enemy"].AddPawnAtLocation(std::move(TESTPawn1),PointU(0,0));
	
	battleDataPtr->graphics["Cursor"]->SetVisibility(false);
	battleDataPtr->buttons["Next"].SetVisibility(false);
}

void Demo::TutorialController::LoadFourthPhase()
{
	tutorialPhase = TPhase::Healing;
	mouseControllArrow = false;
	battleDataPtr->graphics["Cursor"]->SetVisibility(true);
	
	battleDataPtr->messages["Message"].UpdateText("Except basic attacking, some units can\nperform supportive actions to allied units,\nlike medic who can heal allies");
	
	BattlePawn TESTPawn;
	PawnList::GetInstance().OverridePawn("OtterWarrior",TESTPawn);
	battleDataPtr->floors["Player"].AddPawnAtLocation(std::move(TESTPawn),PointU(0,0));
	
	BattlePawn TESTPawn1;
	PawnList::GetInstance().OverridePawn("OtterSwordman",TESTPawn1);
	battleDataPtr->floors["Enemy"].AddPawnAtLocation(std::move(TESTPawn1),PointU(0,0));
	
	BattlePawn TESTPawn2;
	PawnList::GetInstance().OverridePawn("OtterMedic",TESTPawn2);
	battleDataPtr->floors["Player"].AddPawnAtLocation(std::move(TESTPawn2),PointU(0,1));
	
	
	PointU pathBegin(450,450), firstPathEnd(800,160), secondPathEnd(450,150);
	PointI distance1( firstPathEnd.x-pathBegin.x, firstPathEnd.y-pathBegin.y);
	PointI distance2( secondPathEnd.x-pathBegin.x, secondPathEnd.y-pathBegin.y);
	
	mouseCoords.Clear();
	mouseCoords.SetSequenceRepeat(13.0);
	mouseCoords.AddCoordinate(pathBegin,0.0);
	
	int maxi = 100;
	// Cursor goes to enemy
	for(int i = 0; i < maxi; ++i)
		mouseCoords.AddCoordinate( PointU((int)pathBegin.x+distance1.x*i/maxi, (int)pathBegin.y+distance1.y*i/maxi) ,1.0 + i * 0.02);
	mouseCoords.AddCoordinate(firstPathEnd,3.0);
	
	// Cursor goes back
	for(int i = maxi; i > 0; --i)
		mouseCoords.AddCoordinate( PointU((int)pathBegin.x+distance1.x*i/maxi, (int)pathBegin.y+distance1.y*i/maxi) ,4.0 + (maxi-i) * 0.01);
	mouseCoords.AddCoordinate(pathBegin,5.0);
	
	// cursor goes to ally
	for(int i = 0; i < maxi; ++i)
		mouseCoords.AddCoordinate( PointU((int)pathBegin.x+distance2.x*i/maxi, (int)pathBegin.y+distance2.y*i/maxi) ,6.0 + i * 0.02);
	mouseCoords.AddCoordinate(secondPathEnd,8.0);
	
	// Cursor goes back again
	for(int i = maxi; i > 0; --i)
		mouseCoords.AddCoordinate( PointU((int)pathBegin.x+distance2.x*i/maxi, (int)pathBegin.y+distance2.y*i/maxi) ,10.0 + (maxi-i) * 0.01);
	mouseCoords.AddCoordinate(pathBegin,11.0);
	
}

void Demo::TutorialController::LoadFifthPhase()
{
	tutorialPhase = TPhase::Shielding;
	arrowIsUsed = false;
	battleDataPtr->arrow.SetVisibility(false);
	
	battleDataPtr->messages["Message"].UpdateText("Some units can have armor or different\n resistances so some attacks can\nbe not effective");
	
	BattlePawn tankPawn;
	tankPawn.SetSprite("OtterWarrior");
	tankPawn.SetEmptySpace(false);
	
	EnergyBar energy;
	energy.SetMaxEnergy(10); energy.SetEnergy(10);
	tankPawn.AddEnergyBar("Pain",std::move(energy));
	
	auto vec = EffectList::GetInstance().CreateEffect("Adsorption",{2});
	for(auto& i : vec)
		tankPawn.AddEffect(std::move(std::unique_ptr<BattleEffect>(dynamic_cast<BattleEffect*>(i.release()))));
	
	battleDataPtr->floors["Enemy"].AddPawnAtLocation(std::move(tankPawn),PointU(0,0));
	
	BattlePawn TESTPawn;
	PawnList::GetInstance().OverridePawn("OtterSwordman",TESTPawn);
	battleDataPtr->floors["Player"].AddPawnAtLocation(std::move(TESTPawn),PointU(0,0));
	
	BattlePawn TESTPawn1;
	PawnList::GetInstance().OverridePawn("OtterAxeman",TESTPawn1);
	battleDataPtr->floors["Player"].AddPawnAtLocation(std::move(TESTPawn1),PointU(0,1));
	
	PointU pathBegin(450,150), secondPathBegin(450,450), pathEnd(800,160);
	PointI distance1( pathEnd.x-pathBegin.x, pathEnd.y-pathBegin.y);
	PointI distance2( pathEnd.x-secondPathBegin.x, pathEnd.y-secondPathBegin.y);
	
	mouseCoords.Clear();
	mouseCoords.SetSequenceRepeat(13.0);
	mouseCoords.AddCoordinate(pathBegin,0.0);
	
	int maxi = 100;
	// Cursor goes to enemy
	for(int i = 0; i < maxi; ++i)
		mouseCoords.AddCoordinate( PointU((int)pathBegin.x+distance1.x*i/maxi, (int)pathBegin.y+distance1.y*i/maxi) ,1.0 + i * 0.02);
	mouseCoords.AddCoordinate(pathEnd,3.0);
	
	// Cursor goes to ally 
	for(int i = maxi; i > 0; --i)
		mouseCoords.AddCoordinate( PointU((int)secondPathBegin.x+distance2.x*i/maxi, (int)secondPathBegin.y+distance2.y*i/maxi) ,4.0 + (maxi-i) * 0.01);
	mouseCoords.AddCoordinate(secondPathBegin,5.0);
	
	// cursor goes to enemy again
	for(int i = 0; i < maxi; ++i)
		mouseCoords.AddCoordinate( PointU((int)secondPathBegin.x+distance2.x*i/maxi, (int)secondPathBegin.y+distance2.y*i/maxi) ,6.0 + i * 0.02);
	mouseCoords.AddCoordinate(pathEnd,8.0);
	
	// Cursor goes back to start
	for(int i = maxi; i > 0; --i)
		mouseCoords.AddCoordinate( PointU((int)pathBegin.x+distance1.x*i/maxi, (int)pathBegin.y+distance1.y*i/maxi) ,10.0 + (maxi-i) * 0.01);
	mouseCoords.AddCoordinate(pathBegin,11.0);
}

void Demo::TutorialController::LoadSixthPhase()
{
	tutorialPhase = TPhase::Positioning;
	arrowIsUsed = false;
	battleDataPtr->arrow.SetVisibility(false);
	
	battleDataPtr->messages["Message"].UpdateText(
	"Melee attacks cannot reach unit behind another\nunit, to reach that unit either use unit whom\ncan perform ranged attacks or get rid of\nthe unit in front of targeted unit");
	
	BattlePawn rangedPawn;
	rangedPawn.SetSprite("OtterArcher");
	rangedPawn.SetEmptySpace(false);
	
	EnergyBar energy;
	energy.SetMaxEnergy(10); energy.SetEnergy(10);
	rangedPawn.AddEnergyBar("Pain",std::move(energy));
	
	std::unique_ptr<GenericAbility> ability = std::make_unique<GenericAbility>();
	ability->SetMaxCooldown(2.0); ability->SetCooldown(2.0);
	ability->AddDamageToAbility("Pain 2");
	ability->SetAbilityRange(ActionRange::Point);
	rangedPawn.AddAbility(std::move(std::unique_ptr<Ability>(dynamic_cast<Ability*>(ability.release()))));
	
	battleDataPtr->floors["Player"].AddPawnAtLocation(std::move(rangedPawn),PointU(1,0));
	
	BattlePawn TESTPawn;
	PawnList::GetInstance().OverridePawn("OtterSwordman",TESTPawn);
	battleDataPtr->floors["Player"].AddPawnAtLocation(std::move(TESTPawn),PointU(0,0));
	
	BattlePawn TESTPawn1;
	PawnList::GetInstance().OverridePawn("OtterSwordman",TESTPawn1);
	battleDataPtr->floors["Enemy"].AddPawnAtLocation(std::move(TESTPawn1),PointU(0,0));
	
	BattlePawn TESTPawn2;
	PawnList::GetInstance().OverridePawn("OtterArcher",TESTPawn2);
	battleDataPtr->floors["Enemy"].AddPawnAtLocation(std::move(TESTPawn2),PointU(1,0));
	
	PointU pathBegin(450,150), secondPathBegin(220,200), pathEnd(970,160);
	PointI distance1( pathEnd.x-pathBegin.x, pathEnd.y-pathBegin.y);
	PointI distance2( pathEnd.x-secondPathBegin.x, pathEnd.y-secondPathBegin.y);
	
	mouseCoords.Clear();
	mouseCoords.SetSequenceRepeat(13.0);
	mouseCoords.AddCoordinate(pathBegin,0.0);
	
	int maxi = 100;
	// Cursor goes to enemy
	for(int i = 0; i < maxi; ++i)
		mouseCoords.AddCoordinate( PointU((int)pathBegin.x+distance1.x*i/maxi, (int)pathBegin.y+distance1.y*i/maxi) ,1.0 + i * 0.02);
	mouseCoords.AddCoordinate(pathEnd,3.0);
	
	// Cursor goes to ally 
	for(int i = maxi; i > 0; --i)
		mouseCoords.AddCoordinate( PointU((int)secondPathBegin.x+distance2.x*i/maxi, (int)secondPathBegin.y+distance2.y*i/maxi) ,4.0 + (maxi-i) * 0.01);
	mouseCoords.AddCoordinate(secondPathBegin,5.0);
	
	// cursor goes to enemy again
	for(int i = 0; i < maxi; ++i)
		mouseCoords.AddCoordinate( PointU((int)secondPathBegin.x+distance2.x*i/maxi, (int)secondPathBegin.y+distance2.y*i/maxi) ,6.0 + i * 0.02);
	mouseCoords.AddCoordinate(pathEnd,8.0);
	
	// Cursor goes back to start
	for(int i = maxi; i > 0; --i)
		mouseCoords.AddCoordinate( PointU((int)pathBegin.x+distance1.x*i/maxi, (int)pathBegin.y+distance1.y*i/maxi) ,10.0 + (maxi-i) * 0.01);
	mouseCoords.AddCoordinate(pathBegin,11.0);
}

void Demo::TutorialController::LoadSeventhPhase()
{
	tutorialPhase = TPhase::Effecting;
	arrowIsUsed = false;
	battleDataPtr->arrow.SetVisibility(false);
	
	battleDataPtr->messages["Message"].UpdateText("Many units can use abilities different than default\nattack, and those abilities occur in many varieties, \noffensive, supportive, summoning ect.");
	
	BattlePawn TESTPawn;
	PawnList::GetInstance().OverridePawn("OtterBanner",TESTPawn);
	
	std::unique_ptr<SummonAbility> sAbility = std::make_unique<SummonAbility>();
	sAbility->SetMaxCooldown(14.0); sAbility->SetCooldown(0.0);
	sAbility->SetSummonName("OtterSwordman");
	sAbility->SetSprite("Summon");
	TESTPawn.RemoveAbility(3);
	std::unique_ptr<Ability> ptr(dynamic_cast<Ability*>(sAbility.release()));
	TESTPawn.AddAbility(std::move(ptr),4);
	
	TESTPawn.SetAbilityDisplayMode(Ability::DisplayMode::DrawAndActivate);
	battleDataPtr->floors["Player"].AddPawnAtLocation(std::move(TESTPawn),PointU(0,1));
	
	BattlePawn tankPawn;
	tankPawn.SetSprite("OtterWarrior");
	tankPawn.SetEmptySpace(false);
	
	EnergyBar energy;
	energy.SetMaxEnergy(10); energy.SetEnergy(10);
	tankPawn.AddEnergyBar("Pain",std::move(energy));
	
	auto vec = EffectList::GetInstance().CreateEffect("Adsorption",{2});
	for(auto& i : vec)
		tankPawn.AddEffect(std::move(std::unique_ptr<BattleEffect>(dynamic_cast<BattleEffect*>(i.release()))));
	battleDataPtr->floors["Enemy"].AddPawnAtLocation(std::move(tankPawn),PointU(0,0));
	
	PointU pathBegin1(460,520), pathBegin2(400,520), secondPathBegin(440,200), pathEnd(800,160);
	PointI distance1( secondPathBegin.x-pathBegin1.x, secondPathBegin.y-pathBegin1.y);
	PointI distance2( pathEnd.x-secondPathBegin.x, pathEnd.y-secondPathBegin.y);
	PointI distance3( pathEnd.x-pathBegin2.x, pathEnd.y-pathBegin2.y);
	
	mouseCoords.Clear();
	mouseCoords.SetSequenceRepeat(15.0);
	mouseCoords.AddCoordinate(pathBegin1,0.0);
	
	int maxi = 100;
	// Cursor goes to empty space
	for(int i = 0; i < maxi; ++i)
		mouseCoords.AddCoordinate( PointU((int)pathBegin1.x+distance1.x*i/maxi, (int)pathBegin1.y+distance1.y*i/maxi) ,1.0 + i * 0.01);
	mouseCoords.AddCoordinate(secondPathBegin,2.0);
	
	// Cursor goes from ally to enemy 
	for(int i = 0; i < maxi; ++i)
		mouseCoords.AddCoordinate( PointU((int)secondPathBegin.x+distance2.x*i/maxi, (int)secondPathBegin.y+distance2.y*i/maxi) ,3.0 + i * 0.01);
	mouseCoords.AddCoordinate(pathEnd,4.0);
	
	// Cursor goes back to start
	for(int i = maxi; i > 0; --i)
		mouseCoords.AddCoordinate( PointU((int)pathBegin2.x+distance3.x*i/maxi, (int)pathBegin2.y+distance3.y*i/maxi) ,5.0 + (maxi-i) * 0.02);
	mouseCoords.AddCoordinate(pathBegin2,7.0);
	
	// cursor goes to enemy
	for(int i = 0; i < maxi; ++i)
		mouseCoords.AddCoordinate( PointU((int)pathBegin2.x+distance3.x*i/maxi, (int)pathBegin2.y+distance3.y*i/maxi) ,8.0 + i * 0.01);
	mouseCoords.AddCoordinate(pathEnd,9.0);
	
	// Cursor goes from enemy to ally 
	for(int i = maxi; i > 0; --i)
		mouseCoords.AddCoordinate( PointU((int)secondPathBegin.x+distance2.x*i/maxi, (int)secondPathBegin.y+distance2.y*i/maxi) ,10.0 + (maxi-i) * 0.01);
	mouseCoords.AddCoordinate(secondPathBegin,11.0);
	
	// Cursor goes from ally to enemy 
	for(int i = 0; i < maxi; ++i)
		mouseCoords.AddCoordinate( PointU((int)secondPathBegin.x+distance2.x*i/maxi, (int)secondPathBegin.y+distance2.y*i/maxi) ,12.0 + i * 0.01);
	mouseCoords.AddCoordinate(pathEnd,13.0);
}

void Demo::TutorialController::LoadEighthPhase()
{
	tutorialPhase = TPhase::AnotherTry;
	mouseControllArrow = true;
	buttonLocked = true;
	arrowIsUsed = false;
	battleDataPtr->arrow.SetVisibility(false);
	
	battleDataPtr->graphics["Cursor"]->SetVisibility(false);
	battleDataPtr->buttons["Next"].SetVisibility(false);
	
	battleDataPtr->messages["Message"].UpdateText("Now show me you get it all");
	
	
	BattlePawn rangedPawn;
	rangedPawn.SetSprite("OtterArcher");
	rangedPawn.SetEmptySpace(false);
	{
		EnergyBar energy;
		energy.SetMaxEnergy(10); energy.SetEnergy(10);
		rangedPawn.AddEnergyBar("Pain",std::move(energy));
		std::unique_ptr<GenericAbility> ability = std::make_unique<GenericAbility>();
		ability->SetMaxCooldown(2.0); ability->SetCooldown(2.0);
		ability->AddDamageToAbility("Pain 3");
		ability->SetAbilityRange(ActionRange::Point);
		rangedPawn.AddAbility(std::move(std::unique_ptr<Ability>(dynamic_cast<Ability*>(ability.release()))));
	}
	battleDataPtr->floors["Player"].AddPawnAtLocation(std::move(rangedPawn),PointU(1,0));
	
	BattlePawn bannerPawn;
	PawnList::GetInstance().OverridePawn("OtterBanner",bannerPawn);
	bannerPawn.RemoveAbility(3);
	bannerPawn.RemoveAbility(2);
	bannerPawn.SetAbilityDisplayMode(Ability::DisplayMode::DrawAndActivate);
	battleDataPtr->floors["Player"].AddPawnAtLocation(std::move(bannerPawn),PointU(0,1));
	
	BattlePawn axePawn;
	PawnList::GetInstance().OverridePawn("OtterAxeman",bannerPawn);
	battleDataPtr->floors["Player"].AddPawnAtLocation(std::move(bannerPawn),PointU(0,0));
	
	BattlePawn pawn;
	PawnList::GetInstance().OverridePawn("OtterSwordman",pawn);
	{
		EnergyBar energy;
		energy.SetMaxEnergy(20); energy.SetEnergy(20);
		pawn.AddEnergyBar("Pain",std::move(energy));
	}
	battleDataPtr->floors["Enemy"].AddPawnAtLocation(std::move(pawn),PointU(0,0));
	
	BattlePawn medicPawn;
	PawnList::GetInstance().OverridePawn("OtterMedic",medicPawn);
	{
		medicPawn.RemoveAbility(0);
		std::unique_ptr<GenericAbility> ability = std::make_unique<GenericAbility>();
		ability->SetMaxCooldown(0.5); ability->SetCooldown(0.0);
		ability->AddDamageToAbility("*support Pain 15");
		ability->SetAbilityRange(ActionRange::Point);
		medicPawn.AddAbility(std::move(std::unique_ptr<Ability>(dynamic_cast<Ability*>(ability.release()))));
	}
	battleDataPtr->floors["Enemy"].AddPawnAtLocation(std::move(medicPawn),PointU(1,0));
	
	BattlePawn tankPawn;
	tankPawn.SetSprite("OtterWarrior");
	tankPawn.SetEmptySpace(false);
	{
		EnergyBar energy;
		energy.SetMaxEnergy(10); energy.SetEnergy(3);
		tankPawn.AddEnergyBar("Pain",std::move(energy));
	}
		auto vec = EffectList::GetInstance().CreateEffect("Adsorption",{5});
		for(auto& i : vec)
			tankPawn.AddEffect(std::move(std::unique_ptr<BattleEffect>(dynamic_cast<BattleEffect*>(i.release()))));
	battleDataPtr->floors["Enemy"].AddPawnAtLocation(std::move(tankPawn),PointU(2,0));
}

void Demo::TutorialController::PhaseUpdates(double delta)
{
	switch(tutorialPhase)
	{
		case TPhase::WithIsYours: break;
		case TPhase::DragginBeggin: UpdateSecondPhase(delta); break;
		case TPhase::NowYouTry: UpdateThirdPhase(delta); break;
		case TPhase::Healing: UpdateFourthPhase(delta); break;
		case TPhase::Shielding: UpdateFifthPhase(delta); break;
		case TPhase::Positioning: UpdateSixthPhase(delta); break;
		case TPhase::Effecting: UpdateSeventhPhase(delta); break;
		case TPhase::AnotherTry: UpdateEighthPhase(delta); break;
		
	}
}

void Demo::TutorialController::UpdateSecondPhase(double delta)
{
	mouseCoords.Update(delta);
	fakeMousePressed = mouseCoords.GetCurrentTime() >= 0.5 && mouseCoords.GetCurrentTime() < 3.5;
	battleDataPtr->graphics["Cursor"]->SetPosition(mouseCoords.GetCoordinate());
	
	if(mouseCoords.GetCurrentTime() < 0.5)
		GetPawn(Location("Enemy",PointU(0,0))).SetHP("Pain",5);
}

void Demo::TutorialController::UpdateThirdPhase(double delta)
{
	if(GetPawn(Location("Enemy",PointU(0,0))).IsEmptySpace())
	{
		buttonLocked = false;
		battleDataPtr->buttons["Next"].SetVisibility(true);
	}
}

void Demo::TutorialController::UpdateFourthPhase(double delta)
{
	mouseCoords.Update(delta);
	bool attack = mouseCoords.GetCurrentTime() >= 0.5 && mouseCoords.GetCurrentTime() < 3.5;
	bool support = mouseCoords.GetCurrentTime() >= 5.5 && mouseCoords.GetCurrentTime() < 8.5;
	fakeMousePressed = attack || support;
	
	if(mouseCoords.GetCurrentTime() > 4.5 && mouseCoords.GetCurrentTime() < 5.0)
		GetPawn(Location("Enemy",PointU(0,0))).OrderAction(0,Location("Player",PointU(0,0)));
	
	if(mouseCoords.GetCurrentTime() < 0.5)
		GetPawn(Location("Enemy",PointU(0,0))).SetHP("Pain",10);

	battleDataPtr->graphics["Cursor"]->SetPosition(mouseCoords.GetCoordinate());
	
}

void Demo::TutorialController::UpdateFifthPhase(double delta)
{
	mouseCoords.Update(delta);
	bool attack1 = mouseCoords.GetCurrentTime() >= 0.5 && mouseCoords.GetCurrentTime() < 3.5;
	bool attack2 = mouseCoords.GetCurrentTime() >= 5.5 && mouseCoords.GetCurrentTime() < 8.5;
	fakeMousePressed = attack1 || attack2;

	if(mouseCoords.GetCurrentTime() < 0.5)
		GetPawn(Location("Enemy",PointU(0,0))).SetHP("Pain",15);

	battleDataPtr->graphics["Cursor"]->SetPosition(mouseCoords.GetCoordinate());
}

void Demo::TutorialController::UpdateSixthPhase(double delta)
{
	mouseCoords.Update(delta);
	bool attack1 = mouseCoords.GetCurrentTime() >= 0.5 && mouseCoords.GetCurrentTime() < 3.5;
	bool attack2 = mouseCoords.GetCurrentTime() >= 5.5 && mouseCoords.GetCurrentTime() < 8.5;
	fakeMousePressed = attack1 || attack2;

	if(mouseCoords.GetCurrentTime() < 0.5)
		GetPawn(Location("Enemy",PointU(1,0))).SetHP("Pain",10);

	battleDataPtr->graphics["Cursor"]->SetPosition(mouseCoords.GetCoordinate());
}

void Demo::TutorialController::UpdateSeventhPhase(double delta)
{
	mouseCoords.Update(delta);
	bool summon = mouseCoords.GetCurrentTime() >= 0.5 && mouseCoords.GetCurrentTime() < 2.2;
	bool attack1 = mouseCoords.GetCurrentTime() >= 2.8 && mouseCoords.GetCurrentTime() < 4.5;
	bool effect = mouseCoords.GetCurrentTime() >= 7.5 && mouseCoords.GetCurrentTime() < 9.5;
	bool attack2 = mouseCoords.GetCurrentTime() >= 11.5 && mouseCoords.GetCurrentTime() < 13.5;
	fakeMousePressed = summon || attack1 || effect || attack2;

	if(mouseCoords.GetCurrentTime() < 0.3)
	{
		GetPawn(Location("Enemy",PointU(0,0))).SetHP("Pain",10);
		GetPawn(Location("Player",PointU(0,0))).Destroy();
	}

	battleDataPtr->graphics["Cursor"]->SetPosition(mouseCoords.GetCoordinate());
}

void Demo::TutorialController::UpdateEighthPhase(double delta)
{
	if(GetPawn(Location("Enemy",PointU(0,0))).IsEmptySpace() 
		&& GetPawn(Location("Enemy",PointU(1,0))).IsEmptySpace()
			&& GetPawn(Location("Enemy",PointU(2,0))).IsEmptySpace())
	{
		buttonLocked = false;
		battleDataPtr->buttons["Next"].SetVisibility(true);
	}
	GetPawn(Location("Enemy",PointU(1,0))).OrderAction(0,Location("Enemy",PointU(0,0)),false);
}
