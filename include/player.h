#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

/**
* @brief spawn a new player entity
* @param position where to spawn
* @return NULL on error, pointer to the spawn player entity
*/
Entity *player_new_entity(GFC_Vector2D position); 

#endif