#pragma once
#define          OPT_FRAMEWORK
#define          OPT_LAYER 1
//#define        OPT_OPTIMIZING


class Bridge {
public:
	virtual void update() {}
	virtual void check_collision() {}
	virtual void render() {}
	virtual void check_delete_flag() {}
	
	// user functions here

	virtual      ~Bridge() {}
};
