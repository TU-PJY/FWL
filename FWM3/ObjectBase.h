#pragma once
#include <string>
// include headers you need here

class OBJ_BASE {
public:
	bool ObjectDeleteDescriptor{};
	bool FloatingSpecifiedDescriptor{};
	std::string ObjectTag{};

	virtual void Update(float FT) {}
	virtual void InputControl() {}
	virtual void Render() {}
	virtual void ResetControlState() {}

	virtual ~OBJ_BASE() {}
};