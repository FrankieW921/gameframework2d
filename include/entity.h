#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_text.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

typedef struct Entity_S
{
	Uint8			_inuse; /**mem management flag*/
	GFC_TextLine	name; //name of entity for debugging
	Sprite			*sprite; //sprite for entity
	float			frame; //aid in drawing
	GFC_Vector2D	position; //where to draw
	GFC_Vector2D	velocity; //how to move
	GFC_Vector2D	acceleration;
	void			(*think)(struct Entity_S* self); /*pointer to think function*/
}Entity;

/**
 * @brief initialize the entity sub entity_system_init
 * @param maxEnts upper limit for entities can exist at once
 */
void entity_system_init(Uint32 maxEnts);

/**
 * @brief free all entities in the manager
 */
void entity_system_free_all();

/**
* @brief draw all entities with a sprite
*/
void entity_system_draw_all();

/**
* @brief call the think function on all entities that have a pointer to one
*/
void entity_system_think_all();

/*
* @brief called every frame for entities that have a think function
*/
void entity_think(Entity *self);

/**
 * @brief get a new empty entity to use
 * @return NULL if out of entities, or a blank entity otherwise
 */
Entity* entity_new();

/**
 * @brief empty an entity for use
 */
void entity_free(Entity*);

/*
* @brief draw the entity frame
*/
void entity_draw(Entity*);

#endif