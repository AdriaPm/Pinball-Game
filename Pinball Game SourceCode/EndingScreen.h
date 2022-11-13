#ifndef __ENDINGSCREEN_H__
#define __ENDINGCREEN_H__

#include "Module.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class EndingScreen : public Module
{
public:

	EndingScreen(Application* app, bool start_enabled = false);

	// Destructor
	virtual ~EndingScreen();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	update_status Update();

	// Called before quitting
	bool CleanUp();

public:
	uint startSFX = 0;
	SDL_Texture* img;

private:
	

};


#endif // !__ENDINGSCREEN_H__