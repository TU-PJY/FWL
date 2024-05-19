#ifndef FWL_CONFIG_H
#define FWL_CONFIG_H

#include <vector>
#include <string>

#define          USING_FWL
#define          N_MAIN_LAYER 1

#define          USING_SUB_MODE
#define          N_SUB_LAYER 1

#define          USING_FRAME_TIME
#define          USING_FRAME_TIME_OUTSIDE



struct MODELIST {
	// add main mode list here...
	std::vector<std::string> MainModeList = 
	{ 
		"main_mode1", 
	
	
	
	};


	// add sub mode list here...
	std::vector<std::string> SubModeList =
	{ 
		"sub_mode1", 
	
	
	
	};
};



class MAIN_CLS {
public:
	virtual void Update() {}
	virtual void CheckCollision() {}
	virtual void Render() {}
	virtual std::string GetTag() const { return {}; }

	// define user function of main mode object class here...

	virtual     ~MAIN_CLS() {}
};


class SUB_CLS {
public:
	virtual void Update() {}
	virtual void CheckCollision() {}
	virtual void Render() {}
	virtual std::string GetTag() const { return {}; }

	// define user function of sub mode object class here...

	virtual     ~SUB_CLS() {}
};


#endif