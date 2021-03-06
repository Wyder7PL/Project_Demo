#pragma once

#include "Effect.hpp"
#include "BattleEffect.hpp"
#include "WatcherEffect.hpp"

#include <memory>
#include <vector>
#include <unordered_map>

namespace Demo
{

class EffectList
{
public:
	static EffectList& GetInstance();
	
	EffectList(EffectList const&) = delete;
	void operator=(EffectList const&) = delete;
	
	void LoadDefault();
	
	std::vector<std::unique_ptr<Effect>> CreateEffect(const std::string name,const std::vector<double>& args) const;
	std::vector<std::unique_ptr<WatcherEffect>> CreateWatcherEffect(const std::string name, BattleData* data ,const std::vector<double>& args);
	
private:
	void Load(Effect*);

	EffectList(){};
	std::unordered_map<std::string,std::unique_ptr<Effect>> effects;
};

}

