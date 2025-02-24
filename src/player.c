#include "simple_logger.h"

#include "player.h"
#include "projectile.h"
#include "gfc_input.h"

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
	gfc_input_init("gfc/sample_config/input.cfg");
	return self;
}

void player_think(Entity* self) {
	Uint32 mouseState;

	if (!self)return;
	GFC_Vector2D movement = { 0 };
	self->velocity.x = 0;
	self->velocity.y = 0;
	gfc_input_update();
	if (gfc_input_command_down("moveUp")) {
		//movement.y -= 1;
		self->velocity.y -= 1;
		slog("moveUp is held down");
	}
	if (gfc_input_command_down("moveDown")) {
		//movement.y += 1;
		self->velocity.y += 1;
		slog("moveDown is held down");
	}
	if (gfc_input_command_down("moveRight")) {
		//movement.x += 1;
		self->velocity.x += 1;
		slog("moveRight is held down");
	}
	if (gfc_input_command_down("moveLeft")) {
		//movement.x -= 1;
		self->velocity.x -= 1;
		slog("moveLeft is held down");
	}
	gfc_vector2d_normalize(&self->velocity);
	self->position.x += self->velocity.x * .8;
	self->position.y += self->velocity.y * .8;

	mouseState = SDL_GetRelativeMouseState(NULL, NULL);
	if (mouseState & 1) {
		player_shoot(self->position, self->velocity); //TODO update to pass in projectile type to spawn
	}
}

void player_shoot(GFC_Vector2D position, GFC_Vector2D velocity) { //TODO update to pass in projectile type to spawn
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	GFC_Vector2D pv = gfc_vector2d(mx-position.x, my-position.y);
	gfc_vector2d_normalize(&pv);

	projectile_new_entity(position, pv);
}

/*eol@eof*/