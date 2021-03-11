#include "DamageNameToColor.hpp"

Demo::DamageNameToColor::DamageNameToColor()
{
}

int Demo::DamageNameToColor::ToColor(const std::string& damageName)
{
	if(damageName == "Pain")
		return 0xC80000FF;
	return 0;
}
