#include "ControllerV1.hpp"

Demo::ControllerV1::ControllerV1(BattleData* data)
:Controller(data)
{
	for(unsigned int i = 0;i < data->floors["Enemy"].GetDimensions().x-1; ++i)
	{
		BattlePawn EnemyPawn;
		if(PawnList::GetInstance().OverridePawn("OtterSwordman",EnemyPawn))
			data->floors["Enemy"].AddPawnAtLocation(std::move(EnemyPawn),PointU(i,i));
	}
	BattlePawn EnemyPawn;
		if(PawnList::GetInstance().OverridePawn("OtterArcher",EnemyPawn))
			data->floors["Enemy"].AddPawnAtLocation(std::move(EnemyPawn),PointU(1,0));
	
	BattlePawn EnemyPawn1;
		if(PawnList::GetInstance().OverridePawn("OtterArcher",EnemyPawn1))
			data->floors["Enemy"].AddPawnAtLocation(std::move(EnemyPawn1),PointU(1,1));
	
	BattlePawn EnemyPawn2;
		if(PawnList::GetInstance().OverridePawn("OtterKnight",EnemyPawn2))
			data->floors["Enemy"].AddPawnAtLocation(std::move(EnemyPawn2),PointU(0,3));
	
	BattlePawn EnemyPawn3;
		if(PawnList::GetInstance().OverridePawn("OtterGunner",EnemyPawn3))
			data->floors["Enemy"].AddPawnAtLocation(std::move(EnemyPawn3),PointU(1,3));
	
	// Fill entire floor with dummies
	/*for(unsigned int i = 0; i < data->floors["Enemy"].GetDimensions().x; ++i)
	{
		for(unsigned int j = 0; j < data->floors["Enemy"].GetDimensions().y; ++j)
		{
			BattlePawn EnemyPawn;
			if(PawnList::GetInstance().OverridePawn("OtterDummy",EnemyPawn))
				data->floors["Enemy"].AddPawnAtLocation(std::move(EnemyPawn),PointU(i,j));
		}
	}*/
}

Demo::ControllerV1::~ControllerV1()
{
}

void Demo::ControllerV1::Update(double delta)
{
	
	bool foundTarget;
	PointU targetLocation(0,0);
	FindTarget(foundTarget,targetLocation);
	
	if(foundTarget && battleDataPtr->floors.count("Enemy"))
	{
		Floor& ef = battleDataPtr->floors["Enemy"];
		for(unsigned int i = 0; i < ef.GetDimensions().y;i++)
		{
			for(unsigned int j = 0; j < ef.GetDimensions().x;j++)
			{
				BattlePawn& pawn = ef.GetPawnAtLocation(PointU(j,i));
				if(!pawn.IsEmptySpace())
					pawn.OrderAction(0,Location("PlayerFloor",targetLocation));
			}
		}
	}
}

void Demo::ControllerV1::FindTarget(bool& find, PointU& location)
{
	find = false;
	if(battleDataPtr->floors.count("PlayerFloor"))
	{
		Floor& pf = battleDataPtr->floors["PlayerFloor"];
		for(unsigned int i = 0; i < pf.GetDimensions().y;i++)
		{
			for(unsigned int j = 0; j < pf.GetDimensions().x;j++)
			{
				if(!pf.GetPawnAtLocation(PointU(j,i)).IsEmptySpace())
				{
					find = true;
					location.x = j;
					location.y = i;
					return;
				}
			}
		}
	}
}
