#ifndef FWL_CONFIG_H
#define FWL_CONFIG_H

#include <vector>
#include <string>

#define          USING_FWL
#define          NUMBER_OF_LAYER 7

#define          USING_POPUP_MODE
#define          NUMBER_OF_POPUP_LAYER 7

#define          USING_DEBUG_MESSEGE


class MODELIST {
public:
	// add mode list here...
	std::vector<std::string> mode_list = 
	{ "mode1", "mode2"};

#ifdef USING_POPUP_MODE
#if NUMBER_OF_POPUP_LAYER

	// add popup mode list here...
	std::vector<std::string> popup_mode_list =
	{ "popup1", "popup2"};

#endif
#endif
};

class FUNCTION {
public:
	virtual void update() {}
	virtual void check_collision() {}
	virtual void render() {}
	virtual void check_delete() {}

	// define user function here...

	virtual     ~FUNCTION() {}
};


#ifdef USING_POPUP_MODE
#if NUMBER_OF_POPUP_LAYER

class POPUP_FUNCTION {
public:
	virtual void update() {}
	virtual void check_collision() {}
	virtual void render() {}
	virtual void check_delete() {}

	// define user function here...

	virtual     ~POPUP_FUNCTION() {}
};

#endif
#endif

#endif