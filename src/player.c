#include <SDL_mixer.h>
#include "simple_logger.h"
#include "gfc_input.h"

#include "player.h"
#include "world.h"
#include "camera.h"
#include "projectile.h"
#include "interactables.h"
#include "particles.h"
#include "door.h"

static Entity* thePlayer = NULL;

Entity* get_the_player() {
	if (thePlayer) {
		return thePlayer;
	}
}

void move_the_player(GFC_Vector2D newPosition) {
	if (!thePlayer) {
		return NULL;
	}
	gfc_vector2d_copy(thePlayer->position, newPosition);
}

Entity *player_new_entity(GFC_Vector2D position)
{
	Entity* self;
	PlayerData* data;
	Mix_Music* battle_a2;

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
	/*
	self->sprite = gf2d_sprite_load_all(
		"images/space_bug.png",
		128,
		128,
		0,
		0
	);
	*/
	self->bounds = gfc_rect(self->position.x, self->position.y, 32, 96);

	//gfc_input_init("gfc/sample_config/input.cfg");
	thePlayer = self;
	Mix_HaltMusic();
	battle_a2 = Mix_LoadMUS("music/Battle-a2.mp3");
	Mix_PlayMusic(battle_a2, -1);

	return self;
}

void player_data_new(PlayerData* data) {
	SJson* file;

	data->currentHealth = 100; //this is what the default parts add up to
	data->shootCooldown = 0;
	data->partSwitchCooldown = 0;
	data->iTime = 0;
	data->canChangeParts = 0;

	data->headIndex = 0;
	data->armIndex = 0;
	data->torsoIndex = 0;
	data->legIndex = 0;


	file = sj_load("defs/heads.json");
	data->heads = sj_object_get_value(file, "heads");
	//data->headIndexMax = sj_array_get_count(data->heads) - 1;
	data->currentHead = gfc_allocate_array(sizeof(Head), 1);
	data->headInventory = gfc_list_new();
	player_set_head(data->currentHead, sj_array_get_nth(data->heads, data->headIndex));
	gfc_list_append(data->headInventory, data->currentHead);
	data->headIndexMax = gfc_list_get_count(data->headInventory);
	slog("headIndexMax: %i", data->headIndexMax);


	file = sj_load("defs/arms.json");
	data->arms = sj_object_get_value(file, "arms");
	//data->armIndexMax = sj_array_get_count(data->arms) - 1;
	data->currentArm = gfc_allocate_array(sizeof(Arm), 1);
	data->armInventory = gfc_list_new();
	player_set_arm(data->currentArm, sj_array_get_nth(data->arms, data->armIndex));
	gfc_list_append(data->armInventory, data->currentArm);
	data->armIndexMax = gfc_list_get_count(data->armInventory);

	file = sj_load("defs/torsos.json");
	data->torsos = sj_object_get_value(file, "torsos");
	//data->torsoIndexMax = sj_array_get_count(data->torsos) - 1;
	data->currentTorso = gfc_allocate_array(sizeof(Torso), 1);
	data->torsoInventory = gfc_list_new();
	player_set_torso(data->currentTorso, sj_array_get_nth(data->torsos, data->torsoIndex));
	gfc_list_append(data->torsoInventory, data->currentTorso);
	data->torsoIndexMax = gfc_list_get_count(data->torsoInventory);

	file = sj_load("defs/legs.json");
	data->legs = sj_object_get_value(file, "legs");
	//data->legIndexMax = sj_array_get_count(data->legs) - 1;
	data->currentLeg = gfc_allocate_array(sizeof(Leg), 1);
	data->legInventory = gfc_list_new();
	player_set_leg(data->currentLeg, sj_array_get_nth(data->legs, data->legIndex));
	gfc_list_append(data->legInventory, data->currentLeg);
	data->legIndexMax = gfc_list_get_count(data->legInventory);

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
	//gfc_input_update();
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
	if (gfc_input_command_down("nextHead") && data->partSwitchCooldown == 0 && data->canChangeParts) {
		data->partSwitchCooldown = 70;
		player_next_head(self);
		player_do_max_health(self);
	}
	if (gfc_input_command_down("nextArm") && data->partSwitchCooldown == 0 && data->canChangeParts) {
		data->partSwitchCooldown = 70;
		player_next_arm(self);
	}
	if (gfc_input_command_down("nextTorso") && data->partSwitchCooldown == 0 && data->canChangeParts) {
		data->partSwitchCooldown = 70;
		player_next_torso(self);
		player_do_max_health(self);
	}
	if (gfc_input_command_down("nextLeg") && data->partSwitchCooldown == 0 && data->canChangeParts) {
		data->partSwitchCooldown = 70;
		player_next_leg(self);
		player_do_max_health(self);
	}
	/*
	if (gfc_input_command_down("spawnHeal") && data->partSwitchCooldown == 0) {
		slog("PRESSING HEAL");
		data->partSwitchCooldown = 70;
		spawn_interactable_command(self, Healing_Field);
	}
	if (gfc_input_command_down("spawnSpeed") && data->partSwitchCooldown == 0) {
		data->partSwitchCooldown = 70;
		spawn_interactable_command(self, Speed_Gel);
	}
	if (gfc_input_command_down("spawnChanger") && data->partSwitchCooldown == 0) {
		data->partSwitchCooldown = 70;
		spawn_interactable_command(self, Part_Changer);
	}
	if (gfc_input_command_down("spawnStar") && data->partSwitchCooldown == 0) {
		data->partSwitchCooldown = 70;
		spawn_interactable_command(self, Star_Power);
	}
	if (gfc_input_command_down("spawnTeleport") && data->partSwitchCooldown == 0) {
		data->partSwitchCooldown = 70;
		spawn_interactable_command(self, Teleporter);
	}
	*/

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
	DoorData* doorData;
	PartPickupData* partpickupData;
	int i;
	char doorMapName[128];
	int doorSpawnIndex;
	data = self->data;
	if (!data)return;

	self->velocity.x *= data->currentLeg->speed;
	self->velocity.y *= data->currentLeg->speed;

	if (self->collideEntities) {
		for (i = 0; i < self->collideEntities->size; i++) {
			collider = gfc_list_get_nth(self->collideEntities, i);
			if (collider) {
				if ((collider->type == ET_Enemy || collider->type == ET_Boss) && data->iTime <=0) {
					data->currentHealth -= 10; //implement contact damage?
					data->iTime = data->currentTorso->iTime;
					particle_spark(self->position, 3, 20);
					slog("Player iTime Activated: %i", data->iTime);
				}
				else if (collider->type == ET_EnemyProjectile && data->iTime <= 0) {
					projectileData = collider->data;
					data->iTime = data->currentTorso->iTime;
					data->currentHealth -= projectileData->damage;
					slog("Player iTime Activated: %i", data->iTime);
					particle_spark(self->position, 3, 20);
					entity_free(collider);
				}
				else if (collider->type == Door) {
					doorData = collider->data; //need the data before we free the world
					if (!doorData->mapName) {
						slog("NO MAP NAME");
					}
					else {
						slog("PLAYER TOUCHED DOOR: %s, %i", doorData->mapName, doorData->playerSpawnIndex);
					}
					strcpy(&doorMapName, doorData->mapName);
					doorSpawnIndex = doorData->playerSpawnIndex;
					slog("Stored door data: %s, %i", doorMapName, doorSpawnIndex);
					
					world_free(get_current_world());
					world_load(doorMapName, doorSpawnIndex);
				}
				else if (collider->type == ET_PartPickup) {
					partpickupData = collider->data;
					if (partpickupData)
					{
						player_add_part_to_inventory(self, partpickupData);
					}
					partpickup_free(collider);
				}
				else if (collider->type == Healing_Field) {
					if (data->currentHealth < data->maxHealth) {
						data->currentHealth += 1;
					}
				}
				else if (collider->type == Speed_Gel) {
					self->velocity.x *= 3;
					self->velocity.y *= 3;
				}
				else if (collider->type == Part_Changer) {
					data->canChangeParts = true;
				}
				else if (collider->type == Star_Power) {
					entity_free(collider);
					data->iTime = 1000;
				}
				else if (collider->type == Teleporter) {
					self->position.x -= 240;
					self->position.y -= 400;
				}
			}
		}
	}
	else {
		data->canChangeParts = false; //probably a better way to do this, but since when you are changing parts there shouldnt be enemies around, its okay (gulp)
	}
	
	entity_move(self);

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
	switch (arm->projectileType) {
		case 0:
			particle_spark(position, 0, 10);
			break;
		case 1:
			particle_spark(position, 1, 20);
			break;
		case 2:
			particle_spark(position, 0, 20);
			break;
	}
}

void player_draw(Entity* self){
	GFC_Vector2D headPosition, torsoPosition, armPosition, legPosition, offset;
	int mx, my;
	GFC_Vector2D armRotationVector, playerOffset;
	float armRotation;
	PlayerData* data;
	GFC_Rect drawRect;
	if (!self) return;
	data = self->data;
	if (!data) return;

	offset = camera_get_offset();
	gfc_vector2d_add(headPosition, self->position, offset); //head position
	gfc_vector2d_add(torsoPosition, self->position, offset);
	gfc_vector2d_add(torsoPosition, torsoPosition, gfc_vector2d(0, 32)); //torso position
	gfc_vector2d_add(armPosition, self->position, offset);
	gfc_vector2d_add(armPosition, armPosition, gfc_vector2d(32, 32)); //arm position
	gfc_vector2d_add(legPosition, self->position, offset);
	gfc_vector2d_add(legPosition, legPosition, gfc_vector2d(0, 64)); //leg position

	/*
	SDL_GetMouseState(&mx, &my);
	armRotationVector = gfc_vector2d(mx-armPosition.x, my-armPosition.y);
	gfc_vector2d_normalize(&armRotationVector);
	*/

	gf2d_sprite_draw(
		data->currentHead->headSprite,
		headPosition,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame);
	gf2d_sprite_draw(
		data->currentTorso->torsoSprite,
		torsoPosition,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame);
	gf2d_sprite_draw(
		data->currentArm->armSprite,
		armPosition,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame);
	gf2d_sprite_draw(
		data->currentLeg->legSprite,
		legPosition,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame);


	drawRect = self->bounds;
	drawRect.x += offset.x;
	drawRect.y += offset.y;
	gf2d_draw_rect(drawRect, GFC_COLOR_RED);
}

void player_set_head(Head* currentHead, SJson* selectedHead) {
	const char* imageString;
	if (!currentHead) {
		return;
	}
	strcpy(currentHead->name, sj_object_get_value_as_string(selectedHead, "name"));
	sj_object_get_value_as_int(selectedHead, "health", &currentHead->health);
	sj_object_get_value_as_int(selectedHead, "cooldownValue", &currentHead->cooldownValue);
	imageString = sj_object_get_value_as_string(selectedHead, "sprite");
	if (currentHead->headSprite) {
		gf2d_sprite_free(currentHead->headSprite);
	}
	currentHead->headSprite = gf2d_sprite_load_all(
		imageString,
		32,
		32,
		0,
		0
	);
	slog("Head switched or added to inventory");
	slog("Head: %s", currentHead->name);
	slog("Head Health: %i", currentHead->health);
	slog("Shoot Cooldown: %i", currentHead->cooldownValue);
}

void player_set_arm(Arm* currentArm, SJson* selectedArm) {
	const char* imageString;
	if (!currentArm) {
		return;
	}
	//currentArm->name = sj_object_get_value_as_string(selectedArm, "name");
	strcpy(currentArm->name, sj_object_get_value_as_string(selectedArm, "name"));
	sj_object_get_value_as_int(selectedArm, "projectileType", &currentArm->projectileType);
	imageString = sj_object_get_value_as_string(selectedArm, "sprite");
	if (currentArm->armSprite) {
		gf2d_sprite_free(currentArm->armSprite);
	}
	currentArm->armSprite = gf2d_sprite_load_all(
		imageString,
		32,
		32,
		0,
		0
	);
	slog("Arm switched or added to inventory");
	slog("Arm: %s", currentArm->name);
	slog("Projectile Type/Index: %i", currentArm->projectileType);
}

void player_set_torso(Torso* currentTorso, SJson* selectedTorso) {
	const char* imageString;
	if (!currentTorso) {
		return;
	}
	///currentTorso->name = sj_object_get_value_as_string(selectedTorso, "name");
	strcpy(currentTorso->name, sj_object_get_value_as_string(selectedTorso, "name"));
	sj_object_get_value_as_int(selectedTorso, "health", &currentTorso->health);
	sj_object_get_value_as_int(selectedTorso, "iTime", &currentTorso->iTime);
	imageString = sj_object_get_value_as_string(selectedTorso, "sprite");
	if (currentTorso->torsoSprite) {
		gf2d_sprite_free(currentTorso->torsoSprite);
	}
	currentTorso->torsoSprite = gf2d_sprite_load_all(
		imageString,
		32,
		32,
		0,
		0
	);
	slog("Torso switched or added to inventory");
	slog("Torso: %s", currentTorso->name);
	slog("Torso Health: %i", currentTorso->health);
	slog("Torso Invincibility Time: %i", currentTorso->iTime);
}

void player_set_leg(Leg* currentLeg, SJson* selectedLeg) {
	const char* imageString;
	if (!currentLeg) {
		return;
	}
	//currentLeg->name = sj_object_get_value_as_string(selectedLeg, "name");
	strcpy(currentLeg->name, sj_object_get_value_as_string(selectedLeg, "name"));
	sj_object_get_value_as_int(selectedLeg, "health", &currentLeg->health);
	sj_object_get_value_as_float(selectedLeg, "speed", &currentLeg->speed);
	imageString = sj_object_get_value_as_string(selectedLeg, "sprite");
	if (currentLeg->legSprite) {
		gf2d_sprite_free(currentLeg->legSprite);
	}
	currentLeg->legSprite = gf2d_sprite_load_all(
		imageString,
		32,
		32,
		0,
		0
	);
	slog("Leg switched or added to inventory");
	slog("Leg: %s", currentLeg->name);
	slog("Leg Health: %i", currentLeg->health);
	slog("Leg Speed: %f", currentLeg->speed);
}

void player_next_head(Entity* self) {
	PlayerData* data;
	data = self->data;
	if (!data)return;

	data->headIndex += 1;
	if (data->headIndex >= data->headIndexMax) {
		data->headIndex = 0;
	}
	//player_set_head(data->currentHead, sj_array_get_nth(data->heads, data->headIndex));
	data->currentHead = gfc_list_get_nth(data->headInventory, data->headIndex);
	slog("Player switch head from inventory");
	slog("Head: %s", data->currentHead->name);
	slog("Head Health: %i", data->currentHead->health);
	slog("Shoot Cooldown: %i", data->currentHead->cooldownValue);
}

void player_next_arm(Entity* self) {
	PlayerData* data;
	data = self->data;
	if (!data)return;

	data->armIndex += 1;
	if (data->armIndex >= data->armIndexMax) {
		data->armIndex = 0;
	}
	//player_set_arm(data->currentArm, sj_array_get_nth(data->arms, data->armIndex));
	data->currentArm = gfc_list_get_nth(data->armInventory, data->armIndex);
	slog("Player switched arm from inventory");
	slog("Arm: %s", data->currentArm->name);
	slog("Projectile Type/Index: %i", data->currentArm->projectileType);
}

void player_next_torso(Entity* self) {
	PlayerData* data;
	data = self->data;
	if (!data)return;

	data->torsoIndex += 1;
	if (data->torsoIndex >= data->torsoIndexMax) {
		data->torsoIndex = 0;
	}
	//player_set_torso(data->currentTorso, sj_array_get_nth(data->torsos, data->torsoIndex));
	data->currentTorso = gfc_list_get_nth(data->torsoInventory, data->torsoIndex);
	slog("Player switched torso from inventory");
	slog("Torso: %s", data->currentTorso->name);
	slog("Torso Health: %i", data->currentTorso->health);
	slog("Torso Invincibility Time: %i", data->currentTorso->iTime);
}

void player_next_leg(Entity* self) {
	PlayerData* data;
	data = self->data;
	if (!data)return;

	data->legIndex += 1;
	if (data->legIndex >= data->legIndexMax) {
		data->legIndex = 0;
	}
	//player_set_leg(data->currentLeg, sj_array_get_nth(data->legs, data->legIndex));
	data->currentLeg = gfc_list_get_nth(data->legInventory, data->legIndex);
	slog("Player switched leg from inventory");
	slog("Leg: %s", data->currentLeg->name);
	slog("Leg Health: %i", data->currentLeg->health);
	slog("Leg Speed: %f", data->currentLeg->speed);
}

void player_add_part_to_inventory(Entity* self, PartPickupData* partpickupData) {
	PlayerData* data;
	SJson* partJson;
	Head* headToAdd;
	Arm* armToAdd;
	Torso* torsoToAdd;
	Leg* legToAdd;

	if (!self || !partpickupData) return;
	data = self->data;
	if (!data) return;

	switch (partpickupData->partPickupType) {
		case PPT_Head:
			headToAdd = gfc_allocate_array(sizeof(Head), 1);
			partJson = sj_array_get_nth(data->heads, partpickupData->partDefIndex);
			player_set_head(headToAdd, partJson);
			gfc_list_append(data->headInventory, headToAdd);
			data->headIndexMax = (Uint8)gfc_list_get_count(data->headInventory);
			break;
		case PPT_Arm:
			armToAdd = gfc_allocate_array(sizeof(Arm), 1);
			partJson = sj_array_get_nth(data->arms, partpickupData->partDefIndex);
			player_set_arm(armToAdd, partJson);
			gfc_list_append(data->armInventory, armToAdd);
			data->armIndexMax = (Uint8)gfc_list_get_count(data->armInventory);
			break;
		case PPT_Torso:
			slog("Adding torso to inv");
			torsoToAdd = gfc_allocate_array(sizeof(Torso), 1);
			partJson = sj_array_get_nth(data->torsos, partpickupData->partDefIndex);
			player_set_torso(torsoToAdd, partJson);
			gfc_list_append(data->torsoInventory, torsoToAdd);
			data->torsoIndexMax = (Uint8)gfc_list_get_count(data->torsoInventory);
			break;
		case PPT_Leg:
			legToAdd = gfc_allocate_array(sizeof(Leg), 1);
			partJson = sj_array_get_nth(data->legs, partpickupData->partDefIndex);
			player_set_leg(legToAdd, partJson);
			gfc_list_append(data->legInventory, legToAdd);
			data->legIndexMax = (Uint8)gfc_list_get_count(data->legInventory);
			break;
	}
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

void spawn_interactable_command(Entity* self, Uint8 type) {
	slog("SPAWN COMMAND STARTED");
	interactable_new(self->position, type);
}
