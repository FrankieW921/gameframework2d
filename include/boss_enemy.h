#ifndef __BOSS_ENEMY_H__
#define __BOSS_ENEMY_H__

#include "enemy.h"
#include "enemy2.h"

typedef struct {
	Entity* arm1;
	Entity* arm2;
}BossData;


Entity* boss_new_entity(GFC_Vector2D position);

void boss_think(Entity* self);

void boss_update(Entity* self);

#endif