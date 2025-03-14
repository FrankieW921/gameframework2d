#include "simple_logger.h"
#include "interactables.h"

Entity* interactable_new(GFC_Vector2D position, Uint8 type) {
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn interactable");
		return NULL;
	}

	self->position = position;

	switch (type) {
		case Healing_Field:
			slog("HEALING FIELD TYPE");
			self->type = Healing_Field;
			self->sprite = gf2d_sprite_load_image("images/interactables/healzone.png");
			self->bounds = gfc_rect(self->position.x, self->position.y, 128, 128);
			break;
		case Speed_Gel:
			self->type = Speed_Gel;
			self->sprite = gf2d_sprite_load_image("images/interactables/speedgel.png");
			self->bounds = gfc_rect(self->position.x, self->position.y, 300, 30);
			break;
		case Part_Changer:
			self->type = Part_Changer;
			self->sprite = gf2d_sprite_load_image("images/interactables/changer.png");
			self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
			break;
		case Star_Power:
			self->type = Star_Power;
			self->sprite = gf2d_sprite_load_image("images/interactables/star.png");
			self->bounds = gfc_rect(self->position.x, self->position.y, 64, 64);
			break;
		case Teleporter:
			self->type = Teleporter;
			self->sprite = gf2d_sprite_load_image("images/interactables/teleporter.png");
			self->bounds = gfc_rect(self->position.x, self->position.y, 80, 160);
			break;
	}
	return self;
}