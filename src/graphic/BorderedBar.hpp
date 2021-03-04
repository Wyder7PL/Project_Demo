#pragma once

#include "Bar.hpp"

namespace Demo
{

class BorderedBar : public Demo::Bar
{
public:
	BorderedBar(const PointU& dimensions);
	virtual ~BorderedBar();
	
	virtual void UpdateBar();
	
	virtual void SetPosition(const PointU& );
	virtual void SetRotation(const double& angle);
	
	void SetBackgroundColor(const unsigned char& r,const unsigned char& g,const unsigned char& b,const unsigned char& a = 255);
	void SetBorderColor(const unsigned char& r,const unsigned char& g,const unsigned char& b,const unsigned char& a = 255);

protected:
	virtual void HiddenDraw(sf::RenderTarget& ) const override;
	
private:
	sf::RectangleShape background;
	sf::RectangleShape borders;
};

}