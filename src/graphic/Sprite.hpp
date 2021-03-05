#pragma once

#include "Graphic.hpp"

#include <SFML/Graphics.hpp>

namespace Demo
{

class Sprite : public Demo::Graphic
{
public:
	Sprite();
	virtual ~Sprite();

public:
	virtual void SetPosition(const PointU&) override;
	virtual PointU GetPosition() override;
	
	virtual void SetRotation(const double& angle) override;
	virtual double GetRotation() override;
	
	void SetSize(const PointU&);
	PointU GetSize();
	
	void SetColorMultiply(const unsigned char& r,const unsigned char& g,const unsigned char& b,const unsigned char& a = 255);
	void SetColorMultiply(const unsigned int& rgba);
	
protected:
	virtual void HiddenDraw(sf::RenderTarget& ) const override;
	virtual void UpdateSprite();
	
	friend class TextureHolder;
	sf::Sprite sprite;
};

}

