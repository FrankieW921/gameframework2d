#ifndef __WORLD_H__
#define __WORLD_H__

#include "simple_json.h"

#include "gfc_text.h"
#include "gfc_vector.h"
#include "gfc_list.h"


#include "gf2d_sprite.h"

typedef struct
{
	GFC_TextLine	name; //name of world instance
	Sprite			*background; //background image of the world
	GFC_Vector2D	worldSize; //width and height IN TILES, 1200 x 720 resolution is 75 x 45 for 16x16 tiles
	Sprite			*tileSet; //tile image from which tile textures are taken 
	Uint8			*tileMap; //the numerical representation of the world's tile layout
	GFC_Vector2D	tileMapSize; //tile width and height

	Sprite			*tileLayer;	//prerendered tile image

	GFC_List		*entityList; //list of entity's within the world

	
}World;

/*
* @brief test function, builds a hardcoded world
* @return NULL on failure, test world otherwise
*/
World* world_test_new();

/*
* @brief allocate a new empty world
* @return NULL on error, blank world otherwise
*/
World* world_new();

/*
* @brief free an allocated world
* @param world the world to free
*/
void world_free(World* world);

/**
* @brief draw a world
* @param world the world to draw
*/
void world_draw(World* world);
#endif

