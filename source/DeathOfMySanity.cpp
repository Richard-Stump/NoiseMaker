#include <sstream>
#include <iomanip>
#include <cmath>
	
#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

#include <glm/glm.hpp>

float roundNearst(float val, float mul) {
	return std::round(val / mul) * mul;
}

bool graphWidget(const char* label,
	ImVec2 frame_size,
	int nVals,				// The number of values that that can be edited
	float* vals,			// Pointer ot the array of values
	float min,				// Minimum value
	float max,				// Maximum value
	float snapTo,			// Snap to every multiple of this number
	float snapThresh		// Maximum difference between multiples of snapTo and a value to snap to the multiple
) 
{
	using namespace ImGui;

	ImGui::Text("%s:", label);

	// Get the imgui context 
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return -1;

	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	// Figure out the size of the widget (This was copied from the histogram code I think)
	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	if (frame_size.x == 0.0f)
		frame_size.x = CalcItemWidth();
	if (frame_size.y == 0.0f)
		frame_size.y = label_size.y + (style.FramePadding.y * 2);

	ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);
	ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
	ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0));
	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, 0, &frame_bb))
		return -1;

	// Format the minimum and maximum values as text. Use a string stream because C++ is weird
	std::stringstream ss;
	ss << std::setprecision(1) << min;
	std::string minStr = ss.str();
	ss.str("");
	ss << std::setprecision(1) << " " << max;
	std::string maxStr = ss.str();

	// Draw the labels for the minium and maximum values
	window->DrawList->AddText(
		{ frame_bb.Min.x, frame_bb.Min.y },
		GetColorU32(ImGuiCol_Text, 1.0),
		maxStr.c_str()
	);
	window->DrawList->AddText(
		{ frame_bb.Min.x, frame_bb.Max.y - ImGui::GetTextLineHeight() },
		GetColorU32(ImGuiCol_Text, 1.0),
		minStr.c_str()
	);

	// Resize the frames to account for the text for the min and max values
	frame_bb.Min.x += 20.0f;
	total_bb.Min.x += 20.0f;
	inner_bb.Min.x += 20.0f;
	frame_bb.Max.x += label_size.x;
	total_bb.Max.x += label_size.x;
	inner_bb.Max.x += label_size.x;

	const bool hovered = ItemHoverable(frame_bb, id);

	float halfInnerY = (inner_bb.Max.y - inner_bb.Min.y) / 2;
	float centerY = inner_bb.Min.y + halfInnerY;

	// Render the background 
	RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

	float d_inner_bb = (inner_bb.Max.y - inner_bb.Min.y);
	float d_range = (float)(max - min);

	// Draw a horizontal line for each of the multiples of snapTo between the
	// minimum and maximum.
	for (float y = roundNearst(min, snapTo); y < max; y += snapTo) {
		float t = (max - y) / (max - min);
		float ty = (1 - t) * inner_bb.Max.y + t * inner_bb.Min.y;

		window->DrawList->AddLine(
			{ inner_bb.Min.x, ty },
			{ inner_bb.Max.x, ty },
			y == 0.0 ? 
				GetColorU32(ImGuiCol_FrameBgActive, 1.0f) : 
				GetColorU32(ImGuiCol_FrameBgActive, 0.25f),
			1.0f
		);
	}

	// Draw the lines for the minimum and maximum values
	window->DrawList->AddLine(
		{ inner_bb.Min.x, inner_bb.Min.y },
		{ inner_bb.Max.x, inner_bb.Min.y },
		GetColorU32(ImGuiCol_FrameBgActive, 1.0f),
		1.0
	);
	window->DrawList->AddLine(
		{ inner_bb.Min.x, inner_bb.Max.y },
		{ inner_bb.Max.x, inner_bb.Max.y },
		GetColorU32(ImGuiCol_FrameBgActive, 1.0f),
		1.0
	);

	// x is the current screen x-coordinate, dx is the distance in screen coordinates
	// between each value.
	float x = inner_bb.Min.x;
	float dx = (inner_bb.Max.x - inner_bb.Min.x) / (nVals - 1);

	// Draw the lines connecting each pair of values
	for (int i = 0; i < nVals - 1; i++) {
		// This converts from the minimum and maximum value in the value array to the
		// screen coordinates that the min and max correspond to
		float val1 = glm::clamp(vals[i], min, max);
		float val2 = glm::clamp(vals[i + 1], min, max);

		float tVal1 = glm::mix(0.0, 1.0, (val1 - min) / d_range);
		float tVal2 = glm::mix(0.0, 1.0, (val2 - min) / d_range);

		// Draw the line between the two values
		window->DrawList->AddLine(
			{ x,			inner_bb.Max.y - d_inner_bb * tVal1 },
			{ x + dx,	inner_bb.Max.y - d_inner_bb * tVal2 },
			GetColorU32(ImGuiCol_PlotLines, 1.0f),
			1.0f
		);

		// Draw the vertical line at each value's x coordinate
		window->DrawList->AddLine(
			{ x, inner_bb.Min.y },
			{ x, inner_bb.Max.y },
			GetColorU32(ImGuiCol_FrameBgActive, 0.25f),
			1.0f
		);

		x += dx;
	}
	
	// Special case for when there is a single value. Instead of drawing lines between two
	// values, draw a single, horizontal line that spans the entire widget
	if (nVals == 1) {
		x = inner_bb.Min.x;
		float val1 = glm::clamp(vals[0], min, max);
		float tVal1 = glm::mix(0.0, 1.0, (val1 - min) / d_range);

		window->DrawList->AddLine(
			{ inner_bb.Min.x, inner_bb.Max.y - d_inner_bb * tVal1 },
			{ inner_bb.Max.x,		inner_bb.Max.y - d_inner_bb * tVal1 },
			GetColorU32(ImGuiCol_PlotLines, 1.0f),
			1.0f
		);
	}


	bool leftHeld = IsMouseDown(ImGuiMouseButton_Left);

	bool valChanged = false;

	if (hovered && leftHeld) {
		ImVec2 mPos = GetMousePos();

		// Most of this code is to convert the mouse's x position into the nearest index for the
		// value array. I'm not quite sure how I got it to work, but it does.
		float d_inner_bb_x = (inner_bb.Max.x - inner_bb.Min.x);
		float segSize = d_inner_bb_x / (nVals - 1) / 2;

		int segIdx = (mPos.x - inner_bb.Min.x) / segSize;

		float newVal = (inner_bb.Max.y - mPos.y) / d_inner_bb * d_range + min;

		int closestIdx = 0;

		// Handle the weird cases where clicking certain x-coordinates can cause incorrect 
		if (segIdx == 0)
			closestIdx = 0;
		else if (segIdx < nVals * 2 - 1)
			closestIdx = (segIdx - 1) / 2 + 1;
		else
			closestIdx = nVals - 1;

		// Snap to the nearest multiple of snapTo if the difference between that multiple
		// and the current value is less than the threshold.
		float r = roundNearst(newVal, snapTo);
		if (glm::abs(newVal - r) < snapThresh)
			newVal = r;

		vals[closestIdx] = newVal;

		valChanged = true;
	}


	return valChanged;
}