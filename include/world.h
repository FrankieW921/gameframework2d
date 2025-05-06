#ifndef __WORLD_H__
#define __WORLD_H__

#include "simple_json.h"

#include "gfc_text.h"
#include "gfc_vector.h"
#include "gfc_list.h"
#include "gfc_shape.h"

#include "gf2d_sprite.h"

typedef struct World_S
{
	GFC_TextLine	name; //name of world instance
	Sprite			*background; //background image of the world
	Sprite			*tileSet; //tile image from which tile textures are taken 
	Uint8			*tileMap; //the numerical representation of the world's tile layout
	GFC_Vector2I	tileMapSize; //tile width and height

	Sprite			*tileLayer;	//prerendered tile image

	GFC_List		entityList; //list of entity's within the world
	GFC_List		interactableList;
	GFC_List		doorList;
	GFC_List		spawnLocations;


	
}World;

/*
* @brief return the current static/global world
*/
World* get_current_world();

/*
* @brief set the static/global world to a new world
* @param newWorld the world to set as the new world, use the world_load() function
*/
void set_current_world(World* newWorld);

/*
* @brief return the current static/global spawn index
*/
int get_spawn_index();

/*
* @brief set the static/global spawn index to a spawn index specified by a door object
* @param i the new int to set spawn index to
*/
void set_spawn_index(int i);

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
World* world_load(const char* filename, int spawnIndex);

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
*/
void world_setup_camera();

/**
* @brief collision for the world with an entity's bounds
* @param world the world to bound check
* @param entity_bounds the rect bounds of an entity
* @return an int 1 for collision, 
*/
int world_collide(World* world, GFC_Shape entity_bounds);
#endif

