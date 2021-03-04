#include "Bar.hpp"

Demo::Bar::Bar(const PointU& dimensions)
:size(dimensions.x,dimensions.y)
,fillPercentage(100.0)
,shape(sf::Vector2f(dimensions.x,dimensions.y))
{
	shape.setOrigin(dimensions.x/2,dimensions.y/2);
}

Demo::Bar::~Bar()
{
}

void Demo::Bar::SetPosition(const PointU& point)
{
	shape.setPosition(point.x,point.y);
}
Demo::PointU Demo::Bar::GetPosition()
{
	return PointU(shape.getPosition().x,shape.getPosition().y);
}

void Demo::Bar::SetRotation(const double& angle)
{
	shape.setRotation(angle);
}

double Demo::Bar::GetRotation()
{
	return shape.getRotation();
}

void Demo::Bar::SetColor(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	shape.setFillColor(sf::Color(r,g,b,a));
}

void Demo::Bar::SetSize(const PointU& val)
{
	size = val;
	UpdateBar();
}
Demo::PointU Demo::Bar::GetSize()
{
	return size;
}

void Demo::Bar::SetPercentage(const double& percent)
{
	if(percent < 0.0)
		return; //error
	fillPercentage = percent;
	UpdateBar();
}

void Demo::Bar::HiddenDraw(sf::RenderTarget& target) const
{
	target.draw(shape);
}

void Demo::Bar::UpdateBar()
{
	shape.setSize(sf::Vector2f(size.x*fillPercentage/100.0,size.y));
	shape.setOrigin(size.x/2,size.y/2);
}
