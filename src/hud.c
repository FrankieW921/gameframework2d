#include "simple_logger.h"

#include "hud.h"
#include "camera.h"

Hud* new_hud(Entity* player) {
	TTF_Init();
	Hud* h;
	PlayerData* pData;
	pData = player->data;
	if (!pData)return;

	h = gfc_allocate_array(sizeof(Hud), 1);

	h->playerHealth = pData->currentHealth;
	h->hudBox = gfc_rect(0, 0, 200, 100);
	h->font = TTF_OpenFont("pixelfont.ttf", 32);
	if (!h->font)slog("NO FONT");

	return h;

}

void draw_hud(Hud* h) {
	char healthText[20];
	PlayerData* pData;
	if (!h)return;
	pData = get_the_player()->data;
	if (!pData)return;

	h->playerHealth = pData->currentHealth;

	SDL_Color color = { 255,255,255 };
	healthText[0] = sprintf(healthText, "  HP: %i", h->playerHealth);
	GFC_Vector2D offset = camera_get_offset();
	h->surface = TTF_RenderText_Solid(h->font, healthText, color);
	h->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), h->surface);

	gf2d_sprite_draw(h->texture, gfc_vector2d(0, 0), NULL, NULL, NULL, NULL, NULL, 0);
}

