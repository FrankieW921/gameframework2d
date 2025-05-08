#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"
#include "projectile.h"
#include "partpickup.h"

typedef struct {
	const char*			name[128];
	Uint8			health;
	Uint8			cooldownValue; //shootCooldown gets set to this value, this value changes with the selected head
	Sprite*			headSprite;
}Head;

typedef struct {
	const char*			name[128];
	ProjectileType	projectileType;
	Sprite*		armSprite;
}Arm;

typedef struct {
	const char*			name[128];
	Uint8			health;
	Uint8			iTime;
	Sprite*		torsoSprite;
}Torso;

typedef struct {
	const char*			name[128];
	Uint8			health;
	float			speed;
	Sprite*		legSprite;
}Leg;

typedef struct { //mfw i want to make a mecha game
	Sint32			maxHealth;
	Sint32			currentHealth;
	Uint8			shootCooldown; //millisecond countdown/cooldown for player firing
	Uint8			partSwitchCooldown;
	Uint32			iTime;
	Bool			canChangeParts;

	Uint8			headIndex;
	Uint8			armIndex;
	Uint8			torsoIndex;
	Uint8			legIndex;
	Uint8			headIndexMax;
	Uint8			armIndexMax;
	Uint8			torsoIndexMax;
	Uint8			legIndexMax;

	SJson*			heads;
	SJson*			arms;
	SJson*			torsos;
	SJson*			legs;
	GFC_List*		headInventory;
	GFC_List*		armInventory;
	GFC_List*		torsoInventory;
	GFC_List*		legInventory;
	Head			*currentHead;
	Arm				*currentArm;
	Torso			*currentTorso;
	Leg				*currentLeg;
}PlayerData;

/*
* @brief returns the static player entity
* @returns the static player ent
*/
Entity* get_the_player();

/*
* @brief move the player to a new position
* @param newPosition the new position to move to
*/
void move_the_player(GFC_Vector2D newPostion);


/**
* @brief spawn a new player entity
* @param position where to spawn
* @return NULL on error, pointer to the spawn player entity
*/
Entity *player_new_entity(GFC_Vector2D position); 

void player_data_new(PlayerData* data);

/*
* @brief player entity think
* @param self the player entity entity
*/
void player_think(Entity* self);

/*
* @brief player entity update
* @param self the player entity
*/
void player_update(Entity* self);

/*
* @brief incomplete shoot function, shoots a projectile from the player in the direction of the player's mouse
*/
void player_shoot(GFC_Vector2D position, GFC_Vector2D velocity, Entity* self);

/*
* @brief draw specific to the player since they have 4 sprites to draw
*/
void player_draw(Entity* self);

/*
* @brief set the player's head to a head contained with the heads def file
* @param currentHead the player's current head
* @param selectedHead SJson array object of the desired head, usually through get_nth
*/
void player_set_head(Head* currentHead, SJson* selectedHead);

/*
* @brief set the player's arm to a arm contained with the arms def file
* @param currentArm the player's current arm
* @param selectedArm SJson array object of the desired arm, usually through get_nth
*/
void player_set_arm(Arm* currentArm, SJson* selectedArm);

/*
* @brief set the player's torso to a torso contained with the torsos def file
* @param currentTorso the player's current torso
* @param selectedTorso SJson array object of the desired torso, usually through get_nth
*/
void player_set_torso(Torso* currentTorso, SJson* selectedTorso);

/*
* @brief set the player's leg to a leg contained with the legs def file
* @param currentLeg the player's current leg
* @param selectedLeg SJson array object of the desired leg, usually through get_nth
*/
void player_set_leg(Leg* currentLeg, SJson* selectedLeg);


/*
* @brief set the player entity's head to the next available head in its def file
* @param self the player entity
*/
void player_next_head(Entity* self);

/*
* @brief set the player entity's arm to the next available arm in its def file
* @param self the player entity
*/
void player_next_arm(Entity* self);

/*
* @brief set the player entity's torso to the next available torso in its def file
* @param self the player entity
*/
void player_next_torso(Entity* self);

/*
* @brief set the player entity's leg to the next available leg in its def file
* @param self the player entity
*/
void player_next_leg(Entity* self);


/*
* @brief add a part to the player's inventory based on the pickup they touch and what is described in the parts defs
* @param self the player 
* @param partpickupData the PartPickupData void data pointer of the pickup
*/
void player_add_part_to_inventory(Entity* self, PartPickupData* partpickupData);

/*
* @brief update the player's max health based on newly equipped/swapped parts
*/
void player_do_max_health(Entity* self);

/*
* @brief updates the players max health based on equipped parts
*/
void player_output_current_head(Entity* self);

#endif