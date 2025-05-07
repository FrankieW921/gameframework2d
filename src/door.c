#include "simple_logger.h"
#include "door.h"

Entity* door_new(const char* mapName, GFC_Vector2D position, int playerSpawnIndex) {
	slog("Creating door");
	Entity* self;
	DoorData* data;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn door entity");
		return NULL;
	}

	gfc_vector2d_copy(self->position, position);
	self->bounds = gfc_rect(position.x, position.y, 8, 256);
	self->type = Door;
	self->sprite = gf2d_sprite_load_image("images/door.png");

	data = gfc_allocate_array(sizeof(DoorData), 1);
	if (data) {
		strcpy(data->mapName, mapName);
		//data->mapName = mapName;
		data->playerSpawnIndex = playerSpawnIndex;
	}
	self->data = data;

	slog("Door: %s, %f, %f, %i", data->mapName, self->position.x, self->position.y, data->playerSpawnIndex);
	return self;
}

const char* get_door_name(Entity* door) {
	DoorData* data;
	if (!door)return;
	data = door->data;
	if (data) {
		slog("Returning map name from door: %s", data->mapName);
		return data->mapName;
	}
}

void door_free(Entity* door) {
	DoorData* data;
	if (!door) {
		return;
	}
	data = door->data;
	memset(data, 0, sizeof(DoorData));
	entity_free(door);
}
