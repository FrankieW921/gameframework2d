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
	Sprite			*tileSet; //tile image from which tile textures are taken 
	Uint8			*tileMap; //the numerical representation of the world's tile layout
	GFC_Vector2I	tileMapSize; //tile width and height

	Sprite			*tileLayer;	//prerendered tile image

	GFC_List		*entityList; //list of entity's within the world

	
}World;

/*
* @brief test function, builds a hardcoded world
* @return NULL on failure, test world otherwise
*/
World* world_test_new();


/*
* @brief prebuilds the tileLayer sprite to render
* @param world the world to prerender the tileLayer for
*/
void world_tile_layer_build(World* world);

/*
* @brief loads a world from a given JSON file
* @param filename the JSON file's name to load from
* @returns NULL on error, or a usable world
*/
World* world_load(const char* filename);

/*
* @brief allocate a new empty world
* @param worldSize world dimensions in tiles (i.e. 80 x 45)
* @return NULL on error, blank world otherwise
*/
World* world_new(GFC_Vector2I worldSize);

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

/*
* @brief set bounds for global camera based on the world and its size
* @param world the world to base camera bounds on
*/
void world_setup_camera(World* world);
#endif

