#pragma once

#include "../Battle.hpp" 
#include "../controllers/TutorialController.hpp"

namespace Demo
{

class TutorialBattle : public Demo::Battle
{
public:
	TutorialBattle();
	virtual ~TutorialBattle();

	virtual void Update(double delta);
	//virtual void Draw(const Canvas& ) const;
	
	virtual void CreatePlayerController(std::weak_ptr<Input> ptr) override;
	
private:
	unsigned int phase;
	std::unique_ptr<TutorialController> tutorialCon;
	bool controllerIsSet;
	double beforeEnd;
};

}

