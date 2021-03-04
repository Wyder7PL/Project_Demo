#pragma once

#include "BattleData.hpp"
#include "controllers/Controller.hpp"
#include "graphic/Canvas.hpp"
#include "Location.hpp"
#include "Action.hpp"

#include <vector>
#include <memory>
#include <chrono>
#include <sstream>


#include "input/PlayerInput.hpp"

namespace Demo
{

	class Battle
	{
	public:
		Battle();
		virtual ~Battle();
		
		// Represent one tick, can be adjusted to the framerate so
		// it's recomended to call this function more often than actually need
		// (when framerate is 60, calling this function 265 times in a second will result only in 60 ticks)
		virtual void Update(double delta);
		
		// Draw one frame, should be called once before calling display
		virtual void Draw(const Canvas&) const;
		
		// Used to set the limit framerate of updating function
		// Passing 0 as the argument result in unlimited framerate
		void SetFramerate(const unsigned int& f);
		
		void AddPawn(BattlePawn pawn,const Location& location);
		BattlePawn& GetPawn(const Location& location);
		
		virtual void CreatePlayerController(std::weak_ptr<Input> ptr);
		
		bool BattleEnded();
		
	protected:
		BattleData data;
		std::vector<std::unique_ptr<Controller>> controllers;
		

	private:
		void UpdateDamageDisplay(const double& delta);
		
		void ExtractActionsFromPawns();
		void ManageActions();
		void ManageSingleAction(const Action& action);
		void ManageAreaEffectActions(const Action& action);
		void RemoveDestroyedPawns();
		
		bool IsActionAreaEffect(const Action& action);
		bool IsAreaEffectHittingPointedLocation(const Action& action);
		bool IsInFloorRange(const Location& actionLocation,const PointI& shift, PointU& newPoint);
	
		void DealDamageToPawn(const std::string& attackData,const Location& attackLocation);
		void GiveSupportToPawn(const std::string& supportData,const Location& supportLocation);
		void ApplyEffectToPawn(const std::string& effectData,const Location& effectLocation);
		void MovePawn(const std::string& data, const Location& from, const Location& to, bool forced = false);
		bool CreatePawnAction(const std::string& data,const Location& creationLocation);
	
		unsigned int fps;
		double updateCooldown;
		double lastTick;
		double lastUpdateCall;
		unsigned int drawRate;
		
		std::vector<Action> pawnActions;
	};

}

