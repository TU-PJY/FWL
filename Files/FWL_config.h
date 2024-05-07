#ifndef FWL_CONFIG_H
#define FWL_CONFIG_H

#include <vector>
#include <string>

#define          USING_FWL
#define          NUMBER_OF_LAYER 1

#define          USING_POPUP_MODE
#define          NUMBER_OF_POPUP_LAYER 1

#define          USING_DEBUG_MESSEGE
#define          USING_FRAME_TIME
#define          USING_FRAME_TIME_OUTSIDE



class MODELIST {
public:
	// add mode list here...
	std::vector<std::string> mode_list = 
	{ "mode1", };

#ifdef USING_POPUP_MODE
#if NUMBER_OF_POPUP_LAYER

	// add popup mode list here...
	std::vector<std::string> popup_mode_list =
	{ "popup1", };

#endif
#endif
};



class FUNCTION {
public:
	virtual void update() {}
	virtual void check_collision() {}
	virtual void render() {}
	virtual void check_delete() {}

	virtual std::string get_tag() const { return {}; }

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

	virtual std::string get_tag() const { return {}; }

	// define user function here...

	virtual     ~POPUP_FUNCTION() {}
};

#endif
#endif

#endif