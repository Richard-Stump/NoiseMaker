#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glm/glm.hpp>

// These two need to be defines because they're used to create
// fixed length arrays
#define MAX_TEX_SIZE	1024
#define MAX_OCTAVES		15

const int DEF_WIN_WIDTH		= 720;
const int DEF_WIN_HEIGHT	= 480;

// Camera Settings
const float FOV_DEG = 90.0f;
const float NEAR	= 0.01f;
const float FAR		= 10.0f;

const glm::vec3 START_EYE	(1.0, 1.0, 1.0);
const glm::vec3 START_POINT	(0.0, 0.0, 0.0);
const glm::vec3 START_UP	(0.0, 1.0, 0.0);

const float MOVE_SPEED = 0.5f;	// Units per second
const float TURN_SPEED = 90.0f;	// Deg per second

// Forward declarations for Main.cpp
bool init(int width, int height);
bool update(int width, int height, float deltaTime);

//Defined in Main.cpp
bool keyDown(int key);

#endif