#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -10.0f
#define SPRING_K -35.0f
#define RESTITUTION_COEF 6.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum class ColliderType {
	BALL,
	WALL,
	BONUS,
	FLIPPER, // Ball hitter
	BUMPER, //Circle that makes bounce
	LEFTDOWN_SLINGSHOT, //Triangle that bounces
	LEFTUP_SLINGSHOT,
	RIGHTDOWN_SLINGSHOT, 
	RIGHTUP_SLINGSHOT,
	ROLLOVER, //Let pass or not the ball if a bool is active
	THROUGH,
	TRIGGER,
	_100PTS,
	_200PTS,
	x2,
	x3,
	UNKNOWN
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody();
	~PhysBody();

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	ColliderType cType;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	
	// Constructors & Destructors
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	// Main module steps
	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	// Create basic physics objects
	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType bType, ColliderType ctype);
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType bType, ColliderType ctype, float angle = 0.0f);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, b2BodyType bType, ColliderType ctype, float angle = 0.0f);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType bType, ColliderType ctype);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	// Box2D World
	b2World* world;

	b2Vec2 gravity;

	float restitutionCoeff;

	// Debug mode
	bool debug;

private:

	// Main ground
	b2Body* ground;

	// Mouse joint
	b2MouseJoint* mouse_joint;
	b2Body* mouse_body;

	b2Body* box1;
	b2Body* box2;
};