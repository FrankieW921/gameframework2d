#include "simple_logger.h"

#include "player.h"

Entity *player_new_entity(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn player entity");
		return NULL;
	}
	self->think = player_think;
	gfc_vector2d_copy(self->position, position);
	self->sprite = gf2d_sprite_load_all(
		"images/space_bug.png",
		128,
		128,
		0,
		0
	);
	return self;
}

void player_think(Entity* self) {
	if (!self)return;

}


/*eol@eof*/