#include "simple_json.h"
#include "simple_logger.h"

#include "world.h"

World* world_test_new() {
	int i, j;
	int w = 75, h = 45; //hardcode version of the vector2d below

	World* world;
	world = world_new(gfc_vector2d(65.0, 35.0)); 
	if (!world)return NULL;

	world->background = gf2d_sprite_load_image("images/backgrounds/SMTtokyo.png");
	world->tileSet = gf2d_sprite_load_all(
		"images/tilesets/testset.png",
		16,
		16,
		1,
		1
	);

	for (int i = 0; i < w; i++) {
		world->tileMap[i] = 1;
		world->tileMap[i + (((int)world->tileMapSize.y - 1) * (int)world->tileMapSize.x)] = 1;
	}
	for (int i = 0; i < w; i++) {
		world->tileMap[i * (int)world->tileMapSize.x] = 1;
		world->tileMap[i * (int)world->tileMapSize.x + ((int)world->tileMapSize.x - 1)] = 1;
	}

	return world;
}

World* world_new(GFC_Vector2D worldSize) {
	World* world;

	if (!worldSize.x || !worldSize.y) {
		slog("Cannot make a new world, one or both sizes are 0");
		return NULL;
	}

	world = gfc_allocate_array(sizeof(World), 1);

	if (!world) {
		slog("Failed to allocate a new world");
		return NULL;
	}
	world->tileMap = gfc_allocate_array(sizeof(worldSize), (int)worldSize.x * (int)worldSize.y);
	world->tileMapSize.x = worldSize.x;
	world->tileMapSize.y = worldSize.y;

	slog("World initialized");

	return world;
}

void world_free(World* world) {
	if (!world)return;

	gf2d_sprite_free(world->background);
	gf2d_sprite_free(world->tileSet);
	free(world->tileMap);
	free(world);
}

void world_draw(World* world) {
	int i,j,frame,index;
	GFC_Vector2D position;

	if (!world) return; //need a world
	if (!world->tileSet) return; //world needs a tileset to draw
	if (!world->background) return; //world needs a background

	gf2d_sprite_draw_image(world->background, gfc_vector2d(0, 0)); //draw background image

	//map height
	for (int j = 0; j < (int)world->tileMapSize.y; j++) {
		//map width
		for (int i = 0; i < (int)world->tileMapSize.x; i++) {
			index = i + (j * (int)world->tileMapSize.x);
			if (world->tileMap[index] == 0)continue;
			position.x = i * world->tileSet->frame_w;
			position.x = j * world->tileSet->frame_h;
			frame = world->tileMap[index] - 1;

			gf2d_sprite_draw(
				world->tileSet,
				position,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				frame
			);
		}
	}
}

/*eol@eof*/