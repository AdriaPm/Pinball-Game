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
	void ModulePlayer::ResetPosition();

	void Bonus();

	int GetScore() { return score; }
	int GetHighScore() { return highscore; }
	int GetLives() { return lives; }
	int GetPrevScore() { return previousScore; }

public:

	Animation* currentAnim;
	Animation idleAnim;
	Animation rollingAnim;

	iPoint position;

	PhysBody* pbody;

	b2Vec2 bounceImpulse;

	uint flipper_sfx;
	uint FlipperPush_sfx;
	uint bonus_sfx;
	uint bumper_sfx;
	uint extraLife_sfx;
	uint multiplierActivation_sfx;
	uint resetBall_sfx;

	bool alive;
	uint lives = 3;

	int score = 0;
	int highscore = 0;
	int previousScore = 0;

	bool bonusIsActive = false;

	bool multiplierx2IsActive = false;
	bool multiplierx3IsActive = false;

	int multiplierTime = 0;
private:

	bool godMode;
	
	bool deadBall = false;
	int deadTime = 0;

	SDL_Texture* texture;
	const char* texturePath;

	b2Vec2 startPos;
	b2Vec2 velocity;

	int bounces = 0;
};