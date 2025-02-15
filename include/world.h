#ifndef __WORLD_H__
#define __WORLD_H__

#include "gfc_text.h"
#include "gfc_vector.h"
#include "gfc_list.h"

#include "gf2d_sprite.h"

typedef struct
{
	GFC_TextLine	name;
	Sprite			*background;
	GFC_Vector2D	worldSize;
	Sprite			*tileSet;
	Uint8			*tileMap;
	GFC_Vector2D	tileMapSize;
	GFC_List		*entityList;
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

