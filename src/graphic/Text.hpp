#pragma once

#include "Graphic.hpp"

#include <string>

namespace Demo
{

class Text : public Graphic
{
public:
	Text(const std::string& txt = "", const unsigned int& fontSize = 16);
	virtual ~Text();
	
	virtual void SetPosition(const PointU&) override;
	virtual PointU GetPosition() override;
		
	virtual void SetRotation(const double& angle) override;
	virtual double GetRotation() override;
	
	void UpdateText(const std::string& txt);
	
	void SetFontSize(const unsigned int& size);
	unsigned int GetFontSize();
	
	unsigned int GetWidth();
	
	void SetFontColor(const unsigned char& r,const unsigned char& g,const unsigned char& b,const unsigned char& a = 255);
	
protected:
	virtual void HiddenDraw(sf::RenderTarget&) const override;
	
private:
	void UpdatePosition();

	sf::Text text;
	
	static sf::Font ImpactedFontConstruct();
	static sf::Font impactedFont;
	
	static sf::Font FairJudgeFontConstruct();
	static sf::Font fairJudgeFont;
	
	static sf::Font InfiniteConstruct();
	static sf::Font infiniteFont;
	
	static sf::Font IndochineConstruct();
	static sf::Font indochineFont;
};

}

