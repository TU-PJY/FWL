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
	std::array<std::deque<Bridge*>, NUMBER_OF_LAYER> bridge{};
	std::array<std::deque<Bridge*>, NUMBER_OF_LAYER> temp_bridge{};


	#ifdef USING_POPUP_MODE
	#ifdef NUMBER_OF_LAYER_POPUP

	std::array<std::deque<POP_Bridge*>, NUMBER_OF_LAYER_POPUP> pop_bridge{};
	bool        popup_mode_enable{};

	#endif
	#endif


	bool        framework_enable{}, framework_pause{};
	std::string mode_name{}, prev_mode_name{};

	double      ft{};
	clock_t     start_time{}, end_time{};

	using func = void(*)();


	void routine() {
		if (framework_enable) {
			start_time = clock();

			for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
				for (auto it = bridge[i].begin(); it != bridge[i].end();) {
					auto ptr = *it;

					if (ptr != nullptr) {
						if (!framework_pause) {
							ptr->update();
							ptr->check_collision();
						}
						ptr->render();
						ptr->check_delete_flag();

						++it;
					}

					else
						it = bridge[i].erase(remove(bridge[i].begin(), bridge[i].end(), ptr));
				}
			}


			#ifdef USING_POPUP_MODE
			#ifdef NUMBER_OF_LAYER_POPUP

			if (popup_mode_enable) {
				for (int i = 0; i < NUMBER_OF_LAYER_POPUP; ++i) {
					for (auto it = pop_bridge[i].begin(); it != pop_bridge[i].end();) {
						auto ptr = *it;

						if (ptr != nullptr) {
							ptr->update();
							ptr->check_collision();
							ptr->render();
							ptr->check_delete_flag();

							++it;
						}

						else
							it = pop_bridge[i].erase(remove(pop_bridge[i].begin(), pop_bridge[i].end(), ptr));
					}
				}
			}

			#endif
			#endif


			end_time = clock();

			ft = (double)(end_time - start_time) / 1000;
		}
	}





	void init_start_mode(func modefunc, std::string modename) {
		if (framework_enable)
			return;

		modefunc();
		mode_name = modename;
		framework_enable = true;
	}


	void add_object(Bridge*&& object, int layer) {
		if (popup_mode_enable)
			return;
	
		if (framework_pause)
			temp_bridge[layer].push_back(object);

		else
			bridge[layer].push_back(object);
	}


	Bridge* connect_ptr(int layer, int index) {
		if (index >= bridge[layer].size())
			return nullptr;
		else
			return bridge[layer][index];
	}


	size_t layer_size(int layer) {
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
		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			for (auto it = bridge[i].begin(); it != bridge[i].end();) {
				auto target = std::find(bridge[i].begin(), bridge[i].end(), *it);

				delete* target;
				*target = nullptr;

				++it;
			}
		}
	}


	void change_mode(func modefunc, std::string modename) {
		if (mode_name == modename)
			return;

		framework_pause = true;
		
		modefunc();
		mode_name = modename;

		sweep_all();

		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			bridge[i] = temp_bridge[i];
			temp_bridge[i] = {};
		}

		framework_pause = false;
	}





#ifdef USING_POPUP_MODE
#ifdef NUMBER_OF_LAYER_POPUP

	void init_popup_mode(func modefunc, std::string modename, bool pause_option = false) {
		if (popup_mode_enable)
			return;

		modefunc();
		prev_mode_name = mode_name;
		mode_name = modename;

		if (pause_option)
			framework_pause = true;

		popup_mode_enable = true;
	}


	void add_popup_object(POP_Bridge*&& object, int layer) {
		pop_bridge[layer].push_back(object);
	}


	void delete_popup_object(POP_Bridge* object, int layer) {
		auto target = std::find(pop_bridge[layer].begin(), pop_bridge[layer].end(), object);

		if (target != pop_bridge[layer].end()) {
			delete* target;
			*target = nullptr;
		}
	}


	size_t popup_layer_size(int layer) {
		return pop_bridge[layer].size();
	}


	POP_Bridge* connect_popup_ptr(int layer, int index) {
		if (index >= pop_bridge[layer].size())
			return nullptr;
		else
			return pop_bridge[layer][index];
	}


	void sweep_popup_layer(int layer) {
		if (!popup_mode_enable)
			return;

		for (auto it = pop_bridge[layer].begin(); it != pop_bridge[layer].end();) {
			auto target = std::find(pop_bridge[layer].begin(), pop_bridge[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}


	void sweep_popup_all() {
		if (!popup_mode_enable)
			return;

		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			for (auto it = pop_bridge[i].begin(); it != pop_bridge[i].end();) {
				auto target = std::find(pop_bridge[i].begin(), pop_bridge[i].end(), *it);

				delete* target;
				*target = nullptr;

				++it;
			}
		}
	}


	void close_popup_mode() {
		if (!popup_mode_enable)
			return;

		sweep_popup_all();
		mode_name = prev_mode_name;

		framework_pause = false;
		popup_mode_enable = false;
	}

#endif
#endif
};

#endif
#endif
#endif
