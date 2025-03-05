#include "simple_logger.h"

#include "projectile.h"

static SJson* projectileDefFile = NULL;
static SJson* projectilesDefs = NULL;

//TODO take in desired projectile type and assign sprite, velocity, and timetolive
Entity* projectile_new_entity(GFC_Vector2D position, GFC_Vector2D velocity, Uint8 projectileType) {
	Entity* self;
	ProjectileData* data;

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
	gfc_vector2d_copy(self->velocity, velocity); //ADJUST VELOCITY BASED ON PROJECTILE TYPE
	data = gfc_allocate_array(sizeof(ProjectileData), 1);
	if (data) {
		data->timeToLive = 120 * 16; //ADJUST TIME TO LIVE BASED ON PROJECTILE TYPE, i.e. replacing 120 with however many frames you want the proj to live
	}
	self->data = data;
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
	ProjectileData* data;
	data = self->data;
	data->timeToLive -= 1;
}

void projectile_update(Entity* self) {
	ProjectileData* data;
	data = self->data;
	//ADJUST VELOCITY BASED ON PROJECTILE TYPE (entity struct attribute?, entity enum?, switch case?)
	self->position.x += self->velocity.x * 3;
	self->position.y += self->velocity.y * 3;

	if (data->timeToLive <= 0) entity_free(self);
}

