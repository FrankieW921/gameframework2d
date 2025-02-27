#include "camera.h"

static Camera camera = { 0 };


GFC_Vector2D camera_get_position() {
	return gfc_vector2d(camera.camera.x, camera.camera.y);
}


GFC_Vector2D camera_get_offset() {
	return gfc_vector2d(-camera.camera.x, -camera.camera.y);
}


void camera_center_on(GFC_Vector2D point) {
	camera.camera.x = point.x - (camera.camera.w * 0.5);
	camera.camera.y = point.y - (camera.camera.h * 0.5);
}


void camera_bounds_check();

