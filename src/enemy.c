#include "simple_logger.h"

#include "enemy.h"
#include "particles.h"
#include "projectile.h"

Entity* enemy_new_entity(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn enemy");
		return NULL;
	}
	self->think = enemy_think;
	self->update = enemy_update;
	gfc_vector2d_copy(self->position, position);
	self->sprite = gf2d_sprite_load_image("images/enemies/enemy1.png");
	self->bounds = gfc_rect(position.x, position.y, 100, 100);
	self->sight = gfc_rect(position.x - 50, position.y - 50, 100*2, 100*2);
	self->type = ET_Enemy;
	self->health = 3;
	return self;
}

void enemy_think(Entity* self) {
	if (!self)return;

	self->collideEntities = entity_collide_all(self);
	self->sightCollideEntities = entity_sight_all(self);
}

void enemy_update(Entity* self) {
	int i;
	Entity *collider;
	ProjectileData *projData;
	GFC_Vector2D movementVelocity;
	self->position.x += self->velocity.x * .50;
	self->position.y += self->velocity.y * .50;
	self->bounds.x = self->position.x;
	self->bounds.y = self->position.y;
	self->sight.x = self->bounds.x - 100 / 2;
	self->sight.y = self->bounds.y - 100 / 2;

	self->velocity.x = 0;
	self->velocity.y = 0;
	
	if (self->collideEntities) {
		for (i = 0; i < self->collideEntities->size; i++) {
			collider = gfc_list_get_nth(self->collideEntities, i);
			if (collider) {
				if (collider->type == ET_PlayerProjectile) {
					projData = collider->data;
					self->health -= projData->damage;
					collider->_inuse = 0;
					particle_spark(self->position, 2, 20);
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