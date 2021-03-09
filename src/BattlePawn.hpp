#pragma once

#include "GameObject.hpp"
#include "Action.hpp"
#include "Ability.hpp"
#include "graphic/Bar.hpp"
#include "graphic/Sprite.hpp"
#include "graphic/TextureHolder.hpp"
#include "graphic/DefaultAbilityDamageDisplay.hpp"
#include "Location.hpp"
#include "EnergyBar.hpp"
#include "effects/BattleEffect.hpp"

#include <sstream>

#include <memory>
#include <vector>
#include <map>
#include <unordered_set>

namespace Demo
{

class BattlePawn : public GameObject
{
public:
	BattlePawn();
	virtual ~BattlePawn();
	
	BattlePawn(const BattlePawn&);
	BattlePawn(BattlePawn&&) = default;
	BattlePawn& operator=(const BattlePawn&);
	BattlePawn& operator=(BattlePawn&&) = default;
	
	virtual void Update(double delta) override;
	virtual void Draw(const Canvas& canvas) const override;
	
	void Destroy();
	bool ToDestroy();
	
	void OrderAction(unsigned int abilityIndex,Location actionDestination, bool targetingEnemy = true);
	std::vector<Action> GetActions();
	
	void AddAbility(std::unique_ptr<Ability> ab, unsigned int index = 0);
	void RemoveAbility(unsigned int index);
	
	bool IsAbilityDraggable(unsigned int index);
	void SendAbilityCallBack(const CallBack& what,const std::string& actionData);
	ActionRange GetAbilityRange(unsigned int index = 0);
	
	void SetAbilityDisplayMode(Ability::DisplayMode);
	Ability::DisplayMode GetAbilityDisplayMode();
	
	void AddEffect(std::unique_ptr<BattleEffect> be);
	void OrderToMove(Location moveDestination, bool forced = false);
	
	// Applying negative amounts will increase health
	void DealDamage(const std::string& healthType, const int& amount, const bool& ignoreResistances = false);
	void GiveSupport(const std::string& healthType, const int& amount);
	void SetHP(const std::string& healthType, int value);
	std::vector<int>& GetDamageToDisplay();
	
	void UpdatePawnCooldown(const double& delta);
	void IncreasePawnCooldown(const double& cooldown);
	
	virtual void SetPosition(const PointU& pos) override;
	PointU GetPosition(const unsigned int abilityIndex = 0);
	
	bool IsEmptySpace() const;
	void SetEmptySpace(bool empty);
	
	bool IsPointOnAbility(const PointU& point, unsigned int& index);
	
	// It doesn't actually set pawn's location, rather is used to tell pawn where it is;
	void SetLocation(const Location&);
	Location GetLocation();
	
	void SetSize(const PointU& size);
	
	void SetSprite(const std::string& spriteName,unsigned int index = 0);
	
	void AddEnergyBar(const std::string index, EnergyBar energy);
private:
	void ModifyActionBuffer();
	void SetActionsOwnership();
	
	void RearangeEffectLinks();
	void RELIteration(unsigned int& index, unsigned int& linkIndex, std::vector<std::pair<unsigned int,std::string>>& effectLinkVec);
	void RemoveLinksOnEnd(unsigned int& linkIndex, std::vector<std::pair<unsigned int,std::string>>& effectLinkVec);

	void UpdateAbilityDisplay();
	void UpdateEffectGraphicPosition();
	void UpdateEffectGraphicDisplay();
	
	void UpdateDefaultAbilityInfo();

	std::map<std::string,EnergyBar> healths;
	std::vector<std::unique_ptr<Ability>> abilities;
	std::vector<DefaultAbilityDamageDisplay> defaultAbilityInfo;
	std::vector<std::unique_ptr<BattleEffect>> effects;
	std::vector<std::pair<unsigned int,std::string>> attackDealingActionModyfier;
	std::vector<std::pair<unsigned int,std::string>> attackRecivingActionModyfier;
	
	bool toDestroy;
	bool emptySpace;
	Location selfLocation;
	Ability::DisplayMode displayAbilities = Ability::DisplayMode::None;
	
	PointU pawnSize;
	
	std::vector<Action> actionBuffer;
	std::vector<int> damageToDisplay;
	
	double pawnCooldown;
	double pawnMaxCooldown;
	
	Bar cooldownBar;
	Bar pawnCooldownBar;
	Sprite bar;
};

}

