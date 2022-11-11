#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;
struct SDL_Texture;

class ModuleSceneIntro : public Module
{
public:

	// Constructors & Destructors
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	// Main module steps
	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:

	// Lists of physics objects
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	// Lower ground sensor (will kill all objects passig through it)
	PhysBody* lower_ground_sensor;

	//Shooter
	PhysBody* shooter;

	//Death Sensor
	PhysBody* through;

	PhysBody* leftFlipper;
	PhysBody* rightFlipper;

	PhysBody* leftFlipperJoint;
	PhysBody* rightFlipperJoint;

	PhysBody* wall;
	bool sensed;

	// Textures
	SDL_Texture* scene;
	SDL_Texture* leftFlipperTex;
	SDL_Texture* rightFlipperTex;
	

	// FX
	uint bonus_fx;

	// Raycast
	p2Point<int> ray;
	bool ray_on;

	// The pointer to the current ITEM animation --> Create more if there are more than 1 Animated Item
	// It will be switched depending on the ITEM movement action
	Animation* currentAnim;

	//Set of animations ITEM 1
	Animation flipperDown;
	Animation flipperUP;
	
	//Set of animations ITEM 2
	Animation idlePlayer;
	Animation runPlayer;
	Animation jumpPlayer;
	Animation diePlayer;

	float force;

};
