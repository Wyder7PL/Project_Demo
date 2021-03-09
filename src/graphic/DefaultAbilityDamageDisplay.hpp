#pragma once

#include "Graphic.hpp"
#include "Sprite.hpp"
#include "Text.hpp"

#include <sstream>
#include <cassert>


namespace Demo
{

class DefaultAbilityDamageDisplay : public Demo::Graphic
{
public:
	DefaultAbilityDamageDisplay();
	DefaultAbilityDamageDisplay(const std::string& textureName, const int& amount);
	virtual ~DefaultAbilityDamageDisplay();

	virtual void Draw(const Canvas& canvas) const override;
	
	virtual void SetPosition(const PointU& pos);
	virtual PointU GetPosition() override;
	
	virtual void SetRotation(const double& angle) override;
	virtual double GetRotation() override;
	
	virtual void SetSize(const unsigned int&);
	
	void SetIcon(const std::string& textureName);
	
	void UpdateText(const std::string& txt);
	void SetTextColor(const unsigned char& r,const unsigned char& g,const unsigned char& b,const unsigned char& a = 255);
	void SetTextColor(const unsigned int& rgba);
	
	void SetTextAllign(bool allignFromLeftSide);
	
protected:
	virtual void HiddenDraw(sf::RenderTarget& ) const override;

private:
	int DamageNameToColor(const std::string&);

	Sprite icon;
	Text number;
	
	bool textAllignLeft;
};

}

