#pragma once

#include "Canvas.hpp"
#include "../Utility.hpp"

#include <SFML/Graphics.hpp>

namespace Demo
{

	class Graphic
	{
		
	public:
		Graphic();
		virtual ~Graphic();
		
		virtual void Draw(const Canvas&) const;
		
		virtual void SetPosition(const PointU&) = 0;
		virtual PointU GetPosition() = 0;
		
		virtual void SetRotation(const double& angle) = 0;
		virtual double GetRotation() = 0;
		
		virtual void SetVisibility(const bool&);
		bool IsVisible();
		
	protected:
		virtual void HiddenDraw(sf::RenderTarget&) const = 0;
	private:
		bool visible;
	};

}

