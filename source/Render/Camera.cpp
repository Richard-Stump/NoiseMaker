#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_access.hpp>

void Camera::lookAt(glm::vec3 pos, glm::vec3 point, glm::vec3 up)
{
	_pos = pos;

	_forward = normalize(point - pos);
	_right = normalize(cross(_forward, up));
	_up = normalize(cross(_right, _forward));
}

void Camera::perspective(float fovY, float aspect, float near, float far)
{
	_proj = glm::perspective(fovY, aspect, near, far);
}

// Global Movements
void Camera::rotateX(float rad) { rotate(rad, { 1.0f, 0.0f, 0.0f }); }
void Camera::rotateY(float rad) { rotate(rad, { 0.0f, 1.0f, 0.0f }); }
void Camera::rotateZ(float rad) { rotate(rad, { 0.0f, 0.0f, 1.0f }); }

void Camera::transX(float dist) { translate(dist, { 1.0f, 0.0f, 0.0f }); }
void Camera::transY(float dist) { translate(dist, { 0.0f, 1.0f, 0.0f }); }
void Camera::transZ(float dist) { translate(dist, { 0.0f, 0.0f, 1.0f }); }

//Local Movements
void Camera::tilt(float rad) { rotate(rad, _right); }
void Camera::pan(float rad) { rotate(rad, _up); }
void Camera::roll(float rad) { rotate(rad, _forward); }

void Camera::forward(float dist) { translate(dist, _forward); }
void Camera::right(float dist) { translate(dist, _right); }
void Camera::up(float dist) { translate(dist, _up); }

//Arbitrary Movements
void Camera::rotate(float rad, glm::vec3 axis) 
{
	_up = glm::rotate(_up, rad, axis);
	_forward = glm::rotate(_forward, rad, axis);
	_right = glm::rotate(_right, rad, axis);
}

void Camera::translate(float dist, glm::vec3 dir)
{
	_pos += dist * dir;
}

glm::mat4& Camera::getViewMat()
{
	_view = glm::lookAt(_pos, _pos + _forward, _up);

	return _view;
}