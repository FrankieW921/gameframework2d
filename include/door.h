#ifndef __DOOR_H__
#define __DOOR_H__

#include "entity.h"

typedef struct Door_S {
	int playerSpawnIndex;
	const char* mapName[128];
}DoorData;

Entity* door_new(const char* mapName, GFC_Vector2D doorPosition, int playerSpawnIndex);

void door_free(Entity* door);

const char* get_door_name(Entity* door);

#endif
