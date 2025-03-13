#ifndef __ENEMY3_H__
#define __ENEMY3_H__

#include "entity.h"

/**
* @brief spawn a new enemy 
* @param position where to spawn
* @return NULL on error, pointer to the enemy entity
*/
Entity* enemy3_new_entity(GFC_Vector2D position);

/*
* @brief enemy entity think
* @param enemy entity
*/
void enemy3_think(Entity* self);

/*
* @brief enemy entity update
* @param enemy entity
*/

void enemy3_update(Entity* self);

#endif