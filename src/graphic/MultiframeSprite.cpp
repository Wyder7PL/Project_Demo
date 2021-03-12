#include "MultiframeSprite.hpp"

#include <cassert>

Demo::MultiframeSprite::MultiframeSprite(const PointU& tileAmount, const unsigned int& frame)
{
	SetAtlasSize(tileAmount);
	SetFrame(frame);
}

Demo::MultiframeSprite::~MultiframeSprite()
{
}

void Demo::MultiframeSprite::SetFrame(const unsigned int& frame)
{
	if(currentFrame == frame && currentFrame < framesAmount)
		return;

	currentFrame = frame;
	if(currentFrame >= framesAmount)
		currentFrame = framesAmount - 1;

	SetTextureRect();
}

void Demo::MultiframeSprite::SetAtlasSize(const PointU& tileAmount)
{
	assert((tileAmount.x * tileAmount.y > 0) && "Atlas size x*y must be greater than 0");
	
	atlasSize = tileAmount;
	framesAmount = atlasSize.x * atlasSize.y;
	
	if(sprite.getTexture() != nullptr)
		UpdateSprite();
}

void Demo::MultiframeSprite::UpdateSprite()
{
	atlasPixelSize.x = sprite.getTexture()->getSize().x;
	atlasPixelSize.y = sprite.getTexture()->getSize().y;
	
	tileSize.x = atlasPixelSize.x / atlasSize.x;
	tileSize.y = atlasPixelSize.y / atlasSize.y;
	
	SetTextureRect();
	
	Sprite::UpdateSprite();
}

void Demo::MultiframeSprite::SetTextureRect()
{
	int x = tileSize.x * (currentFrame % atlasSize.x);
	int y = tileSize.y * ( currentFrame - (currentFrame % atlasSize.x) / atlasSize.y);
	
	sf::IntRect ir;
	
	ir.left = x;
	ir.top = y;
	ir.width = tileSize.x;
	ir.height = tileSize.y;
	
	sprite.setTextureRect(ir);
}
