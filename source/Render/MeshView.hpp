#ifndef RENDER_MESH_VIEW_HPP
#define RENDER_MESH_VIEW_HPP

#include <vector>

#include "OpenGL.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

class MeshView
{
public:
	MeshView();

	void renderPoints(std::vector<float> data, int texSize, Camera& camera, glm::mat4& matModel);
	void renderWireframe(std::vector<float> data, int texSize, Camera& camera, glm::mat4& matModel);
	void renderSolid(std::vector<float> data, int texSize, Camera& camera, glm::mat4& matModel);

protected:
	unsigned int posVboId;		// Array buffer for the 2d coordinates of each sample
	unsigned int heightVboId;	// Array buffer for the height of each sample
	unsigned int eboId;			// Element buffer for the triangles
	unsigned int vaoId;

	// These buffers are allocated once to be maximum size on initialization for the 
	// class as an optimization. Allocating and deallocating them each frame
	// takes too much CPU time.
	glm::vec2*		posBuffer;
	unsigned int*	indexBuffer;


	Shader shader, solidShader;

	void renderCommon(Shader& shader, std::vector<float> data, int texSize, Camera& camera, glm::mat4& matModel);

	void genPoints(int texSize);
	int genIndices(int texSize);
};

#endif//RENDER_MESH_VIEW_HPP