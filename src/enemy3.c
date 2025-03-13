#include "simple_logger.h"

#include "enemy3.h"
#include "projectile.h"

Entity* enemy3_new_entity(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn enemy");
		return NULL;
	}
	self->think = enemy3_think;
	self->update = enemy3_update;
	gfc_vector2d_copy(self->position, position);
	self->sprite = gf2d_sprite_load_image("images/enemies/enemy3.png");
	self->bounds = gfc_rect(position.x, position.y, 64, 64);
	self->type = ET_Enemy;
	self->health = 1;
	self->attackCooldown = 180;
	self->velocity.y = 1;
	return self;
}

void enemy3_think(Entity* self) {
	if (!self)return;

	if (self->attackCooldown > 0)self->attackCooldown -= 1;

	if (self->attackCooldown <= 0) {
		self->attackCooldown = 180;
		self->velocity.y = -(self->velocity.y);
	}

	self->collideEntities = entity_collide_all(self);
}

void enemy3_update(Entity* self) {
	int i;
	Entity* collider;
	ProjectileData* projData;
	GFC_Vector2D movementVelocity;
	self->position.y += self->velocity.y * 2;
	self->bounds.x = self->position.x;
	self->bounds.y = self->position.y;

	if (self->collideEntities) {
		for (i = 0; i < self->collideEntities->size; i++) {
			collider = gfc_list_get_nth(self->collideEntities, i);
			if (collider) {
				if (collider->type == ET_PlayerProjectile) {
					projData = collider->data;
					if (projData->projectileType == 1) {
						slog("Projectile Damage: %i", projData->damage);
						self->health -= projData->damage;
						slog("Enemy 3 Health: %i", self->health);
						entity_free(collider);
						slog("Enemy hit");
					}
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