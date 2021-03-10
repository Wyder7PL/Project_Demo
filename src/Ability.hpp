#pragma once

#include "Action.hpp"
#include "Location.hpp"
#include "graphic/Sprite.hpp"
#include "graphic/Bar.hpp"

#include "graphic/TextureHolder.hpp"

#include <vector>

namespace Demo
{
	
	enum class CallBack{
		Fail
	};
	
	
class Ability
{
	public:

	enum class DisplayMode{
		None,
		JustDraw,
		DrawAndActivate
	};

	Ability();
	virtual ~Ability();
	
	void Update(float delta);
	void Draw(const Canvas& canvas) const;
	
	// Return empty vector if cooldown is greater than 0
	// otherwise return vector with constructed action
	std::vector<Action> Use(const Location& destination, bool targetingEnemy = true);
	
	// Construct one Action that can contain other actions
	// regardless of cooldown
	virtual Action ConstructAction(const Location& destination, bool targetingEnemy = true) = 0;
	
	// Used to deep-copy chirdlen of this class
	virtual Ability* Clone() = 0;
	
	void SetMaxCooldown(double limit);
	double GetMaxCooldown();
	
	void SetCooldown(double amount);
	double GetCooldown();
	
	void SetPawnCooldownPercentage(const double& percentage);
	
	void ReduceCooldown(double amount);
	void ResetCooldown();
	void IncreaseCooldown(double amount);
	
	bool IsCoolingDown();
	bool CooldownStatusChanged();
	
	void SetAbilityRange(const ActionRange& newRange);
	ActionRange GetAbilityRange();
	
	virtual bool WasItYourAction(const CallBack& whatHappened, const std::string& actionData);
	
	bool IsAbilityDraggable();
	
	void SetDefault(bool);
	
	void SetDisplayPosition(const PointU&);
	PointU GetDisplayPosition();
	
	void SetDisplaySize(const PointU& size);
	PointU GetDisplaySize();
	
	void SetSprite(const std::string& name);
	
	virtual std::vector<std::pair<std::string,int>> GetAbilityDamageData(const bool& isSupportive);
	
protected:
	
	ActionIntentions GetActionIntentions(const std::string& actionData);
	std::string GetRidOfActionIntentionFlag(const std::string& actionData);

	ActionRange range = ActionRange::Point;
	double cooldown;
	double maxCooldown;
	double defaultAction;
	
	Sprite sprite;
private:
	Bar background;
	Bar cooldownBar;
	Bar pawnCooldownBar;
	
	bool wasCoolingDown = false;
	bool pawnCooldownLock = false;
};

}

