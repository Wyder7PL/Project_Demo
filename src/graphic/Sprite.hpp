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
	
protected:
	virtual void HiddenDraw(sf::RenderTarget& ) const override;
	virtual void UpdateSprite();
	
	friend class TextureHolder;
	sf::Sprite sprite;
};

}

