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

/**
 * @brief initialize particle system
 * @param maxEnts upper limit for particles can exist at once
 */
void particle_system_init(int maxParticles);

/**
 * @brief draw and updates all particles
 */
void particle_draw_all();

/**
 * @brief spawn a sparking particle
 * @param position where to spawn
 * @param defIndex index of the particle type in the def to spawn
 * @param count amount of particles to spawn
 */
void particle_spark(GFC_Vector2D position, int defIndex, int count);

/**
 * @brief cleanup and close particle system
 */
void particle_system_close();

#endif