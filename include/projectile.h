#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "entity.h"

typedef struct {
	Uint8			timeToLive; //time to live for projectiles specifically
	SJson*			projectileDefs; //def file to read projectiles from
}ProjectileData;

/**
* @brief spawn a new projectile entity
* @param position where to spawn
* @param velocity direction in which to shoot the projectile,
* @return NULL on error, pointer to the spawn projectile entity
*/
Entity* projectile_new_entity(GFC_Vector2D position, GFC_Vector2D velocity);

/*
* @brief projectile entity think
* @param projectile entity
*/
void projectile_think(Entity* self);

/*
* @brief projectile entity update
* @param projectile entity
*/
void projectile_update(Entity* self);


#endif