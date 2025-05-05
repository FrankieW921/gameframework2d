#include "simple_logger.h"
#include "simple_json.h"

#include "gfc_types.h"
#include "gf2d_draw.h"
#include "gfc_config.h"

#include "particles.h"
#include "camera.h"


typedef struct
{
	Uint32 particle_max;
	Particle* particle_list;
}ParticleSystem;

static Uint8 list_index = 0;
static SJson* particle_defs;
static ParticleSystem particle_system = { 0 };


void particle_system_init(int maxParticles) {
	SJson* file;
	if (!maxParticles) {
		slog("Need int to initialize particle system");
		return;
	}
	particle_system.particle_list = gfc_allocate_array(sizeof(Particle), maxParticles);
	if (!particle_system.particle_list) {
		slog("Failed to initialize particle system");
		return;
	}
	particle_system.particle_max = maxParticles;
	atexit(particle_system_close);

	file = sj_load("defs/particles.json");
	particle_defs = sj_object_get_value(file, "particles");

	slog("entity system initialized");
}

Particle* particle_new(int ttl) {
	if (list_index == particle_system.particle_max) list_index = 0;  //optimization? maybe, probably not significant though
	if (particle_system.particle_list[list_index].ttl == 0) {
		memset(&particle_system.particle_list[list_index], 0, sizeof(Particle));
		particle_system.particle_list[list_index].ttl = ttl;	//im using this from the github because I assume
															//this is assurance that we can at least get one update cycle on it
		return &particle_system.particle_list[list_index];
	}
	list_index++;
	return NULL;
}

void particle_update(Particle* particle) {
	if (!particle) return;
	particle->ttl--;

	gfc_vector2d_add(particle->position, particle->position, particle->velocity);
	gfc_vector2d_add(particle->velocity, particle->velocity, particle->acceleration);
	gfc_vector2d_copy(particle->rect, particle->position);
	
	//professor implements color acceleration
}

void particle_draw(Particle* particle) {
	if (!particle) return;
	gf2d_draw_rect_filled(particle->rect, particle->color);
}

void particle_draw_all() {
	int i;
	for (i = 0; i < particle_system.particle_max; i++) {
		if (particle_system.particle_list[i].ttl > 0) {
			particle_update(&particle_system.particle_list[i]);
			//lack of an update all is okay because particles don't interact so they're allowed to update immediately before their draw
			particle_draw(&particle_system.particle_list[i]);
		}
	}
}

void particle_spark(GFC_Vector2D position, int defIndex, int count) {
	SJson* particleDef;
	Particle* p;
	int i;
	GFC_Vector2D cameraOffset;

	int ttl;
	int angleVariance;
	float speed;
	GFC_Color color;
	int size, colorVariance, alphaVariance;

	cameraOffset = camera_get_offset();
	gfc_vector2d_add(position, position, cameraOffset);

	particleDef = sj_array_get_nth(particle_defs, defIndex); //metal spark
	sj_object_get_value_as_int(particleDef, "ttl", &ttl);
	sj_object_get_value_as_int(particleDef, "angleVariance", &angleVariance);
	sj_object_get_value_as_float(particleDef, "speed", &speed);
	sj_object_get_value_as_int(particleDef, "size", &size);
	sj_object_get_color_value(particleDef, "color", &color);
	sj_object_get_value_as_int(particleDef, "colorVariance", &colorVariance);
	sj_object_get_value_as_int(particleDef, "alphaVariance", &alphaVariance);
	slog("color: %f, %f, %f", color.r, color.g, color.b);

	for (i = 0; i < count; i++) {
		p = particle_new(ttl);
		if (!p) {
			slog("particle failed to create");
			return;
		}
		list_index++;

		gfc_vector2d_copy(p->position, position);
		p->rect = gfc_rect(p->position.x, p->position.y, size, size);
		p->color = color;
		p->color.r += gfc_random() * ((float)colorVariance);
		p->color.g += gfc_random() * ((float)colorVariance);
		p->color.b += gfc_random() * ((float)colorVariance);
		p->color.a += gfc_random() * (alphaVariance / 256);
		slog("Random Color: %f, %f, %f", p->color.r, p->color.g, p->color.b);
		//could feed in the mouse direction velocity but might need overload
		p->velocity = gfc_vector2d_rotate(gfc_vector2d(1, 0), (gfc_crandom() * angleVariance) / 90);
		gfc_vector2d_scale(p->velocity, p->velocity, speed);
	}
}

void particle_system_close() {
	if (particle_system.particle_list)
	{
		free(particle_system.particle_list); //all fixed numbers, all good
	}
	memset(&particle_system, 0, sizeof(ParticleSystem));
}

