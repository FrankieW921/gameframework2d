#include "simple_logger.h"
#include "gfc_input.h"

#include "player.h"
#include "camera.h"
#include "projectile.h"

Entity *player_new_entity(GFC_Vector2D position)
{
	Entity* self;
	PlayerData* data;
	SJson* file;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn player entity");
		return NULL;
	}
	//entity general attributes
	self->think = player_think;
	self->update = player_update;

	//player specific attributes
	data = gfc_allocate_array(sizeof(PlayerData), 1);
	if (data) { //pray this executes or crash
		data->shootCooldown = 0;
		data->cooldownValue = 60;  //current implementation is hard to create an actual time metric for
		file = sj_load("defs/heads.json");
		data->heads = sj_object_get_value(file, "heads");
		data->currentHead = player_set_head(sj_array_get_nth(data->heads, 0));

		file = sj_load("defs/arms.json");
		data->arms = sj_object_get_value(file, "arms");

		file = sj_load("defs/torsos.json");
		data->torsos = sj_object_get_value(file, "torsos");

		file = sj_load("defs/legs.json");
		data->legs = sj_object_get_value(file, "legs");
	}
	self->data = data;

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
	PlayerData* data;

	if (!self)return;

	data = self->data;

	if (data->shootCooldown > 0)data->shootCooldown -= 1;

	GFC_Vector2D movement = { 0 };
	self->velocity.x = 0;
	self->velocity.y = 0;
	gfc_input_update();
	if (gfc_input_command_down("moveUp")) {
		self->velocity.y -= 1;
		//slog("moveUp is held down");
	}
	if (gfc_input_command_down("moveDown")) {
		self->velocity.y += 1;
		//slog("moveDown is held down");
	}
	if (gfc_input_command_down("moveRight")) {
		self->velocity.x += 1;
		//slog("moveRight is held down");
	}
	if (gfc_input_command_down("moveLeft")) {
		self->velocity.x -= 1;
		//slog("moveLeft is held down");
	}


	gfc_vector2d_normalize(&self->velocity);

	mouseState = SDL_GetRelativeMouseState(NULL, NULL);
	if ((mouseState & 1) && data->shootCooldown == 0) {
		player_shoot(self->position, self->velocity); //TODO update to pass in projectile type to spawn
		data->shootCooldown = data->cooldownValue;
	}
}

void player_update(Entity* self) {
	self->position.x += self->velocity.x * 3;
	self->position.y += self->velocity.y * 3;
	camera_center_on(self->position);
	camera_bounds_check();
}

void player_shoot(GFC_Vector2D position, GFC_Vector2D velocity) { //TODO update to pass in projectile type to spawn
	int mx, my;
	GFC_Vector2D pv, offset, playerOffset;

	SDL_GetMouseState(&mx, &my);
	offset = camera_get_offset();
	playerOffset = gfc_vector2d(position.x, position.y);
	gfc_vector2d_add(playerOffset, playerOffset, offset);

	pv = gfc_vector2d(mx-playerOffset.x, my-playerOffset.y); //projectile velocity
	gfc_vector2d_normalize(&pv);

	projectile_new_entity(position, pv);
}

Head* player_set_head(SJson* selectedHead) {
	Head* rHead = gfc_allocate_array(sizeof(Head), 1);
	rHead->name = sj_object_get_value_as_string(selectedHead, "name");
	slog(rHead->name);
	return rHead;
}

Arm* player_set_arm(SJson* selectedArm) {
	
}

Torso* player_set_torso(SJson* selectedTorso) {

}

Leg* player_set_leg(SJson* selectedLeg) {

}

void player_update_max_health(Entity* self) {

}
