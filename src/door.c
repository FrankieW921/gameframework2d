#include "door.h"

Entity* door_new(const char* mapName, GFC_Vector2D position, int playerSpawnIndex) {
	Entity* self;
	DoorData* data;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn door entity");
		return NULL;
	}

	data = gfc_allocate_array(sizeof(DoorData), 1);
	if (data) {
		data->mapName = mapName;
		data->playerSpawnIndex = playerSpawnIndex;
	}
	self->data = data;

	gfc_vector2d_copy(self->position, position);
	self->bounds = gfc_rect(position.x, position.y, 8, 256);
	self->type = Door;
	
	return self;
}
