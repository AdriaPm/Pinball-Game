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

	//Reset player's parameters
	App->player->previousScore = App->player->score;
	App->player->lives = 3;
	App->player->ResetPosition();

	//Disable modules/entities
	App->player->Disable();
	App->physics->Disable();

	// Set camera position
	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Load music
	App->audio->PlayMusic("Assets/Audio/Music/EndingScreen.ogg");

	//Load SFX
	startSFX = App->audio->LoadFx("Assets/Audio/Fx/start_game.wav");

	//Load tex
	img = App->textures->Load("Assets/Textures/EndingScreen.png");
	
	return true;
}

// Called each loop iteration
update_status EndingScreen::Update()
{
	//Pass next screen
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		LOG("PASA A GAME SCENE");
		App->fade->FadeBlack(App->ending_screen, (Module*)App->title, 90);
		App->audio->PlayFx(startSFX);
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		return UPDATE_STOP;
	}

	App->renderer->Blit(img, 0, 0, NULL);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool EndingScreen::CleanUp()
{
	LOG("Freeing ENDING SCENE");
	
	App->textures->Unload(img);
	
	return true;
}
