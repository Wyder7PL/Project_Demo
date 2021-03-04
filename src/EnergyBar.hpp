#pragma once

#include "Energy.hpp"
#include "graphic/BorderedBar.hpp"

namespace Demo
{

class EnergyBar
{
public:
	EnergyBar();
	~EnergyBar();
	
	void Update(double delta);
	void Draw(const Canvas& canvas) const;
	
	virtual void SetPosition(const PointU&);
	virtual PointU GetPosition();
	
	void SetSize(const PointU&);
	PointU GetSize();
	
	void SetColor(const unsigned char& r,const unsigned char& g,const unsigned char& b,const unsigned char& a = 255);
	void SetColor(const unsigned int& rgba);
	
	void SetEnergy(const int&);
	int GetEnergy();
	
	void SetMaxEnergy(const int&);
	int GetMaxEnergy();
	
	double GetPercentage();
	void Increase(const int&);
	
	private:
	
	void UpdateBar();
	
	Energy energy;
	BorderedBar /***/ bar;

};

}

