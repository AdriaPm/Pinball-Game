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
#include "FadeToBlack.h"


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
	//Load tex
	texture = App->textures->Load("Assets/Textures/sprite_sheet.png");

	//Load SFX
	flipper_sfx = App->audio->LoadFx("Assets/Audio/FX/flipper.wav");
	bonus_sfx = App->audio->LoadFx("Assets/Audio/FX/bonus.wav");
	bumper_sfx = App->audio->LoadFx("Assets/Audio/FX/bumper.wav");
	FlipperPush_sfx = App->audio->LoadFx("Assets/Audio/FX/FlipperPush.wav");
	extraLife_sfx = App->audio->LoadFx("Assets/Audio/FX/extraLife.wav");
	multiplierActivation_sfx = App->audio->LoadFx("Assets/Audio/FX/multiplierActivation.wav");
	resetBall_sfx = App->audio->LoadFx("Assets/Audio/FX/resetBall.wav");

	startPos.x = 689;
	startPos.y = 588;
	
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

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		App->player->ResetPosition();
	
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		b2Vec2 impulse = { 0, -1 };
		
		pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
	}

	//Flippers' input
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		App->scene_intro->rightFlipper->body->ApplyTorque(200.0f, true);
	}
	else
		App->scene_intro->rightFlipper->body->ApplyTorque(-50.0f, true);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT){
		App->scene_intro->leftFlipper->body->ApplyTorque(-200.0f, true);
	}
	else
		App->scene_intro->leftFlipper->body->ApplyTorque(50.0f, true);

	if (deadBall) {
		
		deadTime++;
			if (deadTime >= 60)
			{
				ResetPosition();
			}
		
	}

	//Bonus function (1 extra life if score 1000)
	App->player->Bonus();

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
	//Bumper's vector impulse when ball hits one of them
	//b2Vec2 bumperImpulse = { (velocity.x *= -1) * RESTITUTION_COEF, (velocity.y *= -1) * RESTITUTION_COEF };
	
	switch (physB->cType)
	{
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::FLIPPER:
		LOG("Collision FLIPPER");
		App->audio->PlayFx(flipper_sfx);
		break;
	case ColliderType::THROUGH:
		LOG("Collision THROUGH");
		lives--;
		deadBall = true;
		if (lives == 0)
		{
			App->fade->FadeBlack(App->scene_intro, (Module*)App->ending_screen, 90);
		}
		break;
	case ColliderType::BUMPER:
		LOG("Collision BUMPER");
		bounceImpulse = { pbody->body->GetWorldCenter() - physB->body->GetWorldCenter() };
		bounceImpulse.Normalize();
		pbody->body->ApplyLinearImpulse(RESTITUTION_COEF * bounceImpulse, pbody->body->GetWorldCenter(), true);
		App->audio->PlayFx(bumper_sfx);
		break;
	case ColliderType::RIGHTDOWN_SLINGSHOT:
		LOG("Collision Right Down Slingshot");
		bounceImpulse = { -1.0f, -0.5f };
		bounceImpulse.Normalize();
		bounces++;
		if (bounces >= 3) {
			bounceImpulse = { -5.0f, -3.0f };
			bounces = 0;
		}
		pbody->body->ApplyLinearImpulse(RESTITUTION_COEF * bounceImpulse, pbody->body->GetWorldCenter(), true);
		App->audio->PlayFx(bumper_sfx);
		break;
	case ColliderType::LEFTDOWN_SLINGSHOT:
		LOG("Collision Left Down Slingshot");
		bounceImpulse = { 1.0f, -0.5f };
		bounceImpulse.Normalize();
		bounces++;
		if (bounces >= 3) {
			bounceImpulse = { -5.0f, -3.0f };
			bounces = 0;
		}
		pbody->body->ApplyLinearImpulse(RESTITUTION_COEF * bounceImpulse, pbody->body->GetWorldCenter(), true);
		App->audio->PlayFx(bumper_sfx);
		break;
	case ColliderType::RIGHTUP_SLINGSHOT:
		LOG("Collision Right Up Slingshot");
		bounceImpulse = { -7.0f, -22.0f };
		bounceImpulse.Normalize();
		bounces++;
		if (bounces >= 3) {
			bounceImpulse = { -5.0f, -3.0f };
			bounces = 0;
		}
		pbody->body->ApplyLinearImpulse(RESTITUTION_COEF * bounceImpulse, pbody->body->GetWorldCenter(), true);
		App->audio->PlayFx(bumper_sfx);
		break;
	case ColliderType::LEFTUP_SLINGSHOT:
		LOG("Collision Left Up Slingshot");
		bounceImpulse = { 7.0f, -22.0f };
		bounceImpulse.Normalize();
		bounces++;
		if (bounces >= 3) {
			bounceImpulse = { 5.0f, -3.0f };
			bounces = 0;
		}
		pbody->body->ApplyLinearImpulse(RESTITUTION_COEF * bounceImpulse, pbody->body->GetWorldCenter(), true);
		App->audio->PlayFx(bumper_sfx);
		break;
	case ColliderType::BONUS:
		LOG("Collision BONUS");
		break;
	case ColliderType::_100PTS:
		LOG("Collision 100pts");
		if (multiplierx2IsActive == true)
		{
			score += 100*2;
			if (highscore + 100 <= score)
				highscore += 100*2;
		}
		else if (multiplierx3IsActive == true) 
		{
			score += 100*3;
			if (highscore + 100 <= score)
				highscore += 100*3;
		}
		else 
		{
			score += 100;
			if (highscore + 100 <= score)
				highscore += 100;
		}
		App->audio->PlayFx(bonus_sfx);
		break;
	case ColliderType::_200PTS:
		LOG("Collision 200pts");
		if (multiplierx2IsActive == true)
		{
			score += 200 * 2;
			if (highscore + 100 <= score)
				highscore += 200 * 2;
		}
		else if (multiplierx3IsActive == true)
		{
			score += 200 * 3;
			if (highscore + 100 <= score)
				highscore += 200 * 3;
		}
		else 
		{
			score += 200;
			if (highscore + 100 <= score)
				highscore += 200;
		}
		App->audio->PlayFx(bonus_sfx);
		break;
	case ColliderType::x2:
		LOG("Collision x2 Multiplier");
		multiplierx2IsActive = true;
		App->audio->PlayFx(multiplierActivation_sfx);
		break;
	case ColliderType::x3:
		LOG("Collision x3 Multiplier");
		multiplierx3IsActive = true;
		App->audio->PlayFx(multiplierActivation_sfx);
		break;
	case ColliderType::ROLLOVER:
		LOG("Collision ROLLOVER");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}


void ModulePlayer::ResetPosition() {

	//Reset ball's position to initial position
	LOG("Reseting BALL POSITION");
	pbody->body->SetTransform(PIXEL_TO_METERS(startPos), 0);
	deadBall = false;
	deadTime = 0;
	App->audio->PlayFx(resetBall_sfx);
}

void ModulePlayer::Bonus() {

	if (score >= 1000 && bonusIsActive == false)
	{
		LOG("BONUS! 1 extra life");
		lives++;
		bonusIsActive = true;
		App->audio->PlayFx(extraLife_sfx);
	}
}