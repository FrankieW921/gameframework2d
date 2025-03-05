#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"
#include "projectile.h"

typedef struct {
	const char*			name;
	Uint8			health;
}Head;

typedef struct {
	const char*			name;
	ProjectileType	projectileType;
}Arm;

typedef struct {
	const char*			name;
	Uint8			health;
}Torso;

typedef struct {
	const char*			name;
	Uint8			health;
}Leg;

typedef struct {
	Uint8			maxHealth;
	Uint8			currentHealth;
	Uint8			shootCooldown; //millisecond countdown/cooldown for player firing
	Uint8			cooldownValue; //shootCooldown gets set to this value, this value changes with the selected weapon
	SJson*			heads;
	SJson*			arms;
	SJson*			torsos;
	SJson*			legs;
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
* @brief set player attributes based on head parts in the def files
* @param selectedHead json head to build the head data struct out of
* @returns head struct to set to the player's current head
*/
void player_set_head(Head* currentHead, SJson* selectedHead);

/*
* @brief set player attributes based on arm parts in the def files
* @param selectedArm json arm to build the arm data struct out of
* @returns arm struct to set to the player's current arm
*/
void player_set_arm(Arm* currentArm, SJson* selectedArm);

/*
* @brief set player attributes based on torso parts in the def files
* @param selectedTorso json torso to build the torso data struct out of
* @returns torso struct to set to the player's current head
*/
void player_set_torso(Torso* currentTorso, SJson* selectedTorso);

/*
* @brief set player attributes based on leg parts in the def files
* @param selectedLeg json leg to build the leg data struct out of
* @returns leg struct to set to the player's current leg
*/
void player_set_leg(Leg* currentLeg, SJson* selectedLeg);

/*
* @brief updates the players max health based on equipped parts
*/
void player_output_current_head(Entity* self);

#endif