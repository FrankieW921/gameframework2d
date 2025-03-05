#include "simple_logger.h"
#include "gfc_input.h"

#include "player.h"
#include "camera.h"
#include "projectile.h"

static Entity* thePlayer = NULL;

Entity* get_the_player() {
	return thePlayer;
}

Entity *player_new_entity(GFC_Vector2D position)
{
	Entity* self;
	PlayerData* data;

	if (thePlayer) {
		return thePlayer;
	}

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn player entity");
		return NULL;
	}
	
	//entity general attributes
	self->think = player_think;
	self->update = player_update;

	//player specific attributes/data
	data = gfc_allocate_array(sizeof(PlayerData), 1);
	if (data) {
		player_data_new(data);
	}
	self->data = data;
	player_output_current_head(self);

	//slog("Current head name: %s", data->currentHead->name);
	//slog("Current head health: %i", data->currentHead->health);

	gfc_vector2d_copy(self->position, position);
	self->sprite = gf2d_sprite_load_all(
		"images/space_bug.png",
		128,
		128,
		0,
		0
	);
	gfc_input_init("gfc/sample_config/input.cfg");

	thePlayer = self;
	return self;
}

void player_data_new(PlayerData* data) {
	SJson* file;

	file = sj_load("defs/heads.json");
	data->heads = sj_object_get_value(file, "heads");
	data->currentHead = gfc_allocate_array(sizeof(Head), 1);
	player_set_head(data->currentHead, sj_array_get_nth(data->heads, 0));

	file = sj_load("defs/arms.json");
	data->arms = sj_object_get_value(file, "arms");
	data->currentArm = gfc_allocate_array(sizeof(Arm), 1);
	player_set_arm(data->currentArm, sj_array_get_nth(data->arms, 0));

	file = sj_load("defs/torsos.json");
	data->torsos = sj_object_get_value(file, "torsos");
	data->currentTorso = gfc_allocate_array(sizeof(Torso), 1);
	player_set_torso(data->currentTorso, sj_array_get_nth(data->torsos, 0));

	file = sj_load("defs/legs.json");
	data->legs = sj_object_get_value(file, "legs");
	data->currentLeg = gfc_allocate_array(sizeof(Leg), 1);
	player_set_leg(data->currentLeg, sj_array_get_nth(data->legs, 0));

	data->shootCooldown = 0;
	data->cooldownValue = 60;  //current implementation is hard to create an actual time metric for
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
		player_shoot(self->position, self->velocity, self); //TODO update to pass in projectile type to spawn
		data->shootCooldown = data->cooldownValue;
	}
}

void player_update(Entity* self) {
	self->position.x += self->velocity.x * 3;
	self->position.y += self->velocity.y * 3;
	camera_center_on(self->position);
	camera_bounds_check();
}

void player_shoot(GFC_Vector2D position, GFC_Vector2D velocity, Entity* self) { //TODO update to pass in projectile type to spawn
	int mx, my;
	GFC_Vector2D pv, offset, playerOffset;
	PlayerData* data = self->data;
	Arm* arm = data->currentArm;

	SDL_GetMouseState(&mx, &my);
	offset = camera_get_offset();
	playerOffset = gfc_vector2d(position.x, position.y);
	gfc_vector2d_add(playerOffset, playerOffset, offset);

	pv = gfc_vector2d(mx-playerOffset.x, my-playerOffset.y); //projectile velocity
	gfc_vector2d_normalize(&pv);

	projectile_new_entity(position, pv, arm->projectileType);
}

void player_set_head(Head* currentHead, SJson* selectedHead) {
	if (!currentHead) {
		return;
	}
	currentHead->name = sj_object_get_value_as_string(selectedHead, "name");
	sj_object_get_value_as_int(selectedHead, "health", &currentHead->health);
}

void player_set_arm(Arm* currentArm, SJson* selectedArm) {
	if (!currentArm) {
		return;
	}
	currentArm->name = sj_object_get_value_as_string(selectedArm, "name");
	sj_object_get_value_as_int(selectedArm, "projectileType", &currentArm->projectileType);
}

void player_set_torso(Torso* currentTorso, SJson* selectedTorso) {
	if (!currentTorso) {
		return;
	}
	currentTorso->name = sj_object_get_value_as_string(selectedTorso, "name");
	sj_object_get_value_as_int(selectedTorso, "health", &currentTorso->health);
}

void player_set_leg(Leg* currentLeg, SJson* selectedLeg) {
	if (!currentLeg) {
		return;
	}
	currentLeg->name = sj_object_get_value_as_string(selectedLeg, "name");
	sj_object_get_value_as_int(selectedLeg, "health", &currentLeg->health);
}

void player_output_current_head(Entity* self) {
	PlayerData* data;
	Head* currentHead;
	if (!self) {
		slog("Self doesn't exist");
		return;
	}
	data = self->data;
	if (data) {
		currentHead = data->currentHead;
		if (currentHead) {
			slog("Current Head Name: %s", currentHead->name);
			slog("Current Head Health: %i", currentHead->health);
		}
		else {
			slog("Self->Data->CurrentHead doesn't exist");
		}
		
	}
	else {
		slog("Self->Data doesn't exist");
	}
}
