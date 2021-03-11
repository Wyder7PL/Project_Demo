#include "EffectList.hpp"

#include "Critical.hpp"
#include "DefaultDamageAdsorption.hpp"
#include "DefaultDamageBooster.hpp"
#include "Distraction.hpp"
#include "Electricity.hpp"
#include "Fire.hpp"
#include "HealthRegen.hpp"
#include "Marked.hpp"
#include "Poison.hpp"
#include "Prepare.hpp"
#include "Shield.hpp"

#include "ElectricChain.hpp"

#include <iostream>

Demo::EffectList& Demo::EffectList::GetInstance()
{
	static EffectList result;
	return result;
}

void Demo::EffectList::LoadDefault()
{
	Load(new Critical());
	Load(new DefaultDamageAdsorption());
	Load(new DefaultDamageBooster());
	Load(new Distraction());
	Load(new Electricity());
	Load(new Fire());
	Load(new HealthRegen());
	Load(new Marked());
	Load(new Poison());
	Load(new Prepare());
	Load(new Shield());
}

std::vector<std::unique_ptr<Demo::Effect>> Demo::EffectList::CreateEffect(const std::string name, const std::vector<double>& args) const
{
	std::vector<std::unique_ptr<Effect>> result;
	auto iter = effects.find(name);
	if(iter == effects.end())
		std::cout << "Forgot to load effect because it doesn't exist\n";
	if(iter != effects.end())
	{
		std::unique_ptr<Effect> ptr((*iter).second->Clone(args));
		result.push_back(std::move(ptr));
	}
	return result;
}

std::vector<std::unique_ptr<Demo::WatcherEffect>> Demo::EffectList::CreateWatcherEffect(const std::string name, BattleData* data ,const std::vector<double>& args)
{
	std::vector<std::unique_ptr<WatcherEffect>> result;
	if(name == "ElectricChain") result.push_back(std::make_unique<ElectricChain>(data,args));
	return result;
}

void Demo::EffectList::Load(Effect* e)
{
	std::unique_ptr<Effect> ptr(e);
	std::string name = ptr->GetName();
	effects[name] = std::move(ptr);
}
