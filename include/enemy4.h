#ifndef __ENEMY4_H__
#define __ENEMY4_H__

#include "entity.h"

/**
* @brief spawn a new enemy 
* @param position where to spawn
* @return NULL on error, pointer to the enemy entity
*/
Entity* enemy4_new_entity(GFC_Vector2D position);

/*
* @brief enemy entity think
* @param enemy entity
*/
void enemy4_think(Entity* self);

/*
* @brief enemy entity update
* @param enemy entity
*/

void enemy4_update(Entity* self);

#endif