#include "PawnList.hpp"

#include <iostream>
#include <fstream>

#if defined(__linux__)
	#include <filesystem>
#elif defined(_WIN32)
	#include <windows.h>
#endif

Demo::PawnList& Demo::PawnList::GetInstance()
{
	static PawnList result;
	return result;
}

void Demo::PawnList::LoadDefault()
{
	#if defined(__linux__)
		namespace fs = std::filesystem;
		fs::path p = fs::current_path();
		p /= "resources";
		p /= "pawns";
		
		for (const auto& entry : fs::directory_iterator(p))
		{
			if(fs::is_regular_file(entry.status()))
			{
				std::string extension = entry.path().extension();
				if(extension == ".txt")
				{
					std::string relativePath = entry.path().relative_path();
					std::string filepath = "resources/pawns/";
					filepath += entry.path().filename();
					std::string name = entry.path().stem();
					
					defaultName = true;
					pawnSpriteName = "";
					LoadPawnFromFile(filepath,name);
				}
			}
		}
	
	#elif defined(_WIN32)
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = FindFirstFile("resources\\pawns\\*.txt", &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE) {
			std::cout << "Could not open FirstFile\n";
			return;
		}

		do {
			std::string name = FindFileData.cFileName;
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;
			else if(name.length() > 4)
			{
				std::string ext = name.substr(name.length()-4,4);
				if( ext == ".txt")
				{
					std::string nameNoExt = name.substr(0,name.length()-4);
					LoadPawnFromFile("resources\\pawns\\"+name,nameNoExt);
				}
			}
		} while (FindNextFile(hFind, &FindFileData) != 0);
		FindClose(hFind);
	#endif
	
	LoadCustom();
}

std::vector<Demo::BattlePawn> Demo::PawnList::CreatePawn(const std::string& pawnName)
{
	std::vector<BattlePawn> result;
	auto iter = pawns.find(pawnName);
	while(iter != pawns.end())
	{
		BattlePawn pawn = (*iter).second;
		result.push_back(std::move(pawn));
		++iter;
	}
	return result;
}

bool Demo::PawnList::OverridePawn(const std::string& pawnName, BattlePawn& pawn)
{
	auto vec = CreatePawn(pawnName);
	if(vec.empty()) return false;
	pawn = std::move(vec[0]);
	return true;
}

namespace Demo{

inline void SkipLine(std::ifstream& file)
{
	char c = 0;
	do { file.get(c); }
	while(file.good() && c != '\n');
}

void AddEffectToPawn(BattlePawn& pawn,std::ifstream& file)
{
	std::string effectName;
	file >> effectName;
	std::vector<double> args;
	char c = 0;
	while(file.good())
	{
		file >> c;
		if(c == '>')
			break;
		if(isdigit(c) || c == '.')
		{
			file.putback(c);
			double digit;
			file >> digit;
			args.push_back(std::move(digit));
		}
	}
	auto vec = EffectList::GetInstance().CreateEffect(effectName,args);
	if(vec.empty())
		std::cout << "Couldn't load effect";
	for(auto& i : vec)
		pawn.AddEffect(std::move(std::unique_ptr<BattleEffect>(dynamic_cast<BattleEffect*>(i.release()))));
}

void AddEnergyToPawn(BattlePawn& pawn,std::ifstream& file)
{
	std::string energyType;
	file >> energyType;
	std::vector<int> args;
	EnergyBar energy;
	
	char c = 0;
	while(file.good())
	{
		file >> c;
		if(c == '>')
			break;
		if(isdigit(c))
		{
			file.putback(c);
			int digit;
			file >> digit;
			args.push_back(std::move(digit));
		}
		if(c == 'c')
		{
			file.putback(c);
			std::string color;
			file >> color;
			if(color == "color")
			{
				unsigned int value = 0;
				file >> value;
				energy.SetColor(value);
			}
			else continue;
		}
	}
	if(args.size() > 0)
		energy.SetMaxEnergy(args[0]);
	if(args.size() > 1)
		energy.SetEnergy(args[1]);
	
	pawn.AddEnergyBar(energyType,std::move(energy));
}

std::string LoadToString(std::ifstream& file)
{
	std::string s;
	char c = file.get();
	while(c != '>' && file.good())
	{
		c = file.get();
		if(c != '>')
			s.push_back(c);
	}
	return s;
}

Demo::ActionRange ToActionRange(const std::string& str)
{
	if(str == "none") return ActionRange::None;
	if(str == "point") return ActionRange::Point;
	if(str == "plus") return ActionRange::Plus;
	if(str == "x") return ActionRange::X;
	if(str == "line") return ActionRange::Line;
	if(str == "front") return ActionRange::Frontline;
	if(str == "self") return ActionRange::Self;
	if(str == "aof") return ActionRange::AllOnFloor;
	if(str == "aofec") return ActionRange::AllOnFloorExceptCaster;
	if(str == "watcher") return ActionRange::Watcher;
	return ActionRange::Point;
}

void SetAbilityDodge(std::unique_ptr<GenericAbility>& ability,std::ifstream& file)
{
	unsigned int accval = 10;
	bool randomize = false;
	char c = file.get();
	while(c != '>' && file.good())
	{
		c = file.get();
		if(c == '>')
			break;
		if(isdigit(c))
		{
			file.putback(c);
			file >> accval;
		}
		else if(c == 'r')
			randomize = true;
	}
	
	
	Accuracy acc(accval);
	if(randomize)
		acc.RandomizePreciseHitCount();
	ability->SetNewAccuracy(acc);
}

void AddAbilityToPawn(BattlePawn& pawn,std::ifstream& file)
{
	std::unique_ptr<GenericAbility> ability = std::make_unique<GenericAbility>();
	double maxCooldown = 0, cooldown = 0;
	file >> maxCooldown >> cooldown;
	ability->SetMaxCooldown(maxCooldown);
	ability->SetCooldown(cooldown);
	char c = 0;
	while(file.good())
	{
		file >> c;
		if(c == '>')
			break;
		if(c == '<')
		{
			std::string temp;
			file >> temp;
			if(temp == "effect")
			{
				ability->AddEffectToAbility(LoadToString(file));
			}
			else if(temp == "damage")
			{
				ability->AddDamageToAbility(LoadToString(file));
			}
			else if(temp == "accuracy")
			{
				SetAbilityDodge(ability,file);
			}
		}
		if(c == '*')
		{
			std::string temp;
			file >> temp;
			ability->SetAbilityRange(ToActionRange(temp));
		}
		if(c == '#')
		{
			std::string sprite;
			file >> sprite;
			ability->SetSprite(sprite);
		}
	}
	std::unique_ptr<Ability> ptr(dynamic_cast<Ability*>(ability.release()));
	pawn.AddAbility(std::move(ptr));
}

void AddSummonAbilityToPawn(BattlePawn& pawn,std::ifstream& file)
{
	std::unique_ptr<SummonAbility> ability = std::make_unique<SummonAbility>();
	std::string pawnName;
	double maxCooldown = 0, cooldown = 0;
	file >> pawnName >> maxCooldown >> cooldown;
	
	ability->SetSummonName(pawnName);
	ability->SetMaxCooldown(maxCooldown);
	ability->SetCooldown(cooldown);
	
	char c = 0;
	do{
		c = file.get();
	}while(c != '>' && file.good());
		
	std::unique_ptr<Ability> ptr(dynamic_cast<Ability*>(ability.release()));
	pawn.AddAbility(std::move(ptr));
}

void SetPawnDodge(BattlePawn& pawn, std::ifstream& file)
{
	unsigned int dodge = 0, initial = 0;
	bool setInitial = false;
	file >> dodge;
	
	char c = 0;
	while(file.good())
	{
		file >> c;
		if(c == '>')
			break;
		if(isdigit(c))
		{
			file.putback(c);
			file >> initial;
			setInitial = true;
		}
	}
	if(setInitial)
	{
		pawn.SetPawnDodge(dodge);
		pawn.SetPawnInitialDodge(initial);
	}
	else
	{
		pawn.SetPawnDodge(dodge);
		pawn.RandomizeDodge();
	}
}

}

void Demo::PawnList::LoadPawnFromFile(const std::string& filePath,std::string& name)
{
	std::ifstream file{filePath};
	if(!file)
	{
		std::cout << "couldn't load file\n";
	}
	BattlePawn pawn;
	pawn.SetEmptySpace(false);
	char sentenceBegin = 0;
	while(file.good())
	{
		file >> sentenceBegin;
		if(sentenceBegin == '/')
			SkipLine(file);
		else if (sentenceBegin == '<')
		{
			std::string sentenceType;
			file >> sentenceType;
			if(sentenceType == "effect")
			{
				AddEffectToPawn(pawn,file);
			}
			else if(sentenceType == "energy")
			{
				AddEnergyToPawn(pawn,file);
			}
			else if(sentenceType == "ability")
			{
				AddAbilityToPawn(pawn,file);
			}
			else if(sentenceType == "summon")
			{
				AddSummonAbilityToPawn(pawn,file);
			}
			else if(sentenceType == "dodge")
			{
				SetPawnDodge(pawn,file);
			}
		}
	}
	
	if(pawnSpriteName == "")
		pawn.SetSprite(name);
	
	if(pawns.count(name) > 0)
	{
		std::cout << "pawn alredy exist\n";
		// TODO error/waring pawn alredy exist
		return;
	}
	if(defaultName)
		pawns[name] = pawn;
}

void Demo::PawnList::LoadCustom()
{
	BattlePawn OtterBanner;
	OtterBanner.SetEmptySpace(false);
	EnergyBar energy;
	energy.SetMaxEnergy(20); energy.SetEnergy(20);
	energy.SetColor(0x00FF00FF);
	OtterBanner.AddEnergyBar("Pain",std::move(energy));
	OtterBanner.SetSprite("OtterBanner");
	
	std::unique_ptr<GenericAbility> ability = std::make_unique<GenericAbility>();
	ability->SetMaxCooldown(2.0); ability->SetCooldown(2.0);
	ability->SetAbilityRange(ActionRange::Frontline);
	ability->AddDamageToAbility("Pain 2");
	{
		std::unique_ptr<Ability> ptr(dynamic_cast<Ability*>(ability.release()));
		OtterBanner.AddAbility(std::move(ptr));
	}
	
	OtterBanner.AddAbility(std::make_unique<TestAbility>());
	
	ability = std::make_unique<GenericAbility>();
	ability->SetMaxCooldown(10.0); ability->SetCooldown(2.0);
	ability->AddEffectToAbility("*support Prepare 1 1.2 5.0");
	ability->SetAbilityRange(ActionRange::AllOnFloorExceptCaster);
	ability->SetSprite("Prepare");
	{
		std::unique_ptr<Ability> ptr(dynamic_cast<Ability*>(ability.release()));
		OtterBanner.AddAbility(std::move(ptr));
	}
	
	std::unique_ptr<SummonAbility> sAbility = std::make_unique<SummonAbility>();
	sAbility->SetMaxCooldown(30.0); sAbility->SetCooldown(0.0);
	sAbility->SetSummonName("OtterSwordman");
	sAbility->SetSprite("Summon");
	{
		std::unique_ptr<Ability> ptr(dynamic_cast<Ability*>(sAbility.release()));
		OtterBanner.AddAbility(std::move(ptr));
	}
	pawns["OtterBanner"] = OtterBanner;
}
