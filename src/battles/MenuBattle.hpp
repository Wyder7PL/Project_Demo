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

	virtual void Update(double delta);
	
	BattleChoice Choice();
	
	virtual void CreatePlayerController(std::weak_ptr<Input> ptr) override;
private:
	std::unique_ptr<MenuController> menuCon;
	bool controllerIsSet;
	
};

}

