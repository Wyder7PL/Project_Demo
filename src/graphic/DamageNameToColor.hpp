#pragma once

#include <string>

namespace Demo
{

class DamageNameToColor
{
public:
	DamageNameToColor();
	
	static int ToColor(const std::string& damageName);
};

}

