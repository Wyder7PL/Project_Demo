#pragma once

#include "graphic/Canvas.hpp"
#include "Utility.hpp"

namespace Demo
{

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();
	
	virtual void Update(double delta) = 0;
	virtual void Draw(const Canvas& canvas) const = 0;
	
	unsigned long GetID();
	
	virtual void SetPosition(const PointU& pos);
	virtual PointU GetPosition();
	
protected:
	PointU position;
	
private:
	static unsigned long higestID;
	unsigned long id;
};

}

