#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleUI.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"

#include "SDL/include/SDL.h"


ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled) {

}

ModuleUI::~ModuleUI()
{}

bool ModuleUI::Start()
{
	LOG("Loading UI textures");
	bool ret = true;

	font_id = App->font->Load("Assets/Textures/font.png", "0123456789", 1);


	return ret;
}

bool ModuleUI::CleanUp()
{
	LOG("Unloading UI graphics");

	App->font->UnLoad(font_id);

	return true;
}

update_status ModuleUI::Update()
{
	
	if (App->player->alive == true)
	{
		BlitScore();
		BlitHighScore();
		BlitLives();
	}



	return UPDATE_CONTINUE;
}


void ModuleUI::BlitScore()
{
	char scoreText[10];

	_itoa_s(App->player->GetHighScore(), scoreText, 10);

	App->font->BlitText(10, 10, font_id, scoreText);
}

void ModuleUI::BlitHighScore()
{
	char highScoreText[10];

	_itoa_s(App->player->GetHighScore(), highScoreText, 10);

	App->font->BlitText(100, 10, font_id, highScoreText);
}

void ModuleUI::BlitLives()
{
	char livesText[10];

	_itoa_s(App->player->GetLives(), livesText, 10);

	App->font->BlitText(200, 10, font_id, livesText);

}
