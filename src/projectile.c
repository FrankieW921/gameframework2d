#include "simple_logger.h"

#include "projectile.h"

Entity* projectile_new_entity(GFC_Vector2D position, GFC_Vector2D velocity) {
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn projectile entity");
		return NULL;
	}

	self->think = projectile_think;
	gfc_vector2d_copy(self->position, position);
	gfc_vector2d_copy(self->velocity, velocity); //ADJUST VELOCITY BASED ON PROJECTILE TYPE
	self->timeToLive = 120 * 16; //ADJUST TIME TO LIVE BASED ON PROJECTILE TYPE
	self->sprite = gf2d_sprite_load_all(
		"images/testprojectile.png",
		16,
		16,
		1,
		1
	);

	return self;
}

void projectile_think(Entity* self) {
	//ADJUST VELOCITY BASED ON PROJECTILE TYPE (projectile struct attribute?, switch case?)
	self->position.x += self->velocity.x * 3;
	self->position.y += self->velocity.y * 3;
	self->timeToLive -= 1;
	if (self->timeToLive <= 0) entity_free(self);
}

/*eol@eof*/