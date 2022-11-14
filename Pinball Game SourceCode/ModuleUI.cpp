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
	
	return UPDATE_CONTINUE;
}


void ModuleUI::BlitScore()
{
	char scoreText[10];
	sprintf_s(scoreText, 10, "%d", App->player->GetScore());
	App->font->BlitText(163, 39, font_id, scoreText);
}

void ModuleUI::BlitHighScore()
{
	char highScoreText[10];
	sprintf_s(highScoreText, 10, "%d", App->player->GetHighScore());
	App->font->BlitText(435, 39, font_id, highScoreText);
}

void ModuleUI::BlitLives()
{
	char livesText[10];
	sprintf_s(livesText, 10, "%d", App->player->GetLives());
	App->font->BlitText(655, 39, font_id, livesText);
}
