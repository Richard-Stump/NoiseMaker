#include "HeightmapRenderer.hpp"

#include <vector>

HeightmapRenderer::HeightmapRenderer()
{
	shader.load("Shaders/Heightmap");

	glGenBuffers(1, &vboId);
	checkGlErrors();
	glGenVertexArrays(1, &vaoId);
	checkGlErrors();

	glBindVertexArray(vaoId);
	checkGlErrors();
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	checkGlErrors();
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	checkGlErrors();
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
	checkGlErrors();
	glEnableVertexAttribArray(0);
	checkGlErrors();
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	checkGlErrors();
	glEnableVertexAttribArray(1);
	checkGlErrors();

	glGenTextures(1, &texId);
	checkGlErrors();
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void HeightmapRenderer::render(std::vector<float> data, int width, int height)
{

	glActiveTexture(GL_TEXTURE0 + 1);
	checkGlErrors();
	glBindTexture(GL_TEXTURE_2D, texId);
	checkGlErrors();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, data.data());
	checkGlErrors();

	glBindVertexArray(vaoId);
	checkGlErrors();	

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader.use();
	shader.setInt("tex", 1);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	checkGlErrors();
}