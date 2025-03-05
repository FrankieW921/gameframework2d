#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_text.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include "gfc_shape.h"

typedef enum {
	ET_None,
	ET_Player,
	ET_Enemy,
	ET_PlayerProjectile,
	ET_EnemyProjectile,
}EntityType;

typedef enum {
	Small_Bullet,
	Energy_Ball,
	Small_Rocket
}ProjectileType;

typedef struct Entity_S
{
	Uint8			_inuse; /**mem management flag*/
	GFC_TextLine	name; //name of entity for debugging
	EntityType		type; //type of entity (player, pickup, enemy, projectile)
	Sprite			*sprite; //sprite for entity
	float			frame; //aid in drawing
	GFC_Vector2D	position; //where to draw
	GFC_Vector2D	velocity; //how to move
	GFC_Vector2D	acceleration;
	void			(*think)(struct Entity_S* self); /*pointer to think function*/
	void			(*update)(struct Entity_S* self); /*pointer to update function*/
	GFC_Rect		bounds; //collision bounds of the entity, calculate via image or something
	void			*data;
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

/*
* @brief called every frame for entities that have a update function
*/
void entity_update(Entity* self);

/**
* @brief call the update function on all entities that have a pointer to one
*/
void entity_system_update_all();

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

/*
* @brief check if entities are colliding
* @param self the entity colliding
* @param other the entity get acted upon
* @return 0 if no collision, 1 if collision
*/
Uint8 entity_collision_check(Entity* self, Entity* other);

/*
* @brief makes a list of all the collisions currently acting on an entity
* @param self the entity to check collisions for
* @return NULL if no collisions, list of collisions otherwise
* @note must free list afterwards
*/
GFC_List* entity_collide_all(Entity* self);


#endif