#pragma once
#define          USING_FRAMEWORK
#define          NUMBER_OF_LAYER 1
#define          START_MODE "mode_name"


//#define          USING_POPUP_MODE
//#define          NUMBER_OF_LAYER_POPUP 3


class Bridge {
public:
	virtual void update() {}
	virtual void check_collision() {}
	virtual void render() {}
	virtual void check_delete_flag() {}
	
	// define user functions here

	virtual      ~Bridge() {}
};


#ifdef USING_POPUP_MODE
#ifdef NUMBER_OF_LAYER_POPUP

class POP_Bridge {
public:
	virtual void update() {}
	virtual void check_collision() {}
	virtual void render() {}
	virtual void check_delete_flag() {}

	// define user functions here

	virtual     ~POP_Bridge() {} 
};

#endif
#endif