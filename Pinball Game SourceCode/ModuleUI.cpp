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

void ModuleUI::BlitPrevScore()
{
	char prevScoreText[10];
	sprintf_s(prevScoreText, 10, "%d", App->player->GetPrevScore());
	App->font->BlitText(92, 730, font_id, prevScoreText);
}

void ModuleUI::BlitFinalScore()
{
	char finalScoreText[10];
	sprintf_s(finalScoreText, 10, "%d", App->player->GetScore());
	App->font->BlitText(423, 332, font_id, finalScoreText);
}

void ModuleUI::BlitFinalHighScore()
{
	char finalHighScoreText[10];
	sprintf_s(finalHighScoreText, 10, "%d", App->player->GetHighScore());
	App->font->BlitText(423, 402, font_id, finalHighScoreText);
}

void ModuleUI::BlitFinalPrevScore()
{
	char finalPrevScoreText[10];
	sprintf_s(finalPrevScoreText, 10, "%d", App->player->GetPrevScore());
	App->font->BlitText(423, 493, font_id, finalPrevScoreText);
}

void ModuleUI::BlitChangeFPS() {
	char changeFPS[4];
	sprintf_s(changeFPS, 4, "%d", App->fps);
	App->font->BlitText(423, 493, font_id, changeFPS);
}

void ModuleUI::BlitChangeGravity() {
	char changeGravity[4];
	sprintf_s(changeGravity, 4, "%d", App->physics->gravity.y);
	App->font->BlitText(423, 493, font_id, changeGravity);
}