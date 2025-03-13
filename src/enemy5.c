#include "simple_logger.h"

#include "enemy5.h"
#include "projectile.h"

Entity* enemy5_new_entity(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn enemy");
		return NULL;
	}
	self->think = enemy5_think;
	self->update = enemy5_update;
	gfc_vector2d_copy(self->position, position);
	self->sprite = gf2d_sprite_load_image("images/enemies/enemy5.png");
	self->bounds = gfc_rect(position.x, position.y, 100, 60);
	self->sight = gfc_rect(position.x - 150, position.y - 170, 400, 400);
	self->type = ET_Enemy;
	self->health = 10;
	self->attackCooldown = 0;
	return self;
}

void enemy5_think(Entity* self) {
	if (!self)return;

	if (self->attackCooldown > 0)self->attackCooldown -= 1;

	self->collideEntities = entity_collide_all(self);
	self->sightCollideEntities = entity_sight_all(self);
}

void enemy5_update(Entity* self) {
	int i;
	Entity *collider;
	ProjectileData *projData;
	GFC_Vector2D movementVelocity;
	GFC_Vector2D shootVelocity;
	self->position.x += self->velocity.x * .50;
	self->position.y += self->velocity.y * .50;
	self->bounds.x = self->position.x;
	self->bounds.y = self->position.y;
	self->sight.x = self->bounds.x - 150;
	self->sight.y = self->bounds.y - 170;

	self->velocity.x = 0;
	self->velocity.y = 0;
	
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
					if (self->attackCooldown == 0) {
						self->attackCooldown = 120;
						shootVelocity = gfc_vector2d(collider->position.x - self->position.x, collider->position.y - self->position.y); //projectile velocity
						gfc_vector2d_normalize(&shootVelocity);
						projectile_new_entity(self->position, shootVelocity, 3);
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