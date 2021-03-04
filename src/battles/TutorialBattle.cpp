#include "TutorialBattle.hpp"

#include "../graphic/TextureHolder.hpp"
#include "../graphic/Sprite.hpp"

Demo::TutorialBattle::TutorialBattle()
:phase(0)
,beforeEnd(5.0)
{
	data.floors["Player"] = Floor();
	data.floors["Player"].SetName("Player");
	data.floors["Player"].SetMovePawnsFrontAutomatically(false);
	data.floors["Player"].SetFrontline(false);
	
	data.floors["Enemy"] = Floor();
	data.floors["Enemy"].SetName("Enemy");
	data.floors["Enemy"].SetMovePawnsFrontAutomatically(true);
	data.floors["Enemy"].SetFrontline(true);

	data.floors["Player"].SetDimensions(PointU(2,2));
	data.floors["Player"].SetSize(PointU(500,500));
	data.floors["Player"].SetLocation(PointU(350,300));
	data.floors["Player"].SetOffset(PointU(30,30));
	data.floors["Player"].SetSpacing(PointU(30,30));
	
	data.floors["Player"].SetRightOriented(true);
	
	data.floors["Enemy"].SetDimensions(PointU(3,3));
	data.floors["Enemy"].SetSize(PointU(500,500));
	data.floors["Enemy"].SetLocation(PointU(950,300));
	data.floors["Enemy"].SetOffset(PointU(50,50));
	data.floors["Enemy"].SetSpacing(PointU(20,20));
	
	std::unique_ptr<Sprite> backgroundSprite = std::make_unique<Sprite>();
	TextureHolder::GetInstance().SetSpriteTexture(*backgroundSprite,"Lake");
	backgroundSprite->SetPosition(PointU(600,200));
	data.backgroundGraphics["Lake"] = std::move(backgroundSprite);
}

Demo::TutorialBattle::~TutorialBattle()
{
}

void Demo::TutorialBattle::Update(double delta)
{
	Battle::Update(delta);
	if(controllerIsSet)
	{
		if(tutorialCon->TutorialEnded())
		{
			if(beforeEnd == 5.0)
			{
				data.floors.clear();
				data.buttons.clear();
				data.graphics.clear();
				data.messages.clear();
				
				Text message("That's all\n\n						For now!",64);
				message.SetPosition(PointU(600,350));
				data.messages["EndGame"] = std::move(message);
			}
			if(beforeEnd > 0)
				beforeEnd -= delta;
			else
				data.exitBattle = true;
		}
		else
			tutorialCon->Update(delta);
	}
}
/*void Demo::TutorialBattle::Draw(const Canvas& ) const
{
}*/

void Demo::TutorialBattle::CreatePlayerController(std::weak_ptr<Input> ptr)
{
	tutorialCon = std::make_unique<TutorialController>(&data,ptr);
	controllerIsSet = true;
}
