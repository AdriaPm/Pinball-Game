#include "TitleScreen.h"

#include "Application.h"
#include "Animation.h"
#include "ModuleWindow.h"
#include "Module.h"
#include "Globals.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleTextures.h"
#include "ModuleUI.h"
#include "FadeToBlack.h"
#include "ModulePlayer.h"


#include "SDL/include/SDL_render.h"

TitleScreen::TitleScreen(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

// Destructor
TitleScreen::~TitleScreen()
{}

// Called before the first frame
bool TitleScreen::Start()
{
	LOG("Loading TitleScreen");
	bool ret = true;

	//Disable modules/entities
	App->player->Disable();

	// Set camera position
	App->renderer->camera.x = App->renderer->camera.y = 0;

	//Load music
	App->audio->PlayMusic("Assets/Audio/Music/TitleScreen.ogg");
	
	//Load SFX
	selectSFX = App->audio->LoadFx("Assets/Audio/Fx/CoinInsert.wav");

	//Load tex
	img = App->textures->Load("Assets/Textures/TitleScreen.png");


	return ret;
}


// Called each loop iteration
update_status TitleScreen::Update()
{
	//Pass next screen
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		LOG("PASA A GAME SCENE");
		App->fade->FadeBlack(App->title, (Module*)App->scene_intro, 90);
		App->audio->PlayFx(selectSFX);
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		return UPDATE_STOP;
	}

	App->renderer->Blit(img, 0, 0, NULL);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool TitleScreen::CleanUp()
{
	LOG("Unloading Title scene");

	App->textures->Unload(img);
	return true;
}
