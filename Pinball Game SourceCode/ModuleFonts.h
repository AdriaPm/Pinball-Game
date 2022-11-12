#ifndef __MODULEFONTS_H__
#define __MODULEFONTS_H__

#include "Module.h"
#include "Globals.h"
#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256

struct SDL_Texture;

struct Font
{
	char table[MAX_FONT_CHARS];
	SDL_Texture* graphic = nullptr;
	uint len, char_w, char_h, row_chars;
};

class ModuleFonts : public Module
{
public:

	ModuleFonts(Application* app, bool start_enabled = true);
	~ModuleFonts();

	bool Start();

	// Load Font
	int Load(const char* texture_path, const char* characters, int id);
	void UnLoad(int font_id);

	// Create a surface from text
	void BlitText(int x, int y, int bmp_font_id, const char* text) const;


private:
	Font fonts[MAX_FONTS];
};


#endif // __MODULEFONTS_H__