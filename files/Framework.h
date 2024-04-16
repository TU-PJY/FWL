#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <ctime>

#include "FW_config.h"


#ifdef OPT_FRAMEWORK
#ifdef OPT_LAYER

class Framework {
public:
	double    ft{};
	clock_t   start_time{}, end_time{};

	std::array<std::vector<Bridge*>, OPT_LAYER> bridge{};


	void routine() {
		start_time = clock();
		
		for (int i = 0; i < OPT_LAYER; ++i) {
			
			#ifdef OPT_OPTIMIZING
			if (bridge[i].size() * 2 < bridge[i].capacity())
				bridge[i].shrink_to_fit();
			#endif

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
		for (int i = 0; i < OPT_LAYER; i++) {
			for (auto it = bridge[i].begin(); it != bridge[i].end();) {
				auto target = std::find(bridge[i].begin(), bridge[i].end(), *it);
				
				delete* target;
				*target = nullptr;

				++it;
			}
		}
	}
};

#endif
#endif
