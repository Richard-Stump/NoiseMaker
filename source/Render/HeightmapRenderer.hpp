#ifndef RENDER_HEIGHTMAP_RENDERER_HPP
#define RENDER_HEIGHTMAP_RENDERER_HPP

#include <vector>

#include "Shader.hpp"


class HeightmapRenderer
{
public:
	HeightmapRenderer();
	void render(std::vector<float> data, int width, int height);

protected:
	Shader shader;
	unsigned int vboId, vaoId;
	unsigned int texId;
	const float verts[24] = {
		//x		 y		u		v
		-1.0,	-1.0,	0.0,	0.0,	//BL
		-1.0,	 1.0,	0.0,	1.0,	//TL	
		 1.0,	 1.0,	1.0,	1.0,	//TR

		-1.0,	-1.0,	0.0,	0.0,	//BL
		 1.0,	-1.0,	1.0,	0.0,	//BR
		 1.0,	 1.0,	1.0,	1.0		//TR
	};
};

#endif//RENDER_HEIGHTMAP_RENDERER_HPP