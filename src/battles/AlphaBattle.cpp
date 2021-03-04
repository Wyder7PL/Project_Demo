#include "AlphaBattle.hpp"

#include "../controllers/ControllerV1.hpp"

Demo::AlphaBattle::AlphaBattle()
{
	data.floors["PlayerFloor"] = Floor();
	data.floors["PlayerFloor"].SetName("PlayerFloor");
	data.floors["PlayerFloor"].SetMovePawnsFrontAutomatically(false);
	data.floors["PlayerFloor"].SetFrontline(false);
	
	data.floors["Enemy"] = Floor();
	data.floors["Enemy"].SetName("Enemy");
	data.floors["Enemy"].SetMovePawnsFrontAutomatically(true);
	data.floors["Enemy"].SetFrontline(true);

	data.floors["PlayerFloor"].SetDimensions(PointU(2,2));
	data.floors["PlayerFloor"].SetSize(PointU(500,500));
	data.floors["PlayerFloor"].SetLocation(PointU(300,300));
	data.floors["PlayerFloor"].SetOffset(PointU(30,30));
	data.floors["PlayerFloor"].SetSpacing(PointU(30,30));
	
	data.floors["PlayerFloor"].SetRightOriented(true);
	
	data.floors["Enemy"].SetDimensions(PointU(4,4));
	data.floors["Enemy"].SetSize(PointU(500,500));
	data.floors["Enemy"].SetLocation(PointU(900,300));
	data.floors["Enemy"].SetOffset(PointU(50,50));
	data.floors["Enemy"].SetSpacing(PointU(20,20));
	
	controllers.push_back(std::make_unique<ControllerV1>(&data));
	
	std::unique_ptr<Sprite> backgroundSprite = std::make_unique<Sprite>();
	TextureHolder::GetInstance().SetSpriteTexture(*backgroundSprite,"FlowerHill");
	backgroundSprite->SetSize(PointU(1400,900));
	backgroundSprite->SetPosition(PointU(600,400));
	data.backgroundGraphics["FlowerHill"] = std::move(backgroundSprite);
}

Demo::AlphaBattle::~AlphaBattle()
{
}

void Demo::AlphaBattle::Update(double delta)
{
	Battle::Update(delta);
}
