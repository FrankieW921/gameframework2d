#include "simple_logger.h"

#include "camera.h"

static Camera camera = { 0 };


GFC_Vector2D camera_get_position() {
	return gfc_vector2d(camera.camera.x, camera.camera.y);
}


GFC_Vector2D camera_get_offset() {
	return gfc_vector2d(-camera.camera.x, -camera.camera.y);
}

void camera_set_position(GFC_Vector2D position)
{
	camera.camera.x = position.x;
	camera.camera.y = position.y;
}

void camera_set_size(GFC_Vector2D size) {
	camera.camera.w = size.x;
	camera.camera.h = size.y;
	slog("camera w = %f, camera h = %f", size.x, size.y);
}


void camera_set_bounds(GFC_Rect bounds) {
	camera.bounds.x = bounds.x;
	camera.bounds.y = bounds.y;
	camera.bounds.w = bounds.w;
	camera.bounds.h = bounds.h;
	slog("bounds x = %f, bounds y = %f", camera.bounds.x, camera.bounds.y);
	slog("bounds w = %f, bounds h = %f", camera.bounds.w, camera.bounds.h); //this crashes the program because these values are null
}


void camera_center_on(GFC_Vector2D point) {
	camera.camera.x = point.x - (camera.camera.w * 0.5);
	camera.camera.y = point.y - (camera.camera.h * 0.5);
}


void camera_bounds_check() {
	if (camera.camera.x + camera.camera.w > camera.bounds.x + camera.bounds.w)camera.camera.x = (camera.bounds.x + camera.bounds.w) - camera.camera.w;
	if (camera.camera.y + camera.camera.h > camera.bounds.y + camera.bounds.h)camera.camera.y = (camera.bounds.y + camera.bounds.h) - camera.camera.h;
	if (camera.camera.x < camera.bounds.x)camera.camera.x = camera.bounds.x;
	if (camera.camera.y < camera.bounds.y)camera.camera.y = camera.bounds.y;
}

