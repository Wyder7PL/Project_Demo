#include "battles/MenuBattle.hpp"
#include "graphic/Window.hpp"
#include "input/PlayerInput.hpp"
#include "effects/EffectList.hpp"
#include "graphic/TextureHolder.hpp"
#include "PawnList.hpp"

#include <chrono>
#include <memory>

int main()
{
	Demo::Window window(Demo::PointU(1280,720),"TEST");
	
	Demo::TextureHolder::GetInstance().LoadDefault();
	Demo::EffectList::GetInstance().LoadDefault();
	Demo::PawnList::GetInstance().LoadDefault();
	
	Demo::Canvas canvas1 = window.CreateCanvas();
	
	std::shared_ptr<Demo::PlayerInput> input = std::make_shared<Demo::PlayerInput>();

	Demo::MenuBattle menu;
	menu.SetFramerate(60);
	menu.CreatePlayerController(input);
	
	auto lastTick = std::chrono::steady_clock::now();
	double drawRateControll = 0.0;
	
	while(!(*input).Escape() && !(*input).Exit() && !menu.BattleEnded())
	{
		auto currentTick = std::chrono::steady_clock::now();
		double delta = std::chrono::duration_cast<std::chrono::duration<double>>(currentTick - lastTick).count();
		lastTick = currentTick;
		drawRateControll += delta;
		
		window.Clear();
		(*input).UpdateInput(window);
		
		menu.Update(delta);
		
		if(drawRateControll >= 0.016)
		{
			menu.Draw(canvas1);
			window.Display();
			drawRateControll -= 0.016;
		}
	}
	return 0;
}
