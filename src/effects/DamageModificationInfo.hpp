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
			
			for(double& i : preAddition) 
				res += i;
			for(double& i : multiply) 
				res *= i;
			for(double& i : postAddition) 
				res += i;
			
			if( (!heal && amount < 0.0) || (heal && amount > 0.0) )
				res = 0.0;
			
			return (int)round(res);
		}
		
		double amount;
		std::string damageType;
		bool neutralized = false;
		bool heal = false;
		std::vector<double> preAddition = {}; // add before multiply
		std::vector<double> multiply = {};
		std::vector<double> postAddition = {}; // add after multiply
	};
}