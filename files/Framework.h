#pragma once
#include "Header.h"
#define LAYER 3


class Framework {
public:
	virtual void render() {}
	virtual void check_collision() {}
	virtual void update() {}
};


extern clock_t start_time, end_time;
extern double ft;

void __routine__();
void __add__(Framework*&& object, int layer);
void __delete__(Framework* object, int layer);
void __sweep__();
