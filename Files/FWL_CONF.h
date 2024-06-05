// FWL Version 2

#pragma once
#include <string>
#include <vector>

// Frame time option
// Use Frame time system
#define USE_FRAME_TIME

// Use external value as frame time
#define USE_FRAME_TIME_EXTERNAL

// Use debug messege
#define USE_DEBUG_MESSEGE


// Add layer name here, 'END' should be defined at the end
// Main routine layer name
enum class MainLayer 
{L1, L2, L3, END};

// Sub routine layer name
enum class SubLayer  
{L1, L2, L3, END};


// Main routine object class functions
class MAIN_OBJ {
public:
	virtual void Update() {}
	virtual void Collision() {}
	virtual void Render() {}
	virtual std::string Tag() const { return {}; }

	// Define member function of main routine object class here

	virtual     ~MAIN_OBJ() {}
};


// Sub routine object class functions
class SUB_OBJ  {
public:
	virtual void Update() {}
	virtual void Collision() {}
	virtual void Render() {}
	virtual std::string Tag() const { return {}; }

	// Define member function of sub routine object class here

	virtual     ~SUB_OBJ() {}
};