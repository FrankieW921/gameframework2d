#ifndef __BOSS_ENEMY_H__
#define __BOSS_ENEMY_H__

#include "enemy.h"

typedef struct {
	Uint8 subEnemyCount;
	GFC_List* enemies;
}BossData;


Entity* boss_new_entity(GFC_Vector2D position);

void boss_think(Entity* self);

void boss_update(Entity* self);

#endif