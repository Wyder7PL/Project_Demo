#pragma once

#include "BorderedBar.hpp" 
#include "Text.hpp"

namespace Demo
{

class Button : public Demo::BorderedBar
{
public:
	Button(const PointU& dimensions,std::string txt = "BUTTON");
	Button();
	virtual ~Button();
	
	virtual void Draw(const Canvas&) const override;
	
	virtual void SetPosition(const PointU& );
	virtual void SetRotation(const double& angle);
	
	virtual void SetVisibility(const bool&) override;
	
	virtual void UpdateBar();
	void UpdateText(const std::string& txt);
	
	bool IsMouseOnButton(const PointU&);
	
protected:
	
private:
	Text text;
};

}

