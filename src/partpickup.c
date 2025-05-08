#include "simple_logger.h"
#include "partpickup.h"

Entity* partpickup_new(GFC_Vector2D position, PartPickupType partPickupType, const char* partName, int partDefIndex) {
	Entity* self;
	PartPickupData* data;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn interactable");
		return NULL;
	}

	gfc_vector2d_copy(self->position, position);
	self->type = ET_PartPickup;
	self->bounds = gfc_rect(position.x, position.y, 100, 100);
	data = gfc_allocate_array(sizeof(PartPickupData), 1);
	if (data) {
		data->partPickupType = partPickupType;
		data->partDefIndex = partDefIndex;
		strcpy(data->partName, partName);
		switch (data->partPickupType) {
			case PPT_Head:
				self->sprite = gf2d_sprite_load_image("images/partpickups/newhead.png");
				break;
			case PPT_Arm:
				self->sprite = gf2d_sprite_load_image("images/partpickups/newarm.png");
				break;
			case PPT_Torso:
				self->sprite = gf2d_sprite_load_image("images/partpickups/newtorso.png");
				break;
			case PPT_Leg:
				self->sprite = gf2d_sprite_load_image("images/partpickups/newleg.png");
				break;
		}
	}
	self->data = data;

	return self;
}

void partpickup_free(Entity* self) {
	
	PartPickupData* data;
	if (!self) return;
	data = self->data;
	
	//_strset(data->partName, 0);
	//smemset(data, 0, sizeof(PartPickupData));

	entity_free(self);
}