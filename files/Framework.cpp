#include "Framework.h"


std::array<std::vector<Framework*>, LAYER> framework;

clock_t start_time, end_time; 
double ft;


void __routine__() {
	start_time = clock();  	

	for(int i = 0; i < framework.size(); i ++) {
		for (auto it = framework[i].begin(); it != framework[i].end();) {
			auto& ptr = *it;

			if (ptr != nullptr) {
				ptr->render();
				ptr->check_collision();
				ptr->update();	

				++it; 
			}

			else
				it = framework[i].erase(it);
		}
	}

	end_time = clock();  

	ft = (double)(end_time - start_time) / 1000;
}


void __add__(Framework*&& object, int layer) {
	framework[layer].push_back(object);
}


void __delete__(Framework* object, int layer) {
	auto target = std::find(framework[layer].begin(), framework[layer].end(), object);

	if (target != framework[layer].end()) {
		delete* target; 
		*target = nullptr;
	}
}


void __sweep__() {
	for (int i = 0; i < framework.size(); i++) {
		for (auto it = framework[i].begin(); it != framework[i].end();) {
			auto& ptr = *it;
			auto target = std::find(framework[i].begin(), framework[i].end(), ptr);

			delete* target;
			*target = nullptr;

			it++;
		}
	}
}
