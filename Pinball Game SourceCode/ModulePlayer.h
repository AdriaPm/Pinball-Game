#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Animation.h"
#include "ModulePhysics.h"
#include "SDL/include/SDL.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void ModulePlayer::OnCollision(PhysBody* physA, PhysBody* physB);

public:

	Animation* currentAnim;
	Animation ballAnim;

	iPoint position;

private:

	bool godMode;
	bool alive;
	PhysBody* pbody;

	SDL_Texture* texture;
	const char* texturePath;

	b2Vec2 startPos;

};