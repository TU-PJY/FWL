#pragma once
#define          USING_FRAMEWORK
#define          NUMBER_OF_LAYER 1
#define          START_MODE "mode_name"


#define          USING_POPUP_MODE
#define          NUMBER_OF_LAYER_POPUP 3


class FUNCTION {
public:
	virtual void update() {}
	virtual void check_collision() {}
	virtual void render() {}
	virtual void check_delete_flag() {}
	
	// define user functions here

	virtual      ~FUNCTION() {}
};


#ifdef USING_POPUP_MODE
#ifdef NUMBER_OF_LAYER_POPUP

class POP_FUNCTION {
public:
	virtual void update() {}
	virtual void check_collision() {}
	virtual void render() {}
	virtual void check_delete_flag() {}

	// define user functions here

	virtual     ~POP_FUNCTION() {}
};

#endif
#endif