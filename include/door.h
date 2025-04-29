#ifndef __DOOR_H__
#define __DOOR_H__

#include "entity.h"

typedef struct Door_S {
	const char* mapName;
	int playerSpawnIndex;
}DoorData;

Entity* door_new(const char* mapName, GFC_Vector2D doorPosition, int playerSpawnIndex);

#endif
