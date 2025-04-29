#include "simple_json.h"
#include "simple_logger.h"

#include "gf2d_graphics.h"

#include "world.h"
#include "camera.h"
#include "enemy.h"
#include "enemy2.h"
#include "enemy3.h"
#include "enemy4.h"
#include "enemy5.h"
#include "boss_enemy.h"
#include "interactables.h"

static World* world = NULL;
static int spawnIndex = 0;
//void world_tile_layer_build(world);

World* get_current_world() {
	return world;
}

void set_current_world(World* newWorld) {
	world = newWorld;
}

int get_spawn_index() {
	return world;
}

void set_spawn_index(int i) {
	spawnIndex = i;
}

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

int get_world_tile_at(World* world, GFC_Vector2I position) {
	int tile = world->tileMap[position.x + (position.y * (int)world->tileMapSize.x)];
	return tile;
}

World* world_load(const char* filename) {
	//World* world = NULL;
	SJson* json = NULL;
	SJson* wjson = NULL;
	SJson* vertical, * horizontal;
	int w = 0, h = 0;
	GFC_Vector2I dimensions;
	int i, j;
	SJson* item;
	int tile = 0;
	const char* tileSet;
	const char* background;
	int frame_w, frame_h;
	int frames_per_line;
	SJson* enemies;
	SJson* enemy;
	SJson* interactables;
	SJson* interactable;
	SJson* doors;
	int numEnemies, enemyType, numInteractables, interactableType;
	float enemyPosX, enemyPosY, interactablePosX, interactablePosY;

	if (!filename) {
		slog("No file name given for world");
		return NULL;
	}
	json = sj_load(filename);
	if (!json) {
		slog("Failed to load file %s", filename);
		return NULL;
	}
	wjson = sj_object_get_value(json, "world");
	if (!wjson) {
		slog("Failed to load world object from file %s", filename);
		sj_free(json);
		return NULL;
	}
	vertical = sj_object_get_value(wjson, "tileMap");
	if (!vertical) {
		slog("Missing tileMap in %s", filename);
		sj_free(json);
		return NULL;
	}
	h = sj_array_get_count(vertical);
	horizontal = sj_array_get_nth(vertical, 0);
	w = sj_array_get_count(horizontal);

	dimensions.x = w;
	dimensions.y = h;
	world = world_new(dimensions);
	if (!world) {
		slog("failed to create world from %s", filename);
		sj_free(json);
		return NULL;
	}

	//set the tileMap values
	for (j = 0; j < h; j++) {
		horizontal = sj_array_get_nth(vertical, j);
		if (!horizontal) {
			continue;
		}
		for (i = 0; i < w; i++) {
			item = sj_array_get_nth(horizontal, i);
			if (!item)continue;
			sj_get_integer_value(item, &tile);
			world->tileMap[i + (j * w)] = tile;
			//slog("Setting tile %i / %i to %i", i, j, tile);
		}
	}
	background = sj_object_get_value_as_string(wjson, "background");
	world->background = gf2d_sprite_load_image(background);

	tileSet = sj_object_get_value_as_string(wjson, "tileSet");
	sj_object_get_value_as_int(wjson, "frame_w", &frame_w);
	sj_object_get_value_as_int(wjson, "frame_h", &frame_h);
	sj_object_get_value_as_int(wjson, "frames_per_line", &frames_per_line);
	world->tileSet = gf2d_sprite_load_all(
		tileSet,
		frame_w,
		frame_h,
		frames_per_line,
		1
	);
	world_tile_layer_build(world);

	//spawn enemies
	enemies = sj_object_get_value(wjson, "enemies");
	numEnemies = sj_array_get_count(enemies);

	for (i = 0; i < numEnemies; i++) {
		enemy = sj_array_get_nth(enemies, i);
		if (!enemy)continue;
		item = sj_array_get_nth(enemy, 0); //getting enemy type
		sj_get_integer_value(item, &enemyType);
		item = sj_array_get_nth(enemy, 1); //getting x position
		sj_get_float_value(item, &enemyPosX);
		item = sj_array_get_nth(enemy, 2); //getting y position
		sj_get_float_value(item, &enemyPosY);
		//spawn enemy based on types
		switch (enemyType) {
			case 1:
				gfc_list_append(&world->entityList, enemy_new_entity(gfc_vector2d(enemyPosX, enemyPosY))); break;
			case 2:
				gfc_list_append(&world->entityList, enemy2_new_entity(gfc_vector2d(enemyPosX, enemyPosY))); break;
			case 3:
				gfc_list_append(&world->entityList, enemy3_new_entity(gfc_vector2d(enemyPosX, enemyPosY))); break;
			case 4: 
				gfc_list_append(&world->entityList, enemy4_new_entity(gfc_vector2d(enemyPosX, enemyPosY))); break;
			case 5:
				gfc_list_append(&world->entityList, enemy5_new_entity(gfc_vector2d(enemyPosX, enemyPosY))); break;
			case 6:
				gfc_list_append(&world->entityList, boss_new_entity(gfc_vector2d(enemyPosX, enemyPosY))); break;
		}
	}

	//spawn interactables
	interactables = sj_object_get_value(wjson, "interactables");
	numInteractables = sj_array_get_count(interactables);
	for (i = 0; i < numInteractables; i++) {
		interactable = sj_array_get_nth(interactables, i);
		if (!interactable)continue;
		item = sj_array_get_nth(interactable, 0); //getting enemy type
		sj_get_integer_value(item, &interactableType);
		item = sj_array_get_nth(interactable, 1); //getting x position
		sj_get_float_value(item, &interactablePosX);
		item = sj_array_get_nth(interactable, 2); //getting y position
		sj_get_float_value(item, &interactablePosY);
		interactable_new(gfc_vector2d(interactablePosX, interactablePosY), interactableType);
	}

	doors = sj_object_get_value(wjson, "doors");

	sj_free(json);
	return world;
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
	GFC_Vector2D offset;

	if (!world) return; //need a world
	if (!world->tileSet) return; //world needs a tileset to draw
	if (!world->background) return; //world needs a background
	offset = camera_get_offset();
	gf2d_sprite_draw_image(world->background, gfc_vector2d(0, 0)); //draw background image
	gf2d_sprite_draw_image(world->tileLayer, offset); //draw tileLayer image
}

void world_spawn_entity() {

}

void world_setup_camera()
{
	//GFC_Rect bounds = gfc_rect(0, 0, 1280, 720);
	if (!world)return;
	if ((!world->tileLayer) || (!world->tileLayer->surface))
	{
		slog("no tile layer set for world");
		return;
	}
	camera_set_bounds(gfc_rect(0, 0, world->tileLayer->surface->w, world->tileLayer->surface->h));
	camera_bounds_check();
}

int world_collide(World* world, GFC_Shape entity_bounds) { //test if the shape is colliding with tiles
	GFC_Rect tileRect = { 0,0,64,64 };
	GFC_Shape testShape;
	int tileIndex;
	int i, j;

	if ((!world) || (!world->tileSet))return 0;
	tileRect.w = world->tileSet->frame_w;
	tileRect.h = world->tileSet->frame_h;

	for(j = 0; j < world->tileMapSize.y; j++){
		for (i = 0; i < world->tileMapSize.x; i++) {
			tileRect.x = i * tileRect.w;
			tileRect.y = j * tileRect.h;

			tileIndex = get_world_tile_at(world, gfc_vector2i(i, j));
			if (!tileIndex)continue;
			if (tileIndex == 0) return 0; //air
			
			//gf2d_draw_rect(tileRect, GFC_COLOR_BLUE);
			testShape = gfc_shape_from_rect(tileRect);
			if (gfc_shape_overlap(testShape, entity_bounds)) {
				//slog("Colliding with world, tile %i, %i", i, j);
				return 1;
			}	
		}
	}
	return 0;
}