#pragma once

#include "Graphic.hpp"
#include "../Utility.hpp"

namespace Demo
{

class Bar : public Graphic
{
public:
	explicit Bar(const PointU& dimensions);
	virtual ~Bar();
		
	virtual void SetPosition(const PointU&) override;
	virtual PointU GetPosition() override;
		
	virtual void SetRotation(const double& angle) override;
	virtual double GetRotation() override;
	
	void SetColor(const unsigned char& r,const unsigned char& g,const unsigned char& b,const unsigned char& a = 255);
	
	void SetSize(const PointU&);
	PointU GetSize();
	
	void SetPercentage(const double& percent);
	
protected:
	virtual void HiddenDraw(sf::RenderTarget&) const override;
	virtual void UpdateBar();
	
private:

	PointU size;
	double fillPercentage;
	sf::RectangleShape shape;

};

}

