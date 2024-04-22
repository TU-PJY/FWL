#ifndef FWL_CONF_H
#defien FWL_CONF_H

#include <string>
#include <vector>

#define          USING_FWL
#define          NUMBER_OF_LAYER 1
#define          START_MODE "mode_name"

#define          USING_POPUP_MODE
#define          NUMBER_OF_POPUP_LAYER 1


class MODELIST {
public:
	// define mode list here
	std::vector<std::string> mode_list =
	{ "mode_name"};

	#ifdef USING_POPUP_MODE
	#ifdef NUMBER_OF_POPUP_LAYER

	// define popup mode list here
	std::vector<std::string> popup_mode_list =
	{ "popup_mode_name" };

	#endif
	#endif
};


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
#ifdef NUMBER_OF_POPUP_LAYER

class POPUP_FUNCTION {
public:
	virtual void update() {}
	virtual void check_collision() {}
	virtual void render() {}
	virtual void check_delete_flag() {}

	// define user functions here

	virtual     ~POPUP_FUNCTION() {}
};

#endif
#endif

#endif
