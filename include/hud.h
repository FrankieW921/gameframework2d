#ifndef __HUD_H
#define __HUD_H

#include <SDL_ttf.h>

#include "gfc_types.h"
#include "gfc_shape.h"
#include "gfc_vector.h""
#include "gf2d_draw.h"
#include "gf2d_graphics.h"

#include "player.h"

typedef struct Hud_s {
	Uint8 playerHealth;
	GFC_Rect hudBox;
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* texture;

}Hud;

/*
* @brief creates new HUD object to draw to the screen
* @param player the player to draw the hud for
* @return the hud to be drawing
*/
Hud* new_hud(Entity* player);


/*
* @brief draws a hud
* @para h the hud to draw
*/
void draw_hud(Hud* h);

#endif