#pragma once

#include "../graphic/Sprite.hpp"
#include "../Utility.hpp"

#include <bitset>
#include <vector>
#include <memory>


namespace Demo
{

class Effect
{
public:

	enum PersistenceType : unsigned int
	{
		Destroyed = 0,
		Timed,
		BattleTime, // Time of effect pass only during battle
		BattleOnly,
		BattleAffecting,
		
		// Curses are harder to cure, often by overrided HealCurse function but some custom
		// effects can specify different methods to remove curse and disable this function
		
		LightCurse, // This type of curse has "curse points" > 0, when those points reach 0
					// then effect is probably removed, to reduce those points use 
					// overrided HealCurse() function
		
		HardCurse,  // This type of curse has "curse level" > 0, to heal it, overrided HealCurse()
					// must be used with argument that is equal or greater than "curse level" 
		
		// When Light and Hard curses are combined, "curse points" need to be reduced to remove curse,
		// reducing "curse point": cpoints -= cremove - clevel
		
		Trait, // Almost permanent, can be removed but not always
		Constant // Effect is considered as permanent trait, can't be removed
	};
	
	explicit Effect(const std::string& effectName);
	virtual ~Effect();
	
	virtual Effect* Clone() = 0;
	virtual Effect* Clone(const std::vector<double>& args) = 0;
	
	virtual void MergeEffect(Effect*) = 0;
	
	virtual void Update(double delta) = 0;
	
	void Draw(const Canvas& canvas);
	bool IsGraphicSet();
	void SetGraphicPosition(const PointU&);
	void SetGraphicSize(const PointU&);
	
	void SetToDestroy();
	
	std::string GetName();
	
	bool GetFlag(PersistenceType flag);
	
	unsigned int HealCurse(const unsigned int& curseRemovingPoints);
	
protected:
	void SetFlag(PersistenceType flag, bool value);
	
	std::shared_ptr<Sprite> effectGraphic;
private:
	const std::string name; // set by constructors
	std::bitset<9> persistenceFlags;
};

}

