#pragma once

#include "../GameObject.hpp"
#include "Text.hpp"

namespace Demo
{

class DamageDisplay : public Demo::GameObject
{
public:
	// if message is a text, not number, then proportionalSize need to be set false
	DamageDisplay(std::string message = "", bool proportionalSize = true, unsigned int size = 32, double lifetime = 1.0);
	virtual ~DamageDisplay();
	
	//DamageDisplay(DamageDisplay&&) = default;
	//DamageDisplay& operator=(DamageDisplay&&) = default;

	virtual void Draw(const Canvas& canvas) const;
	virtual void Update(double delta);
	
	virtual void SetPosition(const PointU& pos) override;
	
	virtual void SetText(std::string message);
	
	virtual void SetTextColor(int rgba);
	
	bool ToDestroy();
private:
	Text text;
	double lifetimeLeft;
	double initialLifetime;
};

}

