#ifndef DOMS_HPP
#define DOMS_HPP

#include "imgui/imgui.h"

bool graphWidget(const char* label,
	ImVec2 frame_size,
	int nVals,
	float* vals,
	float min = -1.0f,
	float max = 1.0f,
	float snapTo = 1.0,
	float snapThresh = 0.1f
);

#endif
