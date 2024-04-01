#pragma once
#include "Header.h"
#define LAYER 1


class Framework {
public:
	virtual void render() {}
	virtual void check_collision() {}
	virtual void update() {}
	
	virtual ~Framework() {}
};


extern clock_t start_time, end_time;
extern double ft;

void fw_routine();
void fw_add(Framework*&& object, int layer);
void fw_delete(Framework* object, int layer);
void fw_sweep_layer(int layer);
void fw_sweep();
