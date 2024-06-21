#pragma once
#include <string>
// include headers you need here

class OBJ_BASE {
public:
	bool DeleteFlag{};
	bool PartialExecuteObject{};
	std::string ObjectTag{};

	virtual void Update(float FT) {}
	virtual void InputControl() {}
	virtual void Render() {}

	virtual ~OBJ_BASE() {}
};