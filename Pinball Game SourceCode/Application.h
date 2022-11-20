#pragma once

#include "p2List.h"
#include "Globals.h"

class Module;
class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModulePlayer;
class FadeToBlack;
class TitleScreen;
class ModuleSceneIntro;
class ModuleFonts;
class ModuleUI;
class EndingScreen;
class ModulePhysics;



class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePlayer* player;
	FadeToBlack* fade;
	TitleScreen* title;
	ModuleSceneIntro* scene_intro;
	ModuleFonts* font;
	ModuleUI* ui;
	EndingScreen* ending_screen;
	ModulePhysics* physics;
	

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	int fps = FPS;

private:

	void AddModule(Module* mod);
};