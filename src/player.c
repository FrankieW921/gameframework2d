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
	self->type = ET_Player;
	self->think = player_think;
	self->update = player_update;

	//player specific attributes/data
	data = gfc_allocate_array(sizeof(PlayerData), 1);
	if (data) {
		player_data_new(data);
	}
	self->data = data;
	player_do_max_health(self);
	//player_output_current_head(self);

	gfc_vector2d_copy(self->position, position);
	self->sprite = gf2d_sprite_load_all(
		"images/space_bug.png",
		128,
		128,
		0,
		0
	);
	self->bounds = gfc_rect(self->position.x, self->position.y, 128, 128);

	gfc_input_init("gfc/sample_config/input.cfg");
	thePlayer = self;
	return self;
}

void player_data_new(PlayerData* data) {
	SJson* file;

	data->currentHealth = 100; //this is what the default parts add up to
	data->shootCooldown = 0;
	data->partSwitchCooldown = 0;
	data->iTime = 0;
	data->headIndex = 0;
	data->armIndex = 0;
	data->torsoIndex = 0;
	data->legIndex = 0;

	file = sj_load("defs/heads.json");
	data->heads = sj_object_get_value(file, "heads");
	data->headIndexMax = sj_array_get_count(data->heads) - 1;
	data->currentHead = gfc_allocate_array(sizeof(Head), 1);
	player_set_head(data->currentHead, sj_array_get_nth(data->heads, data->headIndex));

	file = sj_load("defs/arms.json");
	data->arms = sj_object_get_value(file, "arms");
	data->armIndexMax = sj_array_get_count(data->arms) - 1;
	data->currentArm = gfc_allocate_array(sizeof(Arm), 1);
	player_set_arm(data->currentArm, sj_array_get_nth(data->arms, data->armIndex));

	file = sj_load("defs/torsos.json");
	data->torsos = sj_object_get_value(file, "torsos");
	data->torsoIndexMax = sj_array_get_count(data->torsos) - 1;
	data->currentTorso = gfc_allocate_array(sizeof(Torso), 1);
	player_set_torso(data->currentTorso, sj_array_get_nth(data->torsos, data->torsoIndex));

	file = sj_load("defs/legs.json");
	data->legs = sj_object_get_value(file, "legs");
	data->legIndexMax = sj_array_get_count(data->legs) - 1;
	data->currentLeg = gfc_allocate_array(sizeof(Leg), 1);
	player_set_leg(data->currentLeg, sj_array_get_nth(data->legs, data->legIndex));

}

void player_think(Entity* self) {
	Uint32 mouseState;
	PlayerData* data;
	if (!self)return;
	data = self->data;
	if (!data)return;
	//cooldowns
	if (data->shootCooldown > 0)data->shootCooldown -= 1;
	if (data->partSwitchCooldown > 0)data->partSwitchCooldown -= 1;
	if (data->iTime > 0)data->iTime -= 1;
	//input
	GFC_Vector2D movement = { 0 };
	self->velocity.x = 0;
	self->velocity.y = 0;
	gfc_input_update();
	if (gfc_input_command_down("moveUp")) {
		self->velocity.y -= 1;
	}
	if (gfc_input_command_down("moveDown")) {
		self->velocity.y += 1;
	}
	if (gfc_input_command_down("moveRight")) {
		self->velocity.x += 1;
	}
	if (gfc_input_command_down("moveLeft")) {
		self->velocity.x -= 1;
	}
	if (gfc_input_command_down("nextHead") && data->partSwitchCooldown == 0) {
		data->partSwitchCooldown = 70;
		player_next_head(self);
		player_do_max_health(self);
	}
	if (gfc_input_command_down("nextArm") && data->partSwitchCooldown == 0) {
		data->partSwitchCooldown = 70;
		player_next_arm(self);
	}
	if (gfc_input_command_down("nextTorso") && data->partSwitchCooldown == 0) {
		data->partSwitchCooldown = 70;
		player_next_torso(self);
		player_do_max_health(self);
	}
	if (gfc_input_command_down("nextLeg") && data->partSwitchCooldown == 0) {
		data->partSwitchCooldown = 70;
		player_next_leg(self);
		player_do_max_health(self);
	}

	gfc_vector2d_normalize(&self->velocity);
	mouseState = SDL_GetRelativeMouseState(NULL, NULL);
	if ((mouseState & 1) && data->shootCooldown == 0) {
		player_shoot(self->position, self->velocity, self); 
		data->shootCooldown = data->currentHead->cooldownValue;
	}

	self->collideEntities = entity_collide_all(self);
}

void player_update(Entity* self) {
	PlayerData* data;
	Entity* collider;
	ProjectileData* projectileData;
	int i;
	data = self->data;
	if (!data)return;

	if (self->collideEntities) {
		for (i = 0; i < self->collideEntities->size; i++) {
			collider = gfc_list_get_nth(self->collideEntities, i);
			if (collider) {
				if (collider->type == ET_Enemy && data->iTime <=0) {
					data->currentHealth -= 10;
					data->iTime = data->currentTorso->iTime;
					slog("Player iTime Activated: %i", data->iTime);
				}
				if (collider->type == ET_EnemyProjectile && data->iTime <= 0) {
					projectileData = collider->data;
					data->iTime = data->currentTorso->iTime;
					data->currentHealth -= projectileData->damage;
					slog("Player iTime Activated: %i", data->iTime);
					entity_free(collider);
				}
			}
		}
	}
	
	self->position.x += self->velocity.x * data->currentLeg->speed;
	self->position.y += self->velocity.y * data->currentLeg->speed;
	self->bounds.x = self->position.x;
	self->bounds.y = self->position.y;

	camera_center_on(self->position);
	camera_bounds_check();

	gfc_list_clear(self->collideEntities);
}

void player_shoot(GFC_Vector2D position, GFC_Vector2D velocity, Entity* self) {
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
	sj_object_get_value_as_int(selectedHead, "cooldownValue", &currentHead->cooldownValue);
	slog("Head switched");
	slog("Head: %s", currentHead->name);
	slog("Head Health: %i", currentHead->health);
	slog("Shoot Cooldown: %i", currentHead->cooldownValue);
}

void player_set_arm(Arm* currentArm, SJson* selectedArm) {
	if (!currentArm) {
		return;
	}
	currentArm->name = sj_object_get_value_as_string(selectedArm, "name");
	sj_object_get_value_as_int(selectedArm, "projectileType", &currentArm->projectileType);
	slog("Arm switched");
	slog("Arm: %s", currentArm->name);
	slog("Projectile Type/Index: %i", currentArm->projectileType);
}

void player_set_torso(Torso* currentTorso, SJson* selectedTorso) {
	if (!currentTorso) {
		return;
	}
	currentTorso->name = sj_object_get_value_as_string(selectedTorso, "name");
	sj_object_get_value_as_int(selectedTorso, "health", &currentTorso->health);
	sj_object_get_value_as_int(selectedTorso, "iTime", &currentTorso->iTime);
	slog("Torso switched");
	slog("Torso: %s", currentTorso->name);
	slog("Torso Health: %i", currentTorso->health);
	slog("Torso Invincibility Time: %i", currentTorso->iTime);
}

void player_set_leg(Leg* currentLeg, SJson* selectedLeg) {
	if (!currentLeg) {
		return;
	}
	currentLeg->name = sj_object_get_value_as_string(selectedLeg, "name");
	sj_object_get_value_as_int(selectedLeg, "health", &currentLeg->health);
	sj_object_get_value_as_float(selectedLeg, "speed", &currentLeg->speed);
	slog("Leg switched");
	slog("Leg: %s", currentLeg->name);
	slog("Leg Health: %i", currentLeg->health);
	slog("Leg Speed: %f", currentLeg->speed);
}

void player_next_head(Entity* self) {
	PlayerData* data;
	data = self->data;
	if (!data)return;

	data->headIndex += 1;
	if (data->headIndex > data->headIndexMax) {
		data->headIndex = 0;
	}
	player_set_head(data->currentHead, sj_array_get_nth(data->heads, data->headIndex));
}

void player_next_arm(Entity* self) {
	PlayerData* data;
	data = self->data;
	if (!data)return;

	data->armIndex += 1;
	if (data->armIndex > data->armIndexMax) {
		data->armIndex = 0;
	}
	player_set_arm(data->currentArm, sj_array_get_nth(data->arms, data->armIndex));
}

void player_next_torso(Entity* self) {
	PlayerData* data;
	data = self->data;
	if (!data)return;

	data->torsoIndex += 1;
	if (data->torsoIndex > data->torsoIndexMax) {
		data->torsoIndex = 0;
	}
	player_set_torso(data->currentTorso, sj_array_get_nth(data->torsos, data->torsoIndex));
}

void player_next_leg(Entity* self) {
	PlayerData* data;
	data = self->data;
	if (!data)return;

	data->legIndex += 1;
	if (data->legIndex > data->legIndexMax) {
		data->legIndex = 0;
	}
	player_set_leg(data->currentLeg, sj_array_get_nth(data->legs, data->legIndex));
}

void player_do_max_health(Entity* self) {
	PlayerData* data;
	int countHealth = 0;
	data = self->data;
	if (!data)return;

	countHealth += data->currentHead->health;
	countHealth += data->currentTorso->health;
	countHealth += data->currentLeg->health;
	data->maxHealth = countHealth;
	if (data->currentHealth > data->maxHealth) {
		data->currentHealth = data->maxHealth;
	}
	slog("New Player Max Health: %i", data->maxHealth);
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
