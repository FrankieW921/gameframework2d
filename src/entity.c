#include "simple_logger.h"

#include "gf2d_draw.h"

#include "entity.h"
#include "camera.h"
#include "player.h"
#include "world.h"

typedef struct
{
	Uint32 entity_max;
	Entity* entity_list;
}EntitySystem;

static EntitySystem entity_system = { 0 };

void entity_system_close() {
	if (entity_system.entity_list)
	{
		entity_system_free_all();
		free(entity_system.entity_list);
		entity_system.entity_list = NULL;
	}
}

void entity_system_init(Uint32 maxEnts)
{
	if (!maxEnts) {
		slog("Cannot initialize entity system with 0 or undefined amount of entities");
		return;
	}
	entity_system.entity_list = gfc_allocate_array(sizeof(Entity), maxEnts);
	if (!entity_system.entity_list)
	{
		slog("failed to allocate %i entities", maxEnts);
		return;
	}
	entity_system.entity_max = maxEnts;
	atexit(entity_system_close);
	slog("entity system initialized");
}

void entity_system_draw_all()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++) {
		if (!entity_system.entity_list[i]._inuse)continue;
		entity_draw(&entity_system.entity_list[i]);
	}
}

void entity_system_think_all() 
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++) {
		if (!entity_system.entity_list[i]._inuse)continue;
		entity_think(&entity_system.entity_list[i]);
	}
}

void entity_think(Entity *self)
{
	if (!self)return;
	if (self->think)self->think(self);
}

void entity_update(Entity* self)
{
	if (!self)return;
	if (self->update)self->update(self);
	
}

void entity_move(Entity* self) {
	GFC_Shape bounds; //just use the player rect bounds if you can
	GFC_Vector2D position;
	if (!self)return;

	gfc_vector2d_add(position, self->position, self->velocity);
	gfc_vector2d_add(self->velocity, self->velocity, self->acceleration);

	bounds = gfc_shape_from_rect(self->bounds);
	gfc_shape_move(&bounds, self->velocity);
	if (self->type == ET_Player || self->type == ET_Enemy || self->type == ET_Boss) { //enemy and player collide with world rn
		if (!world_collide(get_current_world(), bounds)) {
			gfc_vector2d_copy(self->position, position);
		}
		else {
			//gfc_vector2d_copy(self->position, position);
		}
	}
	else gfc_vector2d_copy(self->position, position);
}

void entity_system_update_all()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++) {
		if (!entity_system.entity_list[i]._inuse)continue;
		entity_update(&entity_system.entity_list[i]);
	}
}


Entity* entity_new()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++) {
		if (entity_system.entity_list[i]._inuse)continue;
		memset(&entity_system.entity_list[i], 0, sizeof(Entity));
		entity_system.entity_list[i]._inuse = 1;
		//slog("entity initialized");
		return &entity_system.entity_list[i];
	}
	return NULL;
}

void entity_free(Entity* self)
{
	if (!self) return;
	if (self->sprite) 
	{
		gf2d_sprite_free(self->sprite);
	}
	//more things to free here
	gfc_list_clear(self->collideEntities);
	gfc_list_clear(self->sightCollideEntities);
	self->_inuse = 0;
	memset(self, 0, sizeof(self));
	//slog("Entity freed");
}

void entity_system_free_all()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++) {
		if (entity_system.entity_list[i]._inuse)
		{
			entity_free(&entity_system.entity_list[i]);
		}
	}
}

void entity_draw(Entity* self) 
{
	GFC_Vector2D position, offset;
	GFC_Rect drawRect, sightRect;
	
	if (!self) return;
	if (!self->sprite) return;
	offset = camera_get_offset();
	gfc_vector2d_add(position, self -> position, offset);
	gf2d_sprite_draw(
		self->sprite,
		position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame);

	drawRect = self->bounds;
	drawRect.x += offset.x;
	drawRect.y += offset.y;

	sightRect = self->sight;
	sightRect.x += offset.x;
	sightRect.y += offset.y;

	gf2d_draw_rect(drawRect, GFC_COLOR_RED);
	gf2d_draw_rect(sightRect, GFC_COLOR_YELLOW);
}

Uint8 entity_collision_check(Entity* self, Entity* other) {
	GFC_Rect bounds1, bounds2;
	if (!self || !other) {
		return 0;
	}
	if (self->type == other->type) { //don't collide if the same type of thing
		return 0;
	}
	if ((self->type == ET_None) || (other->type == ET_None)){ //don't collide if you dont have a type
		return 0;
	}

	gfc_rect_copy(bounds1, self->bounds);
	gfc_rect_copy(bounds2, other->bounds);
	//gfc_vector2d_add(bounds1, bounds1, self->position);
	//gfc_vector2d_add(bounds2, bounds2, other->position);

	return gfc_rect_overlap(bounds1, bounds2);
}

GFC_List* entity_collide_all(Entity* self) {
	int i;
	GFC_List* entities;
	if (!self) return;

	entities = gfc_list_new();

	for (i = 0; i < entity_system.entity_max; ++i) {
		if (!entity_system.entity_list[i]._inuse)continue;
		if (self == &entity_system.entity_list[i])continue;
		if (entity_collision_check(self, &entity_system.entity_list[i])) {
			gfc_list_append(entities, &entity_system.entity_list[i]);
		}
	}
	if (!gfc_list_count(entities)) {
		gfc_list_clear(entities); return NULL;
	}
	return entities;
}

Uint8 entity_sight_check(Entity* self, Entity* other) {
	GFC_Rect sight, otherBounds;
	if (!self || !other) {
		return 0;
	}
	if (self->type == other->type) { //don't collide if the same type of thing
		return 0;
	}
	if ((self->type == ET_None) || (other->type == ET_None)) { //don't collide if you dont have a type
		return 0;
	}

	gfc_rect_copy(sight, self->sight);
	gfc_rect_copy(otherBounds, other->bounds);

	return gfc_rect_overlap(sight, otherBounds);
}

GFC_List* entity_sight_all(Entity* self) {
	int i;
	GFC_List* entities;
	if (!self) return;

	entities = gfc_list_new();

	for (i = 0; i < entity_system.entity_max; ++i) {
		if (!entity_system.entity_list[i]._inuse)continue;
		if (self == &entity_system.entity_list[i])continue;
		if (entity_sight_check(self, &entity_system.entity_list[i])) {
			gfc_list_append(entities, &entity_system.entity_list[i]);
		}
	}
	if (!gfc_list_count(entities)) {
		gfc_list_clear(entities); return NULL;
	}
	return entities;
}

