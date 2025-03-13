#include "simple_logger.h"
#include "gfc_vector.h"

#include "boss_enemy.h"
#include "projectile.h"

Entity* boss_new_entity(GFC_Vector2D position) {
	Entity* self;
	BossData* bData;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn enemy");
		return NULL;
	}
	self->think = boss_think;
	self->update = boss_update;
	gfc_vector2d_copy(self->position, position);
	self->sprite = gf2d_sprite_load_image("images/enemies/boss1.png");
	self->bounds = gfc_rect(position.x, position.y, 128, 128);
	slog("BOSS INIT");
	bData = gfc_allocate_array(sizeof(BossData), 1);
	if (bData) {
		bData->arm1 = enemy2_new_entity(gfc_vector2d(position.x - 60, position.y + 60));
		bData->arm2 = enemy2_new_entity(gfc_vector2d(position.x - 60, position.y - 60));
		gf2d_sprite_free(bData->arm1);
		gf2d_sprite_free(bData->arm1);
		bData->arm1->sprite = gf2d_sprite_load_image("images/enemies/boss1arm.png");
		bData->arm2->sprite = gf2d_sprite_load_image("images/enemies/boss1arm.png");
	}
	self->data = bData;
	self->type = ET_Enemy;
	self->health = 30;

}

void boss_think(Entity* self) {
	if (!self)return;

	self->collideEntities = entity_collide_all(self);
}

void boss_update(Entity* self) {
	int i;
	Entity* collider;
	BossData* bData;
	ProjectileData* pData;

	bData = self->data;
	if (!bData)return NULL;

	if (self->collideEntities) {
		if (bData->arm1->health <= 0 && bData->arm2->health <= 0) {
			for (i = 0; i < self->collideEntities->size; i++) {
				collider = gfc_list_get_nth(self->collideEntities, i);
				if (collider) {
					if (collider->type == ET_PlayerProjectile) {
						pData = collider->data;
						self->health -= pData->damage;
						entity_free(collider);
					}
				}
			}
		}
	}

	gfc_list_clear(self->collideEntities);
	if (self->health <= 0) {
		entity_free(self);
	}
}