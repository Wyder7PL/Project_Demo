#pragma once

#include "Sprite.hpp"
#include "../Utility.hpp"

namespace Demo
{

class MultiframeSprite : public Sprite
{
public:
	MultiframeSprite(const PointU& tileAmount = PointU(1,1), const unsigned int& frame = 0);
	virtual ~MultiframeSprite();
	
	virtual void SetFrame(const unsigned int& frame);
	
	void SetAtlasSize(const PointU& tileAmount);
	
protected:
	//virtual void HiddenDraw(sf::RenderTarget& ) const override;
	virtual void UpdateSprite();
	
private:
	void SetTextureRect();
	
	unsigned int framesAmount;
	unsigned int currentFrame;
	
	PointU atlasSize = PointU(1,1);
	PointU atlasPixelSize = PointU(0,0);
	PointU tileSize = PointU(0,0);
	
};

}

