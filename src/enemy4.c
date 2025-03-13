#include "simple_logger.h"

#include "enemy4.h"
#include "projectile.h"

Entity* enemy4_new_entity(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn enemy");
		return NULL;
	}
	self->think = enemy4_think;
	self->update = enemy4_update;
	gfc_vector2d_copy(self->position, position);
	self->sprite = gf2d_sprite_load_image("images/enemies/enemy4.png");
	self->bounds = gfc_rect(position.x, position.y, 100, 60);
	self->type = ET_Enemy;
	self->health = 6;
	self->attackCooldown = 120;
	self->velocity = gfc_vector2d(0, 0);
	
	return self;
}

void enemy4_think(Entity* self) {
	if (!self)return;

	if (self->attackCooldown > 0) self->attackCooldown -= 1;

	self->collideEntities = entity_collide_all(self);
}

void enemy4_update(Entity* self) {
	int i;
	Entity *collider;
	ProjectileData *projData;
	GFC_Vector2D movementVelocity;
	self->bounds.x = self->position.x;
	self->bounds.y = self->position.y;
	
	if (self->attackCooldown == 0) {
		self->attackCooldown = 120;
		projectile_new_entity(self->position, gfc_vector2d(0, 1), 3);
	}

	if (self->collideEntities) {
		for (i = 0; i < self->collideEntities->size; i++) {
			collider = gfc_list_get_nth(self->collideEntities, i);
			if (collider) {
				if (collider->type == ET_PlayerProjectile) {
					projData = collider->data;
					self->health -= projData->damage;
					entity_free(collider);
					slog("Enemy hit");
				}
			}
		}
	}

	if (self->sightCollideEntities) {
		for (i = 0; i < self->sightCollideEntities->size; i++) {
			collider = gfc_list_get_nth(self->sightCollideEntities, i);
			if (collider) {
				if (collider->type == ET_Player) {
					movementVelocity = gfc_vector2d(collider->position.x - self->position.x, collider->position.y - self->position.y);
					gfc_vector2d_normalize(&movementVelocity);
					self->velocity.x = movementVelocity.x;
					self->velocity.y = movementVelocity.y;
				}
			}
		}
	}


	gfc_list_clear(self->collideEntities);
	gfc_list_clear(self->sightCollideEntities);

	if (self->health <= 0) {
		entity_free(self);
	}
}