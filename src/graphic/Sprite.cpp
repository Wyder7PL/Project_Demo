#include "Sprite.hpp"

Demo::Sprite::Sprite()
{
}

Demo::Sprite::~Sprite()
{
}


void Demo::Sprite::SetPosition(const PointU& pos)
{
	sprite.setPosition(pos.x,pos.y);
}
Demo::PointU Demo::Sprite::GetPosition()
{
	return PointU(sprite.getPosition().x,sprite.getPosition().y);
}

void Demo::Sprite::SetRotation(const double& angle)
{
	sprite.setRotation(angle);
}

double Demo::Sprite::GetRotation()
{
	return sprite.getRotation();
}

void Demo::Sprite::SetSize(const PointU& size)
{
	sf::FloatRect dim = sprite.getLocalBounds();
	if(dim.width == 0 || dim.height == 0)
		return; //TODO error
	sprite.setScale(size.x/dim.width,size.y/dim.height);
	
	UpdateSprite();
}

Demo::PointU Demo::Sprite::GetSize()
{
	sf::FloatRect dim = sprite.getLocalBounds();
	sf::Vector2f sca = sprite.getScale();
	return PointU(dim.width*sca.x, dim.height*sca.y);
}

void Demo::Sprite::HiddenDraw(sf::RenderTarget& target) const
{
	target.draw(sprite);
}

void Demo::Sprite::UpdateSprite()
{
	sf::FloatRect ori = sprite.getLocalBounds();
	sprite.setOrigin(ori.width/2,ori.height/2);
}
