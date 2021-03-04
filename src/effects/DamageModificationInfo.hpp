#pragma once

#include <vector>
#include <math.h>  

namespace Demo
{
	struct DamageModificationInfo
	{
		DamageModificationInfo(double a,std::string dtype)
		:amount(a),damageType(dtype),heal(a < 0.0) {}
		
		DamageModificationInfo() {DamageModificationInfo(0.0,"");}
		
		int GetModifiedDamage()
		{
			double res = amount;
			
			res += preAddition;
			for(double& i : multiply) 
				res *= i;
			res += postAddition;
			
			if( (!heal && amount < 0.0) || (heal && amount > 0.0) )
				res = 0.0;
			
			return (int)round(res);
		}
		
		double amount;
		std::string damageType;
		bool neutralized = false;
		bool heal = false;
		double preAddition = 0;; // add before multiply
		std::vector<double> multiply = {};
		double postAddition = 0; // add after multiply
	};
}