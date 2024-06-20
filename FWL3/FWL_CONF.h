// FWL Version 3
#pragma once
#include <string>

#define USE_FRAME_TIME_INSIDE
constexpr bool DebugMessage = false;

enum class Layer
{L1, L2, L3, END};

class OBJ {
public:
	bool DeleteFlag{};
	bool StopAtPauseFlag{true};
	std::string ObjectTag{};

	virtual void Update(float FT) {}
	virtual void Render() {}

	virtual ~OBJ() {}
};