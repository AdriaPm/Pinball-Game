#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleUI.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	// Initialise all the internal class variables, at least to NULL pointer
	//circle = box = rick = NULL;
	ray_on = false;
	sensed = false;

}

ModuleSceneIntro::~ModuleSceneIntro()
{
	App->player->Disable();
	App->physics->Disable();
}

bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//Enable modules/entities
	App->physics->Enable();
	App->player->Enable();
	
	pause = false;

	//Reset player parameters
	App->player->previousScore = App->player->score;
	App->player->score = 0;
	App->player->bonusIsActive = false;

	// Set camera position
	App->renderer->camera.x = App->renderer->camera.y = 0;

	// Load textures
	texture = App->textures->Load("Assets/Textures/sprite_sheet.png");
	leftFlipperTex = App->textures->Load("Assets/Textures/leftFlipper.png");
	rightFlipperTex = App->textures->Load("Assets/Textures/rightFlipper.png");
	scene = App->textures->Load("Assets/Textures/Pinball_Scene_FINAL.png");

	//Load SFX
	kicker_sfx = App->audio->LoadFx("Assets/Audio/FX/kicker2.wav");
	multiplierDeactivation_sfx = App->audio->LoadFx("Assets/Audio/FX/multiplierDeactivation.wav");
	buttonClick_sfx = App->audio->LoadFx("Assets/Audio/FX/buttonClick.wav");

	//Load Music
	App->audio->PlayMusic("Assets/Audio/Music/song.ogg", 1.0f);
	
	//Load animations
	flipperUp.PushBack({ 0, 74, 22, 56});
	
	flipperCompressing.PushBack({ 36, 74, 22, 56});
	flipperCompressing.PushBack({ 70, 74, 22, 56});
	flipperCompressing.PushBack({ 104, 74, 22, 56});
	flipperCompressing.PushBack({ 138, 74, 22, 56});
	flipperCompressing.PushBack({ 172, 74, 22, 56});
	flipperCompressing.PushBack({ 206, 74, 22, 56});
	flipperCompressing.PushBack({ 240, 74, 22, 56});
	flipperCompressing.PushBack({ 274, 74, 22, 56});
	flipperCompressing.PushBack({ 308, 74, 22, 56});
	flipperCompressing.PushBack({ 342, 74, 22, 56});
	flipperCompressing.PushBack({ 376, 74, 22, 56});
	flipperCompressing.PushBack({ 410, 74, 22, 56});
	flipperCompressing.PushBack({ 444, 74, 22, 56});
	flipperCompressing.PushBack({ 478, 74, 22, 56});
	flipperCompressing.PushBack({ 512, 74, 22, 56});
	flipperCompressing.PushBack({ 546, 74, 22, 56});
	flipperCompressing.PushBack({ 580, 74, 22, 56});
	flipperCompressing.PushBack({ 614, 74, 22, 56});
	flipperCompressing.PushBack({ 648, 74, 22, 56});
	flipperCompressing.PushBack({ 682, 74, 22, 56});
	flipperCompressing.PushBack({ 716, 74, 22, 56});
	flipperCompressing.PushBack({ 750, 74, 22, 56});
	flipperCompressing.PushBack({ 784, 74, 22, 56});
	flipperCompressing.PushBack({ 818, 74, 22, 56});
	flipperCompressing.PushBack({ 852, 74, 22, 56});
	flipperCompressing.loop = false;
	flipperCompressing.speed = 0.43f;

	//Flippers creation
	b2RevoluteJointDef flipperJointDef_left;
	b2RevoluteJointDef flipperJointDef_right;

	b2RevoluteJoint* flipperJoint_left;
	b2RevoluteJoint* flipperJoint_right;

	//Flippers' colliders
	leftFlipper = App->physics->CreateRectangle(266+35, 672+10, 75, 20, b2BodyType::b2_dynamicBody, ColliderType::FLIPPER);
	rightFlipper = App->physics->CreateRectangle(338+65, 672+10, 75, 20, b2BodyType::b2_dynamicBody, ColliderType::FLIPPER);

	//Flipper's joints colliders
	leftFlipperJoint = App->physics->CreateCircle(248, 679, 3, b2BodyType::b2_staticBody, ColliderType::UNKNOWN);
	rightFlipperJoint = App->physics->CreateCircle(456, 679, 3, b2BodyType::b2_staticBody, ColliderType::UNKNOWN);
	
	flipperJointDef_left.Initialize(leftFlipper->body, leftFlipperJoint->body, leftFlipperJoint->body->GetWorldCenter());
	flipperJointDef_right.Initialize(rightFlipper->body, rightFlipperJoint->body, rightFlipperJoint->body->GetWorldCenter());

	flipperJointDef_left.lowerAngle = -0.1f * b2_pi;
	flipperJointDef_left.upperAngle = 0.15f * b2_pi;
	flipperJointDef_right.lowerAngle = -0.15f * b2_pi;
	flipperJointDef_right.upperAngle = 0.1f * b2_pi;
	
	flipperJointDef_left.enableLimit = true;
	flipperJointDef_right.enableLimit = true;

	flipperJoint_left = (b2RevoluteJoint*)App->physics->world->CreateJoint(&flipperJointDef_left);
	flipperJoint_right = (b2RevoluteJoint*)App->physics->world->CreateJoint(&flipperJointDef_right);


	shooter = App->physics->CreateRectangle(SCREEN_WIDTH - 80, SCREEN_HEIGHT - 80, 28, 16, b2BodyType::b2_kinematicBody, ColliderType::WALL);
	shooterInitPos = shooter->body->GetWorldCenter();
	shooter->listener = this;
	distance = 0;

	/* COLLIDERS */
	//Wall Collider
	int points2[63] = { 64, 512,
					  144, 512,
					  144, 602,
					  256, 640,
					  256, 767,
						0, 767,
						0,	 0,
					  767,	 0,
					  767, 767,
					  448, 767,
					  448, 640,
					  560, 602,
					  560, 512,
					  640, 512,
					  640, 128,
					  671, 128,
					  671, 736,
					  704, 736,
					  704, 132,
					  701, 123, //First curve, first point
					  697, 112,
					   681, 93, //Half first curve
					   667, 85,
					   650, 80,	//First curve, last point
					   192, 80,	//Second curve, First point
					   153, 86,	
					   125, 99,	
					  105, 113,	//Half second curve
					   82, 142,
					   73, 162,
					   64, 196 };	//Second curve, last point
	wall = App->physics->CreateChain(0, 0, points2, 63, b2BodyType::b2_staticBody, ColliderType::WALL);

	//Death Collider
	through = App->physics->CreateRectangleSensor(SCREEN_WIDTH/2 - 32, (SCREEN_HEIGHT-32) + 16, 64*3, 1, b2BodyType::b2_staticBody, ColliderType::THROUGH);

	//100 points Collider
	App->physics->CreateCircle(298, 244, 14, b2BodyType::b2_staticBody, ColliderType::_100PTS);
	App->physics->CreateCircle(405, 244, 14, b2BodyType::b2_staticBody, ColliderType::_100PTS);

	//200 points Collider
	App->physics->CreateCircle(352, 217, 14, b2BodyType::b2_staticBody, ColliderType::_200PTS);

	//x3 multiplier Collider
	App->physics->CreateCircle(144, 150, 14, b2BodyType::b2_staticBody, ColliderType::x3);

	//x2 multiplier Collider
	App->physics->CreateCircle(560, 150, 14, b2BodyType::b2_staticBody, ColliderType::x2);

	//Bumpers Colliders
	App->physics->CreateCircle(197, 332, 14, b2BodyType::b2_staticBody, ColliderType::BUMPER);
	App->physics->CreateCircle(507, 332, 14, b2BodyType::b2_staticBody, ColliderType::BUMPER);
	App->physics->CreateCircle(241, 471, 14, b2BodyType::b2_staticBody, ColliderType::BUMPER);
	App->physics->CreateCircle(464, 471, 14, b2BodyType::b2_staticBody, ColliderType::BUMPER);
	App->physics->CreateCircle(352, 527, 14, b2BodyType::b2_staticBody, ColliderType::BUMPER);

	/*Green Bumpers Colliders Left */
	App->physics->CreateRectangle(502, 178, 32, 13, b2BodyType::b2_staticBody, ColliderType::BUMPER, 45);
	App->physics->CreateRectangle(530, 206, 32, 13, b2BodyType::b2_staticBody, ColliderType::BUMPER, 45);
	App->physics->CreateRectangle(558, 234, 32, 13, b2BodyType::b2_staticBody, ColliderType::BUMPER, 45);

	/*Green Bumpers Colliders Middle */
	App->physics->CreateRectangle(304, 128, 32, 13, b2BodyType::b2_staticBody, ColliderType::BUMPER);
	App->physics->CreateRectangle(352, 128, 32, 13, b2BodyType::b2_staticBody, ColliderType::BUMPER);
	App->physics->CreateRectangle(400, 128, 32, 13, b2BodyType::b2_staticBody, ColliderType::BUMPER);

	/*Green Bumpers Colliders Right */
	App->physics->CreateRectangle(202, 178, 32, 13, b2BodyType::b2_staticBody, ColliderType::BUMPER, -45);
	App->physics->CreateRectangle(174, 206, 32, 13, b2BodyType::b2_staticBody, ColliderType::BUMPER, -45);
	App->physics->CreateRectangle(146, 234, 32, 13, b2BodyType::b2_staticBody, ColliderType::BUMPER, -45);


	//Center map wall
	int points3[12] = { 351, 264,
					  288, 327,
					  288, 366,
					  351, 429,
					  414, 366,
					  414, 327 };
	wall = App->physics->CreateChain(0, 0, points3, 12, b2BodyType::b2_staticBody, ColliderType::WALL);
	
	//Down-Left curved wall
	int points4[46] = { 112, 296,
					 112, 345,
					 114, 353,
					 117, 363,
					 120, 372,
					 124, 378,
					 130, 384,
					 138, 391,
					 144, 397,
					 155, 402,
					 166, 405,
					 173, 407,
					 190, 408,
					 190, 393,
					 174, 393,
					 165, 390,
					 157, 386,
					 148, 380,
					 140, 372,
					 132, 361,
					 128, 351,
					 126, 342,
					 126, 296 };
	wall = App->physics->CreateChain(0, 0, points4, 46, b2BodyType::b2_staticBody, ColliderType::WALL);

	//Down-Right curved wall
	int points5[44] = { 577, 297,
					  577, 346,
					  574, 353,
					  571, 361,
					  564, 371,
					  559, 377,
					  553, 382,
					  544, 388,
					  535, 392,
					  526, 395,
					  514, 395,
					  514, 408,
					  530, 408,
					  541, 405,
					  554, 400,
					  565, 393,
					  574, 385,
					  581, 376,
					  586, 366,
					  589, 356,
					  592, 346,
					  592, 297 };
	wall = App->physics->CreateChain(0, 0, points5, 44, b2BodyType::b2_staticBody, ColliderType::WALL);

	//Up-Right curved wall
	int points6[46] = { 311, 177,
					  295, 177,
					  285, 179,
					  274, 183,
					  265, 187,
					  260, 192,
					  253, 198,
					  247, 205,
					  241, 216,
					  238, 225,
					  234, 238,
					  234, 287,
					  248, 287,
					  248, 240,
					  250, 232,
					  254, 223,
					  260, 215,
					  265, 209,
					  271, 203,
					  282, 197,
					  290, 193,
					  298, 190,
					  311, 190 };
	wall = App->physics->CreateChain(0, 0, points6, 46, b2BodyType::b2_staticBody, ColliderType::WALL);

	//Up-Right curved wall
	int points7[44] = { 393, 177,
					  393, 190,
					  407, 191,
					  415, 193,
					  422, 196,
					  430, 201,
					  436, 207,
					  444, 214,
					  450, 224,
					  454, 232,
					  456, 243,
					  456, 286,
					  470, 286,
					  470, 238,
					  467, 225,
					  463, 215,
					  458, 206,
					  451, 198,
					  442, 189,
					  429, 183,
					  420, 180,
					  409, 177 };
	wall = App->physics->CreateChain(0, 0, points7, 44, b2BodyType::b2_staticBody, ColliderType::WALL);

	//Upper left Bouncer
	int points8[8] = { 64, 468,
						143, 495,
						142, 512,
						64, 512};
	wall = App->physics->CreateChain(0, 0, points8, 8, b2BodyType::b2_staticBody, ColliderType::LEFTUP_SLINGSHOT);
	
	//Lower left bouncer
	int points9[8] = { 145, 504,
						203, 603,
						203, 623,
						145, 602 };
	wall = App->physics->CreateChain(0, 0, points9, 8, b2BodyType::b2_staticBody, ColliderType::LEFTDOWN_SLINGSHOT);
	
	//Upper right bouncer
	int points10[8] = { 640, 511,
						561, 513,
						561, 495,
						640, 472};
	wall = App->physics->CreateChain(0, 0, points10, 8, b2BodyType::b2_staticBody, ColliderType::RIGHTUP_SLINGSHOT);
	
	//Lower right bouncer
	int points11[10] = { 560, 505,
						560, 602,
						501, 623,
						501, 603,
						558, 501};
	wall = App->physics->CreateChain(0, 0, points11, 10, b2BodyType::b2_staticBody, ColliderType::RIGHTDOWN_SLINGSHOT);

	
	
	return ret;
}

bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	App->textures->Unload(texture);
	App->textures->Unload(scene);
	App->textures->Unload(leftFlipperTex);
	App->textures->Unload(rightFlipperTex);

	return true;
}

update_status ModuleSceneIntro::Update()
{
	
	//Draw Scene
	App->renderer->Blit(scene, 0, 0, NULL);

	currentAnim = &flipperUp;

	// If user presses SPACE, enable RayCast
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// Enable raycast mode
		ray_on = !ray_on;

		// Origin point of the raycast is be the mouse current position now (will not change)
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}


	// Prepare for raycast ------------------------------------------------------
	// The target point of the raycast is the mouse current position (will change over game time)
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// Total distance of the raycast reference segment
	int ray_hit = ray.DistanceTo(mouse);

	// Declare a vector. We will draw the normal to the hit surface (if we hit something)
	fVector normal(0.0f, 0.0f);

	//Ball Shooter mechanic
	Shot();

	//Spring animation
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		currentAnim = &flipperCompressing;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		flipperCompressing.Reset();
		App->audio->PlayFx(kicker_sfx);
	}

	//Check if MULTIPLIERS are ACTIVATED
	//X2 Multiplier
	if (App->player->multiplierx2IsActive == true) {
		if (App->player->multiplierTime > 1200)  //Time multiplier is active is 10 seconds
		{
			LOG("Deactivating x2 multiplier");
			App->player->multiplierx2IsActive = false;
			App->player->multiplierTime = 0;
			App->audio->PlayFx(multiplierDeactivation_sfx);
		}

		App->player->multiplierTime++;
	}
	//X3 Multiplier
	if (App->player->multiplierx3IsActive == true) {
		if (App->player->multiplierTime > 600) //Time multiplier is active is 5 seconds
		{
			LOG("Deactivating x3 multiplier");
			App->player->multiplierx3IsActive = false;
			App->player->multiplierTime = 0;
			App->audio->PlayFx(multiplierDeactivation_sfx);
		}
		App->player->multiplierTime++;
	}

	//Blit UI
	App->ui->BlitScore();
	App->ui->BlitHighScore();
	App->ui->BlitLives();
	App->ui->BlitPrevScore();
	
	if (App->physics->debug)
	{
		App->ui->BlitChangeFPS();
		App->ui->BlitChangeGravity();
		App->ui->BlitChangeRestitutionCoeff();
		App->ui->BlitPlayerXPos();
		App->ui->BlitPlayerYPos();

		//Changes FPS
		if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
			App->fps++;
		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
			App->fps--;

		//Changes Gravity
		if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN) {
			App->physics->gravity.y += 0.1;
			App->player->pbody->body->SetLinearVelocity(App->physics->gravity);
		}
		if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
			App->physics->gravity.y -= 0.1;
			App->player->pbody->body->SetLinearVelocity(App->physics->gravity);
		}
		//Changes Gravity
		if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
			App->physics->restitutionCoeff += 0.1;
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
			App->physics->restitutionCoeff -= 0.1;
	}

	//Activate GodMode
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) {
		App->player->godMode = !App->player->godMode;
		App->audio->PlayFx(buttonClick_sfx);
	}

	//Blit flippers
	if (leftFlipper != NULL)
	{
		int x, y;
		leftFlipper->GetPosition(x, y);
		App->renderer->Blit(leftFlipperTex, x, y, NULL, 0.2f, leftFlipper->GetRotation());
	}
	if (rightFlipper != NULL)
	{
		int x, y;
		rightFlipper->GetPosition(x, y);
		App->renderer->Blit(rightFlipperTex, x, y, NULL, 0.2f, rightFlipper->GetRotation());
	}

	//Blit Kicker anims
	SDL_Rect rect = currentAnim->GetCurrentFrame();
	App->renderer->Blit(texture, 677, 680, &rect);
	currentAnim->Update();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	
}

void ModuleSceneIntro::Shot() {

	//Shooter prepares Ball
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		if (distance < 55)
		{
			distance++;
			LOG("DISTANCE: %f", distance);
			shotVel.y = PIXEL_TO_METERS(distance);
			App->scene_intro->shooter->body->SetLinearVelocity(shotVel);
		}
		else {
			App->scene_intro->shooter->body->SetLinearVelocity({ 0,0 });
		}

	}

	//Shoot Ball
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {

		shotVel.y = PIXEL_TO_METERS(distance) * SPRING_K;
		App->scene_intro->shooter->body->SetLinearVelocity(shotVel);

		LOG("IMPULSE AFTER SHOT %f", shotVel.y);

		stopShot = true;
	}

	//Stops
	if (stopShot == true) {

		LOG("POSITION: %f, %f", shooter->body->GetWorldCenter().x, shooter->body->GetWorldCenter().y);
		
		if (shooter->body->GetWorldCenter().y < shooterInitPos.y) {
			App->scene_intro->shooter->body->SetLinearVelocity({ 0,0 });
			App->scene_intro->shooter->body->SetTransform(shooterInitPos, 0.0f);
			stopShot = false;
			distance = 0;
		}
	}
}