#pragma once

#include "BattlePawn.hpp"
#include "Floor.hpp"
#include "Location.hpp"

#include "graphic/Text.hpp"
#include "graphic/DamageDisplay.hpp"
#include "graphic/Button.hpp"


#include <list>
#include <vector>
#include <memory>
#include <map>
#include <deque>

namespace Demo
{

struct BattleData
{
	//std::list<BattleObject> battlePawns;
	std::map<std::string,Floor> floors;
	std::map<std::string,Button> buttons;
	std::map<std::string,Text> messages;
	std::map<std::string,std::unique_ptr<Graphic>> graphics;
	std::map<std::string,std::unique_ptr<Graphic>> backgroundGraphics;
	std::deque<DamageDisplay> damageDisplay;
	//InputData;
	bool exitBattle = false;
	Text mousepos;
	Bar arrow = Bar(PointU(0,0));
};

}

