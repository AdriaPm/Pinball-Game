#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "Module.h"
#include "ModuleFonts.h"
#include "Globals.h"
#include "SDL_mixer/include/SDL_mixer.h"


#define SECOND 1000

struct SDL_Texture;

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status Update();
	bool CleanUp();

	void BlitScore();
	void BlitHighScore();
	void BlitLives();
	void BlitPrevScore();

	void BlitFinalScore();
	void BlitFinalHighScore();
	void BlitFinalPrevScore();

	void BlitChangeFPS();
	void BlitChangeGravity();
	void BlitChangeRestitutionCoeff();

	uint font_id = -1;
	uint font2_id = -1;

private:

};

#endif // __MODULEUI_H__