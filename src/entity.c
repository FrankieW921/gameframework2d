#include "simple_logger.h"
#include "gfc_types.h"

#include "entity.h"


typedef struct
{
	Uint32 entity_max;
	Entity* entity_list;
}EntitySystem;

static EntitySystem entity_system = { 32, 32 };

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
	entity_system.entity_list = gfc_allocate_array(sizeof(Entity), entity_system.entity_list);
	if (!entity_system.entity_list)
	{
		slog("failed to allocate %i entities", maxEnts);
		return;
	}
	entity_system.entity_max = maxEnts;
	atexit(entity_system_close);
	slog("entity system initialized");
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

void entity_system_draw_all()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++) {
		if (!entity_system.entity_list[i]._inuse)continue;
		entity_draw(&entity_system.entity_list[i]);
	}
}


Entity *entity_new()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++) {
		if (entity_system.entity_list[i]._inuse)continue;
		memset(&entity_system.entity_list[i], 0, sizeof(Entity));
		entity_system.entity_list[i]._inuse = 1;
		slog("entity initialized");
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
}

void entity_draw(Entity* self) 
{
	if (!self) return;
	if (!self->sprite) return;
	gf2d_sprite_draw(
		self->sprite,
		self->position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame);
}

/*eol@eof*/