#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "entity.h"
#include "player.h"

typedef struct {
	Uint8			projectileType;
	Uint8			speed;
	Uint8			damage;
	Uint16			timeToLive; //time to live for projectiles specifically
}ProjectileData;

/**
* @brief spawn a new projectile entity
* @param position where to spawn
* @param velocity direction in which to shoot the projectile
* @param currentArm the player's current arm type to base the spawned projectile off of
* @return NULL on error, pointer to the spawn projectile entity
*/
Entity* projectile_new_entity(GFC_Vector2D position, GFC_Vector2D velocity, Uint8 projectileType);

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