#pragma once

#include "Controller.hpp"
#include "../input/Input.hpp"
#include "../CoordinatesHolder.hpp"

#include "../PawnList.hpp"
#include "../effects/EffectList.hpp"
#include "../graphic/Sprite.hpp"
#include "../graphic/TextureHolder.hpp"
#include "../abilities/GenericAbility.hpp"
#include "../abilities/SummonAbility.hpp"

namespace Demo
{
	
enum class TPhase{
	WithIsYours,
	DragginBeggin,
	NowYouTry,
	Healing,
	Shielding,
	Positioning,
	Effecting,
	AnotherTry
};

class TutorialController : public Demo::Controller
{
public:
	TutorialController(BattleData* data,std::weak_ptr<Input> input);
	virtual ~TutorialController();
	
	virtual void Update(double delta);
	
	TPhase GetPhase();
	
	bool TutorialEnded();

private:
	void UpdateArrow(const PointU& mousePos);
	void PerformAttack(const PointU& attackLocation);

	void NextPhase();
	void ClearPhase();
	
	void LoadSecondPhase();
	void LoadThirdPhase();
	void LoadFourthPhase();
	void LoadFifthPhase();
	void LoadSixthPhase();
	void LoadSeventhPhase();
	void LoadEighthPhase();
	
	void PhaseUpdates(double delta);
	
	void UpdateSecondPhase(double delta);
	void UpdateThirdPhase(double delta);
	void UpdateFourthPhase(double delta);
	void UpdateFifthPhase(double delta);
	void UpdateSixthPhase(double delta);
	void UpdateSeventhPhase(double delta);
	void UpdateEighthPhase(double delta);

	std::weak_ptr<Input> inputHandle;
	TPhase tutorialPhase;
	
	CoordinatesHolder mouseCoords;
	bool mouseControllArrow;
	bool fakeMousePressed;
	bool justPressed;
	bool buttonLocked;
	
	PointU arrowBegin;
	bool arrowIsUsed;
	unsigned int abilityIndex = 0;
	Location attackPawn;
	
	bool endOfTutorial;
};

}

