#include <SDL_mixer.h>
#include <gfc_audio.h>
#include "simple_logger.h"

#include "projectile.h"

static SJson* projectileDefFile = NULL;
static SJson* projectilesDefs = NULL;


//TODO take in desired projectile type and assign sprite, velocity, and timetolive
Entity* projectile_new_entity(GFC_Vector2D position, GFC_Vector2D velocity, Uint8 projectileType) {
	Entity* self;
	ProjectileData* data;
	SJson* projectileObject;
	GFC_Sound* projectileSound;

	if (!projectileDefFile) {
		projectileDefFile = sj_load("defs/projectiles.json");
		if (!projectilesDefs) {
			projectilesDefs = sj_object_get_value(projectileDefFile, "projectiles");
		}
	}
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn projectile entity");
		return NULL;
	}

	self->think = projectile_think;
	self->update = projectile_update;
	gfc_vector2d_copy(self->position, position);
	gfc_vector2d_copy(self->velocity, velocity);
	data = gfc_allocate_array(sizeof(ProjectileData), 1);
	if (data) {
		projectileObject = sj_array_get_nth(projectilesDefs, projectileType);
		data->projectileType = projectileType;
		//slog("Projectile Type: %i", data->projectileType);
		sj_object_get_int(projectileObject, "speed", &data->speed);
		//slog("Projectile Speed: %i", data->speed);
		sj_object_get_int(projectileObject, "damage", &data->damage);
		//slog("Projectile Damage: %i", data->damage);
		sj_object_get_int(projectileObject, "timeToLive", &data->timeToLive);

		if (data->projectileType == 0 || data->projectileType == 1 || data->projectileType == 2) {
			self->type = ET_PlayerProjectile;
		}
		else {
			self->type = ET_EnemyProjectile;
		}
	}
	self->data = data;
	self->sprite = gf2d_sprite_load_all(
		"images/projectiles/testprojectile.png",
		16,
		16,
		1,
		1
	);
	self->bounds = gfc_rect(position.x, position.y, 16, 16);

	//play the projectile's sound
	if (data) { //shouts at the cases for null pointers this is just a slapdash solution
		switch (data->projectileType) {
		case 0:
			projectileSound = gfc_sound_load("sounds/PISTOL3.wav", .25, 4);
			gfc_sound_play(projectileSound, 0, .25, -1, -1);
			break;
		case 1:
			projectileSound = gfc_sound_load("sounds/PLASMA1.wav", .25, 4);
			gfc_sound_play(projectileSound, 0, .25, -1, -1);
			break;
		case 2:
			projectileSound = gfc_sound_load("sounds/ROCKET0.wav", .25, 4);
			gfc_sound_play(projectileSound, 0, .25, -1, -1);
			break;
		}
	}
	return self;
}

void projectile_think(Entity* self) {
	ProjectileData* data;
	data = self->data;
	data->timeToLive -= 1;
}

void projectile_update(Entity* self) {
	ProjectileData* data;
	data = self->data;
	if (!data) {
		slog("Data for projectile update does not exist");
	}
	self->position.x += self->velocity.x * data->speed;
	self->position.y += self->velocity.y * data->speed;
	self->bounds.x = self->position.x;
	self->bounds.y = self->position.y;
	if (data->timeToLive <= 0) projectile_free(self);//free projectile itself first
}

void projectile_free(Entity* self) {
	ProjectileData* data;
	if (!self) return;
	data = self->data;

	memset(data, 0, sizeof(ProjectileData)); //all ints no pointers, this is okay
	entity_free(self);
}
