#include "Application.hpp"

#include <vector>
#include <array>
#include <memory>
#include <iomanip>
#include <sstream>
#include <glm/glm.hpp>
#include "imgui/imgui.h"

#include "Render/OpenGL.hpp"
#include "Render/HeightmapRenderer.hpp"
#include "Render/MeshView.hpp"

#include "Functions/Function.hpp"
#include "Render/Camera.hpp"

#include "DeathofMySanity.hpp"

std::vector<std::unique_ptr<Function>> noiseFunctions;
const char* noiseNames;
int currentNoiseFunc = 0;

// Settings that the user can toy around with
int viewMode	= 0;
int seed		= 0;
int numOctaves	= 0;
int divisions	= 128;
float cellSize	= 0.25f;
float scale3d	= 1.0f;
float transY	= 0.0f;
float octaveWeights[MAX_OCTAVES + 1];

std::vector<float>					heightMap;	
std::unique_ptr<HeightmapRenderer>	renderer;
std::unique_ptr<MeshView>			meshView;

Camera camera;

inline float snoise(Function& curFunc, float x, float y)
{
	float curCellSize = cellSize;

	float value = 0.0;

	for (int octave = 0; octave <= numOctaves; octave++) {
		float sample = curFunc.sample(x / curCellSize, y / curCellSize);

		float weight = octaveWeights[octave] / (octave + 1);

		if (octave == 0)
			value = weight * sample;
		else
			value += weight * sample;

		curCellSize *= 0.5f;
	}

	return value;
}

std::vector<float> genHeightmap(int texSize)
{
	std::vector<float> data(texSize * texSize);
	Function& curFunc = *noiseFunctions[currentNoiseFunc];

	int halfSize = texSize / 2;

	float sy = -1.0f;
	float step = 2.0f / (float)texSize;

	for (int y = 0; y < texSize; y++) {
		float sx = -1.0f;
		for (int x = 0; x < texSize; x++) {
			sx += step;

			data[y * texSize + x] = snoise(curFunc, sx, sy);
		}

		sy += step;
	}

	return data;
}


void addNoises()
{
	noiseFunctions.push_back(std::make_unique<WhiteNoise>());
	noiseFunctions.push_back(std::make_unique<Value>());
	noiseFunctions.push_back(std::make_unique<Perlin>());
	//noiseFunctions.push_back(std::make_unique<Simplex>());		// No Loose Ends
	noiseFunctions.push_back(std::make_unique<Worley>());
	noiseFunctions.push_back(std::make_unique<Worley2>());

	noiseNames =
		"White Noise\0"
		"Value\0"
		"Perlin\0"
		//"Simplex\0"
		"Worley\0"
		"Worley F2\0"
		;
}


void drawUi(int cellWidth, int cellGheight)
{
	bool funcChanged = false;
	bool seedChanged = false;

	bool regen = false;

	if (ImGui::Begin("Settings", nullptr, 0)) {
		ImGui::Text("View Mode:");
		ImGui::RadioButton("2D", &viewMode, 0);				ImGui::SameLine();
		ImGui::RadioButton("3D Points", &viewMode, 1);
		ImGui::RadioButton("3D Wireframe", &viewMode, 2);	ImGui::SameLine();
		ImGui::RadioButton("3D Solid", &viewMode, 3);

		ImGui::SliderFloat("3D Scale", &scale3d, 1.0f, 10.0f, "%f", 0);
		ImGui::SliderFloat("3D Trans Y", &transY, -5.0f, 5.0f, "%f", 0);
		ImGui::Separator();

		funcChanged = ImGui::Combo("Function", &currentNoiseFunc, noiseNames, 8);

		seedChanged = ImGui::InputInt("Seed", &seed, 0, 0, 0);

		regen |= ImGui::SliderInt("Octaves", &numOctaves, 0, MAX_OCTAVES, "%i", 0);
		regen |= ImGui::SliderInt("Divisions", &divisions, 1, MAX_TEX_SIZE / 2, "%i", 0);
		regen |= ImGui::SliderFloat("Cell Size", &cellSize, 0.00001f, 8.0f, "%f", 0);

		regen |= graphWidget(
			"Octave Amplitudes", 
			{0.0, 240.0}, 
			numOctaves + 1, 
			octaveWeights,
			-2.0f, 2.0f,
			0.5f,
			0.05f
			);
	}
	ImGui::End();

	regen |= funcChanged | seedChanged;

	// If the funciton has changed the new function needs re-seeded because it
	// won't have the most recent seed
	if (funcChanged || seedChanged)
		noiseFunctions[currentNoiseFunc]->reseed(seed);

	if (regen)
		heightMap = genHeightmap(divisions * 2);
}

bool init(int width, int height)
{
	addNoises();

	renderer	= std::make_unique<HeightmapRenderer>();
	meshView	= std::make_unique<MeshView>();

	// Initialize octive weights to all contribute 100%
	for (float& i : octaveWeights)
		i = 1.0f;

	// Reseed all the functions so they have valid seeds
	for (auto& i : noiseFunctions)
		i->reseed(seed);

	heightMap = genHeightmap(divisions * 2);

	float aspect = (float)width / (float)height;
	camera.perspective(glm::radians(FOV_DEG), aspect, NEAR, FAR);
	camera.lookAt(START_EYE, START_POINT, START_UP);

	return true;
}

bool handleInput(float deltaTime)
{
	float moveDist = MOVE_SPEED * deltaTime;
	float turnDist = glm::radians(TURN_SPEED) * deltaTime;

	if (keyDown(GLFW_KEY_W))			camera.forward(moveDist);
	if (keyDown(GLFW_KEY_S))			camera.forward(-moveDist);
	if (keyDown(GLFW_KEY_D))			camera.right(moveDist);
	if (keyDown(GLFW_KEY_A))			camera.right(-moveDist);
	if (keyDown(GLFW_KEY_LEFT))			camera.rotateY(turnDist);
	if (keyDown(GLFW_KEY_RIGHT))		camera.rotateY(-turnDist);
	if (keyDown(GLFW_KEY_UP))			camera.tilt(turnDist);
	if (keyDown(GLFW_KEY_DOWN))			camera.tilt(-turnDist);
	if (keyDown(GLFW_KEY_LEFT_SHIFT))	camera.up(-moveDist);
	if (keyDown(GLFW_KEY_SPACE))		camera.up(moveDist);

	return true;
}

bool update(int width, int height, float deltaTime)
{
	drawUi(width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	float aspect = (float)width / (float)height;
	camera.perspective(glm::radians(FOV_DEG), aspect, NEAR, FAR);

	int texSize = divisions * 2;

	glm::mat4 matModel(1.0);
	matModel = glm::scale(matModel, glm::vec3(scale3d));
	matModel = glm::translate(matModel, glm::vec3(0, transY, 0));

	switch (viewMode)
	{
	case 0:	
		renderer->render(heightMap, texSize, texSize);		
		break;
	case 1:
		meshView->renderPoints(heightMap, texSize, camera, matModel);	
		break;
	case 2:
		meshView->renderWireframe(heightMap, texSize, camera, matModel);
		break;
	case 3:
		meshView->renderSolid(heightMap, texSize, camera, matModel);
		break;
	}

	return handleInput(deltaTime);
}