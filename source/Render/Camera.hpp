#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <glm/glm.hpp>

/** A class to handle camera movements
 *
 * Global movements move relative to the global coordinate system, and local move the camera
 * relative to it's forward, right, and up vectors.
 * 
 * For rotations, + means ccw, and - means cw
 * For translations, + move in the direction of the axis' vector, - moves opposite
 */

class Camera
{
public:
	void lookAt(glm::vec3 pos, glm::vec3 point, glm::vec3 up);

	void perspective(float fovY, float aspect, float near, float far);

	// Global Movements
	void rotateX(float rad);
	void rotateY(float rad);
	void rotateZ(float rad);
	void transX(float dist);
	void transY(float dist);
	void transZ(float dist);

	// Local Movements
	void tilt(float rad);
	void pan(float rad);
	void roll(float rad);
	void forward(float dist);
	void right(float dist);
	void up(float dist);

	void rotate(float rad, glm::vec3 axis);
	void translate(float dist, glm::vec3 dir);

	glm::vec3 getForward() { return _forward; }
	glm::vec3 getPos() { return _pos; }

	glm::mat4& getProjMat() { return _proj; }
	glm::mat4& getViewMat();

	glm::vec3	_pos, _forward, _right, _up;

private:
	glm::mat4	_proj, _view;
};

#endif//CAMERA_HPP_INCLUDED