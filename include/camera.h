#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gfc_shape.h"

typedef struct {
	GFC_Rect camera;
	GFC_Rect bounds;
}Camera;


/*
* @brief get the camera's position
* @return the x and y position of the camera
*/
GFC_Vector2D camera_get_position();

/*
* @brief set the camera's position
*/
void camera_set_position(GFC_Vector2D position);

/*
* @brief set the camera's size
*/
void camera_set_size(GFC_Vector2D size);

/*
* @brief set the camera's bounds
*/
void camera_set_bounds(GFC_Vector2D bounds);

/*
* @brief get an offset for the camera
* @return the x and y offset
*/
GFC_Vector2D camera_get_offset();

/*
* @brief center the camera to a specific point
*/
void camera_center_on(GFC_Vector2D point);

/*
* @brief check if the camera can move past a certain obstacle
*/
void camera_bounds_check();


#endif	