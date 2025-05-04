#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "string.h"
#include "gfc_vector.h"
#include "gfc_shape.h"
#include "gfc_color.h"

typedef struct Particle_S {
	GFC_Rect rect;
	GFC_Vector2D position;
	GFC_Vector2D velocity;
	GFC_Vector2D acceleration;
	GFC_Color color;
	Uint8 ttl;
}Particle;

void particle_system_init(int maxParticles);

void particle_draw_all();

void particle_spark(GFC_Vector2D position, int count);

void particle_system_close();

#endif