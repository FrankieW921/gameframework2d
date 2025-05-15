#include "simple_logger.h"

#include "game.h"
#include "hud.h"
#include "camera.h"


static Hud* healthHud;
static Hud* partsHud;
static Hud* inventoryHud;
static Hud* partButtonsHud;

static Hud* startButton;
static Hud* editButton;

static Uint8 doDrawPartsHuds;

static Uint8 startMenu = 1;

void init_huds() {
	if (healthHud || partsHud || inventoryHud) return;
	healthHud = new_hud(gfc_vector2d(0,0));
	partsHud = new_hud(gfc_vector2d(0, 550));
	inventoryHud = new_hud(gfc_vector2d(400, 200));
	partButtonsHud = new_part_buttons();

	doDrawPartsHuds = 0;
}

void init_start_buttons() {
	startButton = start_button_new();
	editButton = edit_button_new();
}

void draw_all_huds() {
	draw_health_hud(healthHud);
	draw_current_parts_hud(partsHud);
	draw_inventory_hud(inventoryHud);
	draw_part_buttons(partButtonsHud);

	start_button_update();
	edit_button_update();
}

Hud* new_hud(GFC_Vector2D position) {
	Hud* h;

	h = gfc_allocate_array(sizeof(Hud), 1);
	h->hudBox = gfc_rect(position.x, position.y, 200, 100);
	h->font = TTF_OpenFont("pixelfont.ttf", 32);
	if (!h->font)slog("NO FONT");

	return h;
}

Hud* new_part_buttons() {
	Hud* partButtonsHud;

	partButtonsHud = gfc_allocate_array(sizeof(Hud), 1);
	partButtonsHud->buttonSprite = gf2d_sprite_load_image("images/partButtons.png");

	return partButtonsHud;
}

void draw_part_buttons(Hud* partButtonsHud) {
	if (doDrawPartsHuds == 0) return;
	if (!partButtonsHud) return;
	gf2d_sprite_draw_image(partButtonsHud->buttonSprite, gfc_vector2d(1180, 200));
}

void draw_health_hud(Hud* h) {
	SDL_Color color = { 255,255,255,255 };
	PlayerData* pData;
	if (!h)return;
	pData = get_the_player()->data;
	if (!pData)return;

	strcpy(h->text, "");
	sprintf(h->text, "HP: %i", pData->currentHealth);

	h->surface = TTF_RenderText_Blended_Wrapped(h->font, h->text, color, 512);
	h->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), h->surface);
	SDL_Rect rect = { h->hudBox.x, h->hudBox.y, h->surface->w, h->surface->h };
	SDL_RenderCopy(gf2d_graphics_get_renderer(), h->texture, NULL, &rect);
	SDL_FreeSurface(h->surface);
	SDL_DestroyTexture(h->texture);
}

void draw_current_parts_hud(Hud* h) {
	SDL_Color color = { 255,255,255,255 };
	PlayerData* pData;

	if (doDrawPartsHuds == 0) return;

	if (!h)return;
	strcpy(h->text, "");

	pData = get_the_player()->data;
	if (!pData)return;
	
	strcat(h->text, "Parts Currently Equipped\n  ");
	strcat(h->text, "Head: ");
	strcat(h->text, pData->currentHead->name);
	strcat(h->text, "\n  Arm: ");
	strcat(h->text, pData->currentArm->name);
	strcat(h->text, "\n  Torso: ");
	strcat(h->text, pData->currentTorso->name);
	strcat(h->text, "\n  Leg: ");
	strcat(h->text, pData->currentLeg->name);

	h->surface = TTF_RenderText_Blended_Wrapped(h->font, h->text, color, 512);
	h->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), h->surface);
	SDL_Rect rect = { h->hudBox.x, h->hudBox.y, h->surface->w, h->surface->h};
	SDL_RenderCopy(gf2d_graphics_get_renderer(), h->texture, NULL, &rect);
	SDL_FreeSurface(h->surface);
	SDL_DestroyTexture(h->texture);
}



void draw_inventory_hud(Hud* h) {
	PlayerData* pData;
	Head* head;
	Arm* arm; 
	Torso* torso;
	Leg* leg;
	SDL_Color color = { 255,255,255 };
	int i;

	if (doDrawPartsHuds == 0) return;

	if (!h)return;
	strcpy(h->text, "");

	pData = get_the_player()->data;
	if (!pData)return;

	strcat(h->text, "Parts Held");
	strcat(h->text, "\nHeads: ");
	for (i = 0; i < pData->headInventory->count; i++) {
		if (i > 0) {
			strcat(h->text, ", ");
		}
		head = gfc_list_get_nth(pData->headInventory, i);
		strcat(h->text, head->name);
	}

	strcat(h->text, "\nArms: ");
	for (i = 0; i < pData->armInventory->count; i++) {
		if (i > 0) {
			strcat(h->text, ", ");
		}
		arm = gfc_list_get_nth(pData->armInventory, i);
		strcat(h->text, arm->name);
	}

	strcat(h->text, "\nTorsos: ");
	for (i = 0; i < pData->torsoInventory->count; i++) {
		if (i > 0) {
			strcat(h->text, ", ");
		}
		torso = gfc_list_get_nth(pData->torsoInventory, i);
		strcat(h->text, torso->name);
	}

	strcat(h->text, "\nLegs: ");
	for (i = 0; i < pData->legInventory->count; i++) {
		if (i > 0) {
			strcat(h->text, ", ");
		}
		leg = gfc_list_get_nth(pData->legInventory, i);
		strcat(h->text, leg->name);
	}

	h->surface = TTF_RenderText_Blended_Wrapped(h->font, h->text, color, 512);
	h->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), h->surface);
	SDL_Rect rect = { h->hudBox.x, h->hudBox.y, h->surface->w, h->surface->h };
	SDL_RenderCopy(gf2d_graphics_get_renderer(), h->texture, NULL, &rect);
	SDL_FreeSurface(h->surface);
	SDL_DestroyTexture(h->texture);
}

void enable_do_draw_parts_huds() {
	doDrawPartsHuds = 1;
}

void disable_do_draw_parts_huds() {
	doDrawPartsHuds = 0;
}

Hud* start_button_new() {
	Hud* sb;

	sb = gfc_allocate_array(sizeof(Hud), 1);
	sb->buttonSprite = gf2d_sprite_load_image("images/startButton.png");
	sb->button_update = start_button_update;

	return sb;
}

Hud* edit_button_new() {
	Hud* eb;

	eb = gfc_allocate_array(sizeof(Hud), 1);
	eb->buttonSprite = gf2d_sprite_load_image("images/editButton.png");
	eb->button_update = start_button_update;

	return eb;
}

void enable_start_buttons() {
	startMenu = 1;
}

void disable_start_buttons() {
	startMenu = 0;
}

void start_button_update() {
	Uint32 mouseState;
	int mx, my;
	if (!startButton) return;

	gf2d_sprite_draw_image(startButton->buttonSprite, gfc_vector2d(540, 400));

	mouseState = SDL_GetMouseState(&mx, &my);
	if ((mouseState & 1) && (mx >= 540 && mx <= 740) && (my >= 400 && my <= 500) && (startMenu == 1)) {
		start_game();
	}
}

void edit_button_update() {
	Uint32 mouseState;
	int mx, my;
	if (!editButton) return;

	gf2d_sprite_draw_image(editButton->buttonSprite, gfc_vector2d(540, 550));

	mouseState = SDL_GetMouseState(&mx, &my);
	if ((mouseState & 1) && (mx >= 540 && mx <= 740) && (my >= 550 && my <= 650) && (startMenu == 1)) {
		start_edit();
	}
}

void free_huds() {
	if (!healthHud || !partsHud || !inventoryHud)return;

	strcpy(healthHud->text, "");
	strcpy(partsHud->text, "");
	strcpy(inventoryHud->text, "");

	TTF_CloseFont(healthHud->font);
	TTF_CloseFont(partsHud->font);
	TTF_CloseFont(inventoryHud->font);

	SDL_FreeSurface(healthHud->surface);
	SDL_DestroyTexture(healthHud->texture);
	SDL_FreeSurface(partsHud->surface);
	SDL_DestroyTexture(partsHud->texture);
	SDL_FreeSurface(inventoryHud->surface);
	SDL_DestroyTexture(inventoryHud->texture);

	gf2d_sprite_free(partButtonsHud->buttonSprite);

	memset(healthHud, 0, sizeof(Hud));
	memset(partsHud, 0, sizeof(Hud));
	memset(inventoryHud, 0, sizeof(Hud));
	memset(partButtonsHud, 0, sizeof(Hud));
}

void free_start_buttons() {
	if (!startButton || !editButton) return;

	gf2d_sprite_free(startButton->buttonSprite);
	gf2d_sprite_free(editButton->buttonSprite);

	memset(startButton, 0, sizeof(Hud));
	memset(editButton, 0, sizeof(Hud));
}



