#include "MeshView.hpp"

#include "OpenGL.hpp"
#include "Shader.hpp"

#include "../Application.hpp"

MeshView::MeshView()
{
	shader.load("Shaders/MeshView");
	solidShader.load("shaders/SolidView");

	glGenBuffers(1, &posVboId);
	glGenBuffers(1, &heightVboId); 
	glGenBuffers(1, &eboId);
	glGenVertexArrays(1, &vaoId);

	glBindVertexArray(vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, posVboId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(0);

	// Bind the vbo for the vertices' heights, but don't upload anything because
	// the height changes.
	glBindBuffer(GL_ARRAY_BUFFER, heightVboId);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	// Allocate the buffers to maximum size so they aren't allocated/deallocated
	// each frame
	posBuffer		= new glm::vec2		[MAX_TEX_SIZE * MAX_TEX_SIZE];
	indexBuffer		= new unsigned int	[MAX_TEX_SIZE * MAX_TEX_SIZE * 6];
}

void MeshView::renderPoints(std::vector<float> data, int texSize, Camera& camera, glm::mat4& matModel)
{
	shader.use();
	renderCommon(shader, data, texSize, camera, matModel);

	glDrawArrays(GL_POINTS, 0, texSize * texSize);
}

void MeshView::renderWireframe(std::vector<float> data, int texSize, Camera& camera, glm::mat4& matModel)
{
	shader.use();
	renderCommon(shader, data, texSize, camera, matModel);

	int numIndices = genIndices(texSize);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
}

void MeshView::renderSolid(std::vector<float> data, int texSize, Camera& camera, glm::mat4& matModel)
{
	solidShader.use();
	renderCommon(solidShader, data, texSize, camera, matModel);

	int numIndices = genIndices(texSize);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
}

void MeshView::renderCommon(Shader& shader, std::vector<float> data, int texSize, Camera& camera, glm::mat4& matModel)
{
	shader.setMat4("matView", camera.getViewMat());
	shader.setMat4("matProj", camera.getProjMat());
	shader.setMat4("matModel", matModel);

	glBindVertexArray(vaoId);
	genPoints(texSize);

	// Upload the vertex heights to the GPU
	glBindBuffer(GL_ARRAY_BUFFER, heightVboId);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_DRAW);
}

void MeshView::genPoints(int texSize)
{
	// Generate the data for the postions
	int numVerts = texSize * texSize;
	
	#pragma omp parallel for
	for (int y = 0; y < texSize; y++) {
		for (int x = 0; x < texSize; x++) {
			glm::vec2 vPos(x, y);

			// Scale the vertex's position so that all vertices fall in the range
			// [-1, 1],[-1, 1]
			vPos /= (float)(texSize / 2);
			vPos += glm::vec2(-1, -1);

			//Add the vertex
			posBuffer[y * texSize + x] = vPos;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, posVboId);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(glm::vec2), posBuffer, GL_DYNAMIC_DRAW);
}

int MeshView::genIndices(int texSize)
{
	int quadsPerRow = texSize - 1;
	int numIndices	= quadsPerRow * quadsPerRow * 6; //6 indices per quad

	#pragma omp parallel for
	for (int y = 0; y < quadsPerRow; y++) {
		for (int x = 0; x < quadsPerRow; x++) {
			// Indices for the actual vertices in the VBOs
			unsigned int blIdx = y * texSize + x;
			unsigned int brIdx = blIdx + 1;
			unsigned int tlIdx = blIdx + texSize;
			unsigned int trIdx = tlIdx + 1;

			int startIndex = 6 * (y * quadsPerRow + x);
			
			indexBuffer[startIndex + 0] = trIdx;
			indexBuffer[startIndex + 1] = tlIdx;
			indexBuffer[startIndex + 2] = blIdx;

			indexBuffer[startIndex + 3] = trIdx;
			indexBuffer[startIndex + 4] = blIdx;
			indexBuffer[startIndex + 5] = brIdx;
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indexBuffer, GL_DYNAMIC_DRAW);

	return numIndices;
}