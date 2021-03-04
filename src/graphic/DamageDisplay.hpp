#pragma once

#include "../GameObject.hpp"
#include "Text.hpp"

namespace Demo
{

class DamageDisplay : public Demo::GameObject
{
public:
	DamageDisplay(std::string message = "", bool proportionalSize = true, unsigned int size = 32, double lifetime = 1.0);
	virtual ~DamageDisplay();
	
	//DamageDisplay(DamageDisplay&&) = default;
	//DamageDisplay& operator=(DamageDisplay&&) = default;

	virtual void Draw(const Canvas& canvas) const;
	virtual void Update(double delta);
	
	virtual void SetPosition(const PointU& pos) override;
	
	virtual void SetText(std::string message);
	
	bool ToDestroy();
	void TESTPrintStat();
private:
	Text text;
	double lifetimeLeft;
	double initialLifetime;
};

}

