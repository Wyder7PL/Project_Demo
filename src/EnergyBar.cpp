#include "EnergyBar.hpp"

Demo::EnergyBar::EnergyBar()
:bar(PointU(0,0))
{
}

Demo::EnergyBar::~EnergyBar()
{
}

void Demo::EnergyBar::Update(double delta)
{
	
}

void Demo::EnergyBar::Draw(const Canvas& canvas) const
{
	bar.Draw(canvas);
}

void Demo::EnergyBar::SetPosition(const PointU& p)
{
	bar.SetPosition(p);
}

Demo::PointU Demo::EnergyBar::GetPosition()
{
	return bar.GetPosition();
}

void Demo::EnergyBar::SetSize(const PointU& d)
{
	bar.SetSize(d);
}

Demo::PointU Demo::EnergyBar::GetSize()
{
	return bar.GetSize();
}

void Demo::EnergyBar::SetColor(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	bar.SetColor(r,g,b,a);
}

void Demo::EnergyBar::SetColor(const unsigned int& rgba)
{
	const unsigned char r = (rgba&0xFF000000) >> (8 * 3);
	const unsigned char g = (rgba&0x00FF0000) >> (8 * 2);
	const unsigned char b = (rgba&0x0000FF00) >> (8 * 1);
	const unsigned char a = (rgba&0x000000FF) >> (8 * 0);
	SetColor(r,g,b,a);
}

void Demo::EnergyBar::SetEnergy(const int& e)
{
	energy.amount = e;
	UpdateBar();
}

int Demo::EnergyBar::GetEnergy()
{
	return energy.amount;
}

void Demo::EnergyBar::SetMaxEnergy(const int& me)
{
	energy.maxAmount = me;
	UpdateBar();
}

int Demo::EnergyBar::GetMaxEnergy()
{
	return energy.maxAmount;
}

double Demo::EnergyBar::GetPercentage()
{
	if(energy.maxAmount == 0.0)
		return 0;
	return energy.amount * 100.0 / energy.maxAmount;
}

void Demo::EnergyBar::Increase(const int& i)
{
	energy.amount += i;
	if(energy.amount > energy.maxAmount)
		energy.amount = energy.maxAmount;
	UpdateBar();
}

void Demo::EnergyBar::UpdateBar()
{
	if(energy.maxAmount > 0)
		if(energy.amount < 0)
			bar.SetPercentage(0.0);
		else
			bar.SetPercentage(100.0*energy.amount/energy.maxAmount);
	else
		bar.SetPercentage(0.0);
}
