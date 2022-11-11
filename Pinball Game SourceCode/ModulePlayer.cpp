#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "p2Point.h"
#include "p2List.h"
//#include "ModuleFadeToBlack.h"


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
	texturePath = ("Assets/Textures/sprite_sheet.png");
	texture = App->textures->Load(texturePath);

	startPos.x = 689;
	startPos.y = 591;
	
	position.x = startPos.x;
	position.y = startPos.y;

	idleAnim.PushBack({ 138, 1, 27, 27 });

	rollingAnim.PushBack({ 138, 1, 27, 27 });
	rollingAnim.PushBack({ 168, 1, 27, 27 });
	rollingAnim.PushBack({ 199, 1, 27, 27 });
	rollingAnim.PushBack({ 229, 1, 27, 27 });
	rollingAnim.loop = true;
	rollingAnim.speed = 0.4f;

	//Add physics to the player - initialize physics body
	pbody = App->physics->CreateCircle(position.x, position.y, 13, b2_dynamicBody, ColliderType::BALL);
	pbody->listener = this;

	velocity.y = -GRAVITY_Y;
	pbody->body->SetLinearVelocity(velocity);

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
	currentAnim = &idleAnim;
	
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
		currentAnim = &rollingAnim;
	
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		b2Vec2 impulse = { 0, -1 };
		
		pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
	}

	//Flippers
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->scene_intro->rightFlipper->body->ApplyTorque(80.0f, true);
	else
		App->scene_intro->rightFlipper->body->ApplyTorque(-15.0f, true);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->scene_intro->leftFlipper->body->ApplyTorque(-80.0f, true);
	else
		App->scene_intro->leftFlipper->body->ApplyTorque(15.0f, true);





	/* Link player's texture with pbody when moving */
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (27 / 2));
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (27 / 2));

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	App->renderer->Blit(texture, position.x, position.y, &rect);
	currentAnim->Update();

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->cType)
	{
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::FLIPPER:
		LOG("Collision FLIPPER");
		break;
	case ColliderType::THROUGH:
		LOG("Collision THROUGH");
		lives--;
		break;
	case ColliderType::BUMPER:
		LOG("Collision BUMPER");
		break;
	case ColliderType::SLINGSHOT:
		LOG("Collision SLINGSHOT");
		break;
	case ColliderType::BONUS:
		LOG("Collision BONUS");
		break;
	case ColliderType::_100PTS:
		LOG("Collision 100pts");
		score += 100;
		highscore += 100;
		break;
	case ColliderType::_200PTS:
		LOG("Collision 200pts");
		score += 200;
		highscore += 200 ;
		break;
	case ColliderType::ROLLOVER:
		LOG("Collision ROLLOVER");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}

