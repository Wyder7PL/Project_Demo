#include "ElectricChain.hpp"
#include "EffectList.hpp"

Demo::ElectricChain::ElectricChain(BattleData* data, const std::vector<double>& args)
:WatcherEffect(data,"ElectricChain")
,electricityVoltage(10)
,electricityIntensity(4)
,targetsLeft(4)
{
	for(unsigned int i = 0; i < args.size(); ++i)
	{
		switch(i)
		{
			case 0: electricityVoltage = abs(round(args[i])); break;
			case 1: electricityIntensity = abs(round(args[i])); break;
			case 2: targetsLeft = abs(round(args[i])); break;
			default: break;
		}
	}
}

Demo::ElectricChain::~ElectricChain()
{
}

void Demo::ElectricChain::WatcherUpdate(const double& delta)
{
	
	while(targetsLeft > 0)
	{
		
		if(hittedTargets.size() == 0)
		{
			BattlePawn& target = battleDataPtr->floors[destination.GetFloor()].GetPawnAtLocation(destination.GetLocationPoint());
			if(!target.IsEmptySpace())
			{
				hittedTargets.push_back(destination.GetLocationPoint());
				ApplyElectricityToPawn(target);
			}
			else
				break;
		}
		else if(potentialTargets.size() > 0)
		{
			unsigned int index = RandomInt<unsigned int>(0, potentialTargets.size()-1);
			ApplyElectricityToPawn( battleDataPtr->floors[destination.GetFloor()].GetPawnAtLocation(potentialTargets[index]) );
			
			hittedTargets.push_back(potentialTargets[index]);
			potentialTargets.clear();
		}
		else
			break;
		
		SetPotentialTargets();
		--targetsLeft;
	}
	
	SetToDestroy();
}

void Demo::ElectricChain::SetPotentialTargets()
{
	PointU floorDim = battleDataPtr->floors[destination.GetFloor()].GetDimensions();
	std::vector<PointI> points = { PointI(-1,0), PointI(0,-1), PointI(1,0), PointI(0,1) };
	for(auto& ht : hittedTargets)
	{
		for(auto& p : points)
		{
			PointI res((int)ht.x + p.x, (int)ht.y + p.y);
			if( res.x >= 0 && res.x < (int)floorDim.x && res.y >= 0 && res.y < (int)floorDim.y )
			{
				PointU resu(res.x,res.y);
				if(!battleDataPtr->floors[destination.GetFloor()].GetPawnAtLocation(resu).IsEmptySpace())
				{
					bool wasHitTarget = false;
					for(auto& hta : hittedTargets)
						if(hta == resu)
						{
							wasHitTarget = true;
							break;
						}
					if(!wasHitTarget)
						potentialTargets.push_back(resu);
				}
			}
		}
	}
}

void Demo::ElectricChain::ApplyElectricityToPawn(BattlePawn& target)
{
	std::vector<std::unique_ptr<Demo::Effect>> vec;
	vec = EffectList::GetInstance().CreateEffect("Electricity",{(double)electricityVoltage,(double)electricityIntensity});
	for(auto& i : vec)
	{
		BattleEffect* be = dynamic_cast<BattleEffect*>(i.release());
		std::unique_ptr<BattleEffect> bep(be);
		target.AddEffect(std::move(bep));
		
		if(electricityIntensity > 1)
			electricityIntensity -= 1;
	}
	
}
