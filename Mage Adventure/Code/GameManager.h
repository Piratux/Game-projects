#pragma once

#include "olcPixelGameEngine.h"
#include "EntityManager.h"
#include "AssetManager.h"
#include "PhysicsEngine.h"

#include "olcPGEX_SplashScreen.h"

class RPG : public olc::PixelGameEngine
{
private:
	enum class State
	{
		LoadScreen, Initialise, SplashScreen, Game
	};
	enum Layer
	{
		GUI, // for UI
		Debug, // for additional debug info like hitboxes or colliders
		Game // everything else
	};

private:
	State state = State::LoadScreen;

	olcPGEX_SplashScreen* splash_screen;

	std::unique_ptr<PhysicsEngine> physics_engine;
	std::unique_ptr<AssetManager> asset_manager;
	std::unique_ptr<EntityManager> entity_manager; // must be defined after PhysicsEngine
	std::unique_ptr<DebugDraw> debug_draw; // draws physics engine bodies

	uint8_t total_layers = 1;

	// temporary
	std::map<std::string, std::unique_ptr<olc::Renderable>>::iterator sprite_iterator;
	Entity* player;

	bool debug_view = false;

public:
	RPG();

private:
	bool OnUserCreate() override;
	bool OnUserUpdate(float delta) override;

	// state functions
	bool LoadScreenState();
	bool InitialiseState();
	bool SplashScreenState(float delta);
	bool GameState(float delta);

	void DebugCreate();
	void DebugUpdate(float delta);
};