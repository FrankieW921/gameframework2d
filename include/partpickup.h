#ifndef __PARTPICKUP_H__
#define __PARTPICKUP_H__

#include "entity.h"

typedef struct {
	Uint8 partPickupType;
	const char* partName[128];
}PartPickupData;

Entity* partpickup_new(GFC_Vector2D position, Uint8 partPickupType, const char* partName);

void partpickup_free(Entity* self);

#endif