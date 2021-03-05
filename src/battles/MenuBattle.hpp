#pragma once

#include "../Battle.hpp" 
#include "../controllers/MenuController.hpp"

namespace Demo
{

class MenuBattle : public Demo::Battle
{
public:
	MenuBattle();
	virtual ~MenuBattle();

	virtual void Update(double delta) override;
	virtual void Draw(const Canvas&) const override;
	
	BattleChoice Choice();
	
	virtual void CreatePlayerController(std::weak_ptr<Input> ptr) override;
		
	virtual bool BattleEnded() override;
	
private:
	void SetNewBattle();

	std::weak_ptr<Input> inputPtr;
	std::unique_ptr<MenuController> menuCon;
	bool controllerIsSet;

	std::unique_ptr<Demo::Battle> actualBattle;
	bool battleSet;
};

}

