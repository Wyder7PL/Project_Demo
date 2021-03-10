#pragma once

#include "BattlePawn.hpp"

#include "abilities/TestAbility.hpp"

#include "effects/EffectList.hpp"
#include "graphic/TextureHolder.hpp"
#include "EnergyBar.hpp"
#include "abilities/GenericAbility.hpp"
#include "abilities/SummonAbility.hpp"

#include <memory>
#include <vector>

namespace Demo
{

class PawnList
{
public:
	static PawnList& GetInstance();
	
	PawnList(PawnList const&) = delete;
	void operator=(PawnList const&) = delete;
	
	void LoadDefault();
	
	std::vector<BattlePawn> CreatePawn(const std::string& pawnName);
	
	bool OverridePawn(const std::string& pawnName,BattlePawn& pawn);
	
private:
	PawnList(){};
	void LoadPawnFromFile(const std::string& filePath, std::string& name);
	
	void LoadCustom();
	
	std::unordered_map<std::string,BattlePawn> pawns;
	bool defaultName = true;
	std::string pawnSpriteName = "";
};

}