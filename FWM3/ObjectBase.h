#pragma once
#include <string>

class OBJ_BASE {
public:
	bool DeleteDescriptor{};
	bool FloatingDescriptor{};
	std::string ObjectTag{};

	virtual void Update(float FT) {}
	virtual void Render() {}
};

// dummy object for avoiding iterator error
class FWM_DUMMY : public OBJ_BASE {};