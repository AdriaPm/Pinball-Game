#include "EndingScreen.h"

#include "Module.h"
#include "Globals.h"

#include "Application.h"
#include "Animation.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"
#include "FadeToBlack.h"
#include "ModuleUI.h"
#include "ModuleFonts.h"

#include "SDL/include/SDL_render.h"


EndingScreen::EndingScreen(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
EndingScreen::~EndingScreen()
{}

// Called before the first frame
bool EndingScreen::Start()
{
	LOG("Loading EndingScreen");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	LOG("--STARTS ENDING SCENE--");
	
	img = App->textures->Load("Assets/Textures/EndingScreen.png");
	startSFX = App->audio->LoadFx("Assets/Audio/Fx/start_game.wav");
	
	App->player->alive = false;
	App->player->ResetPosition();

	
	return true;
}

// Called each loop iteration
update_status EndingScreen::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		LOG("PASA A GAME SCENE");
		App->fade->FadeBlack(App->ending_screen, (Module*)App->scene_intro, 90);
		App->audio->PlayFx(startSFX);
	}

	App->renderer->Blit(img, 0, 0, NULL);

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool EndingScreen::CleanUp()
{
	LOG("Freeing ENDING SCENE");
	
	App->textures->Unload(img);
	
	return true;
}
