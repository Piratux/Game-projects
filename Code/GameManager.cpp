#include "GameManager.h"

RPG::RPG()
{
	sAppName = "RPG";
	srand(time(0));
}
bool RPG::OnUserCreate()
{
	return true;
}
bool RPG::OnUserUpdate(float delta)
{
	switch (state)
	{
	case State::LoadScreen:
		return LoadScreenState();
		break;
	
	case State::Initialise:
		return InitialiseState();
		break;
	
	case State::SplashScreen:
		return SplashScreenState(delta);
		break;
	
	case State::Game:
		return GameState(delta);
		break;
	
	default: // should never happen
		throw std::logic_error("RPG::OnUserUpdate: unhandled main update state");
	}
}
bool RPG::LoadScreenState()
{
	olc::Sprite* loading_screen = new olc::Sprite("./Assets/Extra/LoadingScreen.png");
	DrawSprite(0, 0, loading_screen);
	delete loading_screen;

	state = State::Initialise;

	return true;
}
bool RPG::InitialiseState()
{
	physics_engine = std::make_unique<PhysicsEngine>();
	asset_manager = std::make_unique<AssetManager>();
	entity_manager = std::make_unique<EntityManager>(this, asset_manager.get(), physics_engine.get());

	debug_draw = std::make_unique<DebugDraw>(this, physics_engine.get(), &entity_manager->camera->pos);

	CreateLayer(); // Debug layer
	CreateLayer(); // Game layer

	EnableLayer(Debug, true);
	++total_layers;
	EnableLayer(Game, true);
	++total_layers;

	DebugCreate();

	state = State::SplashScreen;

	return true;
}
bool RPG::SplashScreenState(float delta)
{
	// upon release uncomment it
	/*if (splash_screen == nullptr)
		splash_screen = new olcPGEX_SplashScreen();

	if (splash_screen->AnimateSplashScreen(delta) == false)
	{
		delete splash_screen;
		state = State::Game;
	}*/

	state = State::Game;

	return true;
}
bool RPG::GameState(float delta)
{
	// clear all layers
	for (uint8_t layer = 0; layer < total_layers; ++layer)
	{
		SetDrawTarget(layer);
		Clear(olc::BLANK);
	}

	SetDrawTarget(Game);
	entity_manager->Update(delta);
	physics_engine->Update();

	if (debug_view)
	{
		SetDrawTarget(Debug);
		debug_draw->Update();
	}

	DebugUpdate(delta);

	return true;
}
void RPG::DebugCreate()
{
	sprite_iterator = asset_manager->sprites.begin();
	player = entity_manager->entities.front().get();
}
void RPG::DebugUpdate(float delta)
{
	// change spritesheets with mouse scroll
	if (GetMouseWheel() > 0)
	{
		sprite_iterator++;
		if (sprite_iterator == asset_manager->sprites.end())
		{
			sprite_iterator = asset_manager->sprites.begin();
		}

		player->SetAnimation(sprite_iterator->second.get(), asset_manager->GetAnimationSheet(Animation::Type::Hero_10x5));
	}
	if (GetMouseWheel() < 0)
	{
		if (sprite_iterator == asset_manager->sprites.begin())
		{
			sprite_iterator = asset_manager->sprites.end();
		}
		sprite_iterator--;

		player->SetAnimation(sprite_iterator->second.get(), asset_manager->GetAnimationSheet(Animation::Type::Hero_10x5));
	}

	if (GetKey(olc::B).bPressed)
		debug_view = !debug_view;


	SetDrawTarget(Layer::Debug);
	std::string display_message =
		"WASD/arrows - move around\n"
		"B - toggle debug view\n"
		"Left mouse button - shoot projectile\n"
		"Right mouse button - test special animation\n"
		"Scroll - scroll through different spritesheets\n"
		"5 - test death animation\n";
	DrawStringProp({ 10, 10 }, display_message, olc::RED);
}