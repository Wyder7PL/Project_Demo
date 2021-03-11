#include "DefaultAbilityDamageDisplay.hpp"
#include "TextureHolder.hpp"


Demo::DefaultAbilityDamageDisplay::DefaultAbilityDamageDisplay()
:textAllignLeft(false)
{
}

Demo::DefaultAbilityDamageDisplay::DefaultAbilityDamageDisplay(const std::string& textureName, const int& amount)
{
	SetIcon(textureName);
	UpdateText(std::to_string(amount));
	SetTextColor(DamageNameToColor::ToColor(textureName));
}

Demo::DefaultAbilityDamageDisplay::~DefaultAbilityDamageDisplay()
{
}

void Demo::DefaultAbilityDamageDisplay::Draw(const Canvas& canvas) const
{
	Graphic::Draw(canvas);
	icon.Draw(canvas);
	number.Draw(canvas);
}

void Demo::DefaultAbilityDamageDisplay::SetPosition(const PointU& pos)
{
	icon.SetPosition(pos);
	if(textAllignLeft)
		number.SetPosition(PointU(pos.x - number.GetWidth()/2,pos.y));
	else
		number.SetPosition(PointU(pos.x + number.GetWidth()/2,pos.y));
}

Demo::PointU Demo::DefaultAbilityDamageDisplay::GetPosition()
{
	return icon.GetPosition();
}

void Demo::DefaultAbilityDamageDisplay::SetRotation(const double& angle)
{
	
}

double Demo::DefaultAbilityDamageDisplay::GetRotation()
{
	return 0.0;
}

void Demo::DefaultAbilityDamageDisplay::SetSize(const unsigned int& s)
{
	icon.SetSize(PointU(s,s));
	number.SetFontSize(s);
}

void Demo::DefaultAbilityDamageDisplay::UpdateText(const std::string& txt)
{
	number.UpdateText(txt);
}

void Demo::DefaultAbilityDamageDisplay::SetTextColor(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	number.SetFontColor(r,g,b,a);
}

void Demo::DefaultAbilityDamageDisplay::SetTextColor(const unsigned int& rgba)
{
	const unsigned char r = (rgba&0xFF000000) >> (8 * 3);
	const unsigned char g = (rgba&0x00FF0000) >> (8 * 2);
	const unsigned char b = (rgba&0x0000FF00) >> (8 * 1);
	const unsigned char a = (rgba&0x000000FF) >> (8 * 0);
	SetTextColor(r,g,b,a);
}

void Demo::DefaultAbilityDamageDisplay::SetIcon(const std::string& textureName)
{
	TextureHolder::GetInstance().SetSpriteTexture(icon,textureName);
}

void Demo::DefaultAbilityDamageDisplay::SetTextAllign(bool allignFromLeftSide)
{
	textAllignLeft = allignFromLeftSide;
}

void Demo::DefaultAbilityDamageDisplay::HiddenDraw(sf::RenderTarget& ) const
{
	
}