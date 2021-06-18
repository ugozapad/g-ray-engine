#include "stdafx.h"
#include "game.h"

#include "objectfactory.h"

#include "actor.h"
#include "weaponrail.h"
#include "staticobject.h"

#include "engine/luascriptmanager.h"

class Game : public IGame
{
public:
	void Init();
	void Shutdown();

	void Update(float dt);
	void Render();

};

void Game::Init()
{
	GetObjectFactory()->RegisterObject<Actor>("Actor");
	GetObjectFactory()->RegisterObject<WeaponRail>("WeaponRail");
	GetObjectFactory()->RegisterObject<StaticObject>("StaticObject");
}

void Game::Shutdown()
{

}

void Game::Update(float dt)
{

}

void Game::Render()
{

}

IGame* GetGame()
{
	static Game s_Game;
	return (IGame*)&s_Game;
}
