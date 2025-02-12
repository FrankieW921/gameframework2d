#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

/**
* @brief spawn a new enemy 
* @param position where to spawn
* @return NULL on error, pointer to the enemy entity
*/
Entity* enemy_new_entity(GFC_Vector2D position);

/*
* @brief enemy entity think
* @param enemy entity
*/
void enemy_think(Entity* self);

#endif