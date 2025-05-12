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
	const char* text[1024];
	GFC_Rect hudBox;
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* texture;
}Hud;


/*
* @brief inits the static huds
*/
void init_huds();

/*
* @brief calls each static huds proper draw
*/
void draw_all_huds();

/*
* @brief creates new HUD object to draw to the screen
* @param player the player to draw the hud for
* @return the hud to be drawing
*/
Hud* new_hud(GFC_Vector2D position);

/*
* @brief switches the draw parts huds variable to 1
*/
void enable_do_draw_parts_huds();

/*
* @brief switches the draw parts huds variable to 0
*/
void disable_do_draw_parts_huds();

/*
* @brief draws the hud as the health hud, 
* @param h the hud to draw
*/
void draw_health_hud(Hud* h);

/*
* @brief draws a parts hud and makes its text display the parts the player has equipped
* @param ph the parts hud to draw
*/
void draw_current_parts_hud(Hud* ph);

/*
* @brief draws the inventory hud, all the names of the parts the player currently holds
* @param h the hud to draw
*/
void draw_inventory_hud(Hud* h);


#endif