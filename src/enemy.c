#include "simple_logger.h"

#include "enemy.h"

Entity* enemy_new_entity(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn enemy");
		return NULL;
	}
	self->think = enemy_think;
	gfc_vector2d_copy(self->position, position);
	self->sprite = gf2d_sprite_load_image("images/smtDemonIcon.png");
	return self;
}

void enemy_think(Entity* self) {
	if (!self)return;
	self->velocity.x += 0;
	self->velocity.y += 0;

	int r = rand() % 2;
	self->velocity.x += r;
	r = rand() % 2;
	self->velocity.y += r;

	self->position.x += self->velocity.x * .05;
	self->position.y += self->velocity.y * .05;

}

/*eol@eof*/