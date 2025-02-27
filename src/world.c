#include "simple_json.h"
#include "simple_logger.h"

#include "gf2d_graphics.h"

#include "world.h"

World* world_test_new() {
	int i, j;
	int w = 20, h = 12; //hardcode version of the vector2d below, JSONify it 
	GFC_Vector2I dimensions = { 20,12 }; //put them into a vectori so we dont have to do int conversions anymore omg

	World* world;
	world = world_new(dimensions);
	if (!world)return NULL;

	world->background = gf2d_sprite_load_image("images/backgrounds/SMTtokyo.png");
	world->tileSet = gf2d_sprite_load_all(
		"images/tilesets/testset.png",
		64,
		64,
		1,
		1
	);

	for (int i = 0; i < w; i++) { //this works right only when the tile resolution fits the screen resolution
		world->tileMap[i] = 2;
		world->tileMap[i + (((int)world->tileMapSize.y - 1) * (int)world->tileMapSize.x)] = 2;
	}
	for (int i = 0; i < h; i++) {
		world->tileMap[i * (int)world->tileMapSize.x] = 1; //width, i.e. w
		world->tileMap[i * (int)world->tileMapSize.x + ((int)world->tileMapSize.x - 1)] = 1;
	}
	world_tile_layer_build(world);
	return world;
}

void world_tile_layer_build(World* world) {
	Uint32 frame, index;
	int i, j;
	GFC_Vector2D position;

	if (!world)return;
	if (!world->tileSet)return;
	if (world->tileLayer) {
		gf2d_sprite_free(world->tileLayer);
	}
	world->tileLayer = gf2d_sprite_new();

	world->tileLayer->surface = gf2d_graphics_create_surface(
		(int)world->tileMapSize.x * world->tileSet->frame_w,
		(int)world->tileMapSize.y * world->tileSet->frame_h
	);

	world->tileLayer->frame_w = (int)world->tileMapSize.x * world->tileSet->frame_w;
	world->tileLayer->frame_h = (int)world->tileMapSize.y * world->tileSet->frame_h;

	if (!world->tileLayer->surface) {
		slog("failed to create tileLayer surface");
		return;
	}

	//map height
	for (int j = 0; j < (int)world->tileMapSize.y; j++) {
		//map width
		for (int i = 0; i < (int)world->tileMapSize.x; i++) {
			index = i + (j * (int)world->tileMapSize.x);
			if (world->tileMap[index] == 0)continue;

			position.x = i * world->tileSet->frame_w;
			position.y = j * world->tileSet->frame_h;
			frame = world->tileMap[index] - 1;

			gf2d_sprite_draw_to_surface(
				world->tileSet,
				position,
				NULL,
				NULL,
				frame,
				world->tileLayer->surface
			);
		}
	}
	world->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), world->tileLayer->surface);
	if (!world->tileLayer->texture) {
		slog("failed to convert world surface to texture");
		return;
	}
	slog("Tile layer built and set");
}

World* world_new(GFC_Vector2I worldSize) {
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
	gf2d_sprite_free(world->tileLayer);
	free(world->tileMap);
	free(world);
}

void world_draw(World* world) {
	int i, j, frame, index;
	GFC_Vector2D position;

	if (!world) return; //need a world
	if (!world->tileSet) return; //world needs a tileset to draw
	if (!world->background) return; //world needs a background

	gf2d_sprite_draw_image(world->background, gfc_vector2d(0, 0)); //draw background image
	gf2d_sprite_draw_image(world->tileLayer, gfc_vector2d(0, 0)); //draw tileLayer image
}

/*eol@eof*/