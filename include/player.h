#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

/**
* @brief spawn a new player entity
* @param position where to spawn
* @return NULL on error, pointer to the spawn player entity
*/
Entity *player_new_entity(GFC_Vector2D position); 

/*
* @brief player entity think
* @param self the player entity entity
*/
void player_think(Entity* self);

/*
* @brief player entity update
* @param self the player entity
*/
void player_update(Entity* self);

/*
* @brief incomplete shoot function, shoots a projectile from the player in the direction of the player's mouse
*/
void player_shoot(GFC_Vector2D position, GFC_Vector2D velocity);

#endif