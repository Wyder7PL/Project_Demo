#include "Button.hpp"

Demo::Button::Button(const PointU& dimensions, std::string txt)
:BorderedBar(dimensions)
,text(std::move(txt),8)
{
}

Demo::Button::Button()
:Button(PointU(0,0),"BUTTON")
{
}

Demo::Button::~Button()
{
}

void Demo::Button::Draw(const Canvas& canvas) const
{
	Graphic::Draw(canvas);
	text.Draw(canvas);
}

void Demo::Button::SetPosition(const PointU& point)
{
	BorderedBar::SetPosition(point);
	text.SetPosition(point);
}

void Demo::Button::SetRotation(const double& angle)
{
	BorderedBar::SetRotation(angle);
	text.SetRotation(angle);
}

void Demo::Button::SetVisibility(const bool& b)
{
	Graphic::SetVisibility(b);
	text.SetVisibility(b);
}

void Demo::Button::UpdateBar()
{
	BorderedBar::UpdateBar();
	int fontSize = GetSize().y / 2.0;
	if(fontSize < 8)
		fontSize = 8;
	text.SetFontSize(fontSize);
}

void Demo::Button::UpdateText(const std::string& txt)
{
	text.UpdateText(txt);
}

bool Demo::Button::IsMouseOnButton(const PointU& point)
{
	PointU siz = PointU(GetSize().x/2.0,GetSize().y/2.0);
	
	if(point.x >= GetPosition().x - siz.x && point.x < GetPosition().x + siz.x)
		if(point.y >= GetPosition().y - siz.y && point.y < GetPosition().y + siz.y)
			return true;
	return false;
}
