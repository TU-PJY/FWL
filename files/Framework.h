#pragma once
#include <iostream>
#include <deque>
#include <array>
#include <algorithm>
#include <ctime>

#include "FW_config.h"


#ifdef USING_FRAMEWORK
#ifdef NUMBER_OF_LAYER
#ifdef START_MODE

class Framework {
public:
	double    ft{};
	clock_t   start_time{}, end_time{};
	
	bool  mode_start{};
	using func = void(*)();

	std::array<std::deque<Bridge*>, OPT_LAYER> bridge{};


	void routine() {
		if(mode_start) {
			start_time = clock();
			
			for (int i = 0; i < OPT_LAYER; ++i) {
				for (auto it = bridge[i].begin(); it != bridge[i].end();) {
					auto& ptr = *it;
	
					if (ptr != nullptr) {
						ptr->update();
						ptr->check_collision();
						ptr->render();
						ptr->check_delete_flag();
	
						++it;
					}
	
					else
						it = bridge[i].erase(remove(bridge[i].begin(), bridge[i].end(), ptr));
				}
			}
	
			end_time = clock();
	
			ft = (double)(end_time - start_time) / 1000;
		}
	}


	void add_object(Bridge*&& object, int layer) {
		bridge[layer].push_back(object);
	}


	Bridge* connect_ptr(int layer, int index) {
		if (index >= bridge[layer].size())
			return nullptr;
		else
			return bridge[layer][index];
	}


	int layer_size(int layer) {
		return bridge[layer].size();
	}


	void delete_object(Bridge* object, int layer) {
		auto target = std::find(bridge[layer].begin(), bridge[layer].end(), object);
		
		if (target != bridge[layer].end()) {
			delete* target;
			*target = nullptr;
		}
	}


	void sweep_layer(int layer) {
		for (auto it = bridge[layer].begin(); it != bridge[layer].end();) {
			auto target = std::find(bridge[layer].begin(), bridge[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}


	void sweep_all() {
		for (int i = 0; i < OPT_LAYER; ++i) {
			for (auto it = bridge[i].begin(); it != bridge[i].end();) {
				auto target = std::find(bridge[i].begin(), bridge[i].end(), *it);
				
				delete* target;
				*target = nullptr;

				++it;
			}
		}
	}
	

	void init_start_mode(func modefunc) {
		modefunc();
		mode_start = true;
	}
	

	void change_mode(func modefunc) {
		sweep_all();
	
		framework_enable = false;
		modefunc();
		framework_enable = true;
	}
};

#endif
#endif
#endif
