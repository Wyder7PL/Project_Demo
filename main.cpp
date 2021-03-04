#include "src/battles/AlphaBattle.hpp"
#include "src/battles/MenuBattle.hpp"
#include "src/battles/TutorialBattle.hpp"
#include "src/graphic/Window.hpp"
#include "src/input/PlayerInput.hpp"
#include "src/effects/EffectList.hpp"
#include "src/graphic/TextureHolder.hpp"
#include "src/PawnList.hpp"


#include <iostream>
#include <chrono>
#include <memory>
#include <filesystem>

#include "src/Action.hpp"
void ActionTestground();
void PrintFiles();

int main()
{
	//PrintFiles();
	
	Demo::Window window(Demo::PointU(1280,720),"TEST");
	
	Demo::TextureHolder::GetInstance().LoadDefault();
	Demo::EffectList::GetInstance().LoadDefault();
	Demo::PawnList::GetInstance().LoadDefault();
	
	Demo::Canvas canvas1 = window.CreateCanvas();
	
	std::shared_ptr<Demo::PlayerInput> input = std::make_shared<Demo::PlayerInput>();
	
	/*Demo::Battle battle;
	battle.SetFramerate(60);
	battle.TESTCreatePlayerController(input);*/
	
	Demo::MenuBattle menu;
	menu.SetFramerate(60);
	menu.CreatePlayerController(input);
	
	std::unique_ptr<Demo::Battle> battle;
	bool battleSet = false;
	
	
	auto lastTick = std::chrono::steady_clock::now();
	double drawRateControll = 0.0;
	
	while(!(*input).Escape() && !(*input).Exit() && (!battleSet || !battle->BattleEnded()))
	{
		auto currentTick = std::chrono::steady_clock::now();
		double delta = std::chrono::duration_cast<std::chrono::duration<double>>(currentTick - lastTick).count();
		lastTick = currentTick;
		drawRateControll += delta;
		
		window.Clear();
		(*input).UpdateInput(window);
		
		if(battleSet)
			battle->Update(delta);
		else
			menu.Update(delta);
		
		if(drawRateControll >= 0.016)
		{
			if(battleSet)
				battle->Draw(canvas1);
			else
				menu.Draw(canvas1);
			window.Display();
			drawRateControll -= 0.016;
		}
		
		if(!battleSet && menu.Choice() != Demo::BattleChoice::None)
		{
			if(menu.Choice() == Demo::BattleChoice::Alpha)
				battle = std::make_unique<Demo::AlphaBattle>();
			else if(menu.Choice() == Demo::BattleChoice::Tutorial)
				battle = std::make_unique<Demo::TutorialBattle>();
			
			battle->CreatePlayerController(input);
			battle->SetFramerate(60);
			battleSet = true;
		}
	}
	return 0;
}

/*void ActionTestground()
{
	Demo::Location n("",0,0);
	Demo::Action a1(n),a2(n),a3(n),a4(n),a5(n),a6(n),a7(n);
	a1.actionData = "1"; a2.actionData = "2"; a3.actionData = "3"; a4.actionData = "4"; a5.actionData = "5"; a6.actionData = "6"; a7.actionData = "7";
	a3.AddSubaction(std::move(a7));
	a2.AddSubaction(std::move(a3)); a2.AddSubaction(std::move(a4)); a2.AddSubaction(std::move(a5)); 
	a1.AddSubaction(std::move(a2)); a1.AddSubaction(std::move(a6)); //a1.AddSubaction(std::move(a7)); 
	
	for(auto i = a1.begin();i != a1.end(); ++i)
		std::cout << (*i).actionData << " ";
}*/

namespace fs = std::filesystem;
void PrintFiles()
{
	fs::path p = fs::current_path();
	p /= "resources";
	p /= "graphic";
	std::cout << p.relative_path() << "\n";
	for (const auto& entry : fs::directory_iterator(p))
	{
		if(fs::is_regular_file(entry.status()))
		{
			std::cout << entry.path().filename();
		}
	}
}
