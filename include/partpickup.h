#ifndef __PARTPICKUP_H__
#define __PARTPICKUP_H__

#include "entity.h"

typedef struct {
	PartPickupType partPickupType;
	const char* partName[128];
	int partDefIndex;
}PartPickupData;

Entity* partpickup_new(GFC_Vector2D position, PartPickupType partPickupType, const char* partName, int partDefIndex);

void partpickup_free(Entity* self);

#endif