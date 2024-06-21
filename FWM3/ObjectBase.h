#pragma once
#include <string>
// include headers you need here

class OBJ_BASE {
public:
	bool DeleteFlag{};
	bool StopAtPauseFlag{ true };
	std::string ObjectTag{};

	virtual void Update(float FT) {}
	virtual void Render() {}

	virtual ~OBJ_BASE() {}
};