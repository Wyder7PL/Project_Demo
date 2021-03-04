#include "BorderedBar.hpp"

Demo::BorderedBar::BorderedBar(const PointU& dimensions)
:Bar(dimensions)
{
	borders.setFillColor(sf::Color(0,0,0,0));
	SetBackgroundColor(50,50,50,100);
	SetBorderColor(200,200,200,255);
}

Demo::BorderedBar::~BorderedBar()
{
}

void Demo::BorderedBar::SetPosition(const PointU& point)
{
	Bar::SetPosition(point);
	background.setPosition(point.x,point.y);
	borders.setPosition(point.x,point.y);
}

void Demo::BorderedBar::SetRotation(const double& angle)
{
	Bar::SetRotation(angle);
	background.setRotation(angle);
	borders.setRotation(angle);
}

void Demo::BorderedBar::UpdateBar()
{
	Bar::UpdateBar();
	background.setSize(sf::Vector2f(GetSize().x,GetSize().y));
	background.setOrigin(GetSize().x/2,GetSize().y/2);
	
	unsigned int bordetThickness = 2;
	
	borders.setSize(sf::Vector2f(GetSize().x-bordetThickness,GetSize().y-bordetThickness));
	borders.setOrigin(GetSize().x/2,GetSize().y/2);
	borders.setOutlineThickness(bordetThickness);
}

void Demo::BorderedBar::HiddenDraw(sf::RenderTarget& target) const
{
	target.draw(background);
	Bar::HiddenDraw(target);
	target.draw(borders);
}

void Demo::BorderedBar::SetBackgroundColor(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	background.setFillColor(sf::Color(r,g,b,a));
}

void Demo::BorderedBar::SetBorderColor(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	borders.setOutlineColor(sf::Color(r,g,b,a));
}



