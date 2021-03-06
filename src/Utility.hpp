#pragma once

#include <random>

namespace Demo
{
	template <typename T>
	struct Point
	{
		T x;
		T y;
		Point<T>(const T& X, const T& Y)
		:x(X),y(Y) {}
		bool operator==(const Point<T>& val)
		{return this->x == val.x && this->y == val.y;}
		
		bool operator!=(const Point<T>& val)
		{return !(*this == val);}
	};
	
	template <typename T>
	T RandomReal(const T& min, const T& max)
	{
		static std::default_random_engine engine;
		std::uniform_real_distribution<T> distribution(min,max);
		return distribution(engine);
	}
	
	template <typename T>
	T RandomInt(const T& min, const T& max)
	{
		static std::default_random_engine engine;
		std::uniform_int_distribution<T> distribution(min,max);//[min,max]
		return distribution(engine);
	}
	
	using PointU = Point<unsigned int>;
	using PointI = Point<int>;
	using PointF = Point<float>;
	using PointD = Point<double>;
	using PointB = Point<bool>;
}