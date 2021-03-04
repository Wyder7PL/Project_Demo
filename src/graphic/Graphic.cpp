#include "Graphic.hpp"

Demo::Graphic::Graphic()
:visible(true)
{
}

Demo::Graphic::~Graphic()
{
}

void Demo::Graphic::Draw(const Demo::Canvas& canvas) const
{
	if(visible)
		HiddenDraw(canvas.windowReference);
}

void Demo::Graphic::SetVisibility(const bool& v)
{
	visible = v;
}

bool Demo::Graphic::IsVisible()
{
	return visible;
}
