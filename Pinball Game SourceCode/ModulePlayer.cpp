#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	alive = true;
	godMode = false;
	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* physA, PhysBody* physB)
{
	/*switch (physB->ctype)
	{
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::PADDEL:
		LOG("Collision GROUND");
		break;
	case ColliderType::DEATH:
		LOG("Collision DEATH");
		alive = false;
		break;
	case ColliderType::BUMPERS:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::SLINGSHOT:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::SLINGSHOT:
	LOG("Collision UNKNOWN");
	break;
	}*/

}

