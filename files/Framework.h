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
	std::array<std::deque<FUNCTION*>, NUMBER_OF_LAYER> main_cont{};
	std::array<std::deque<FUNCTION*>, NUMBER_OF_LAYER> temp_cont{};


	#ifdef USING_POPUP_MODE
	#ifdef NUMBER_OF_LAYER_POPUP

	std::array<std::deque<POP_FUNCTION*>, NUMBER_OF_LAYER_POPUP> pop_cont{};
	bool        popup_mode_enable{};

	#endif
	#endif


	bool        framework_enable{}, framework_pause{};
	bool        framework_changing_mode{};

	std::string mode_name{}, prev_mode_name{};

	double      ft{};
	clock_t     start_time{}, end_time{};

	using func = void(*)();


	void routine() {
		if (framework_enable) {
			start_time = clock();

			for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
				for (auto it = main_cont[i].begin(); it != main_cont[i].end();) {
					auto ptr = *it;

					if (ptr != nullptr) {
						if (!framework_pause && !framework_changing_mode) {
							ptr->update();
							ptr->check_collision();
						}
						ptr->render();
						ptr->check_delete_flag();

						++it;
					}

					else
						it = main_cont[i].erase(remove(main_cont[i].begin(), main_cont[i].end(), ptr));
				}
			}


			#ifdef USING_POPUP_MODE
			#ifdef NUMBER_OF_LAYER_POPUP

			if (popup_mode_enable) {
				for (int i = 0; i < NUMBER_OF_LAYER_POPUP; ++i) {
					for (auto it = pop_cont[i].begin(); it != pop_cont[i].end();) {
						auto ptr = *it;

						if (ptr != nullptr) {
							ptr->update();
							ptr->check_collision();
							ptr->render();
							ptr->check_delete_flag();

							++it;
						}

						else
							it = pop_cont[i].erase(remove(pop_cont[i].begin(), pop_cont[i].end(), ptr));
					}
				}
			}

			#endif
			#endif


			end_time = clock();

			ft = (double)(end_time - start_time) / 1000;
		}
	}




	void init(func startmode, std::string modename) {
		if (framework_enable)
			return;

		startmode();
		mode_name = modename;
		framework_enable = true;
	}


	void change_mode(func modefunc, std::string modename) {
		if (mode_name == modename)
			return;

		#ifdef USING_POPUP_MODE
		#ifdef NUMBER_OF_LAYER_POPUP

		if (popup_mode_enable)
			close_popup_mode();

		#endif
		#endif


		framework_changing_mode = true;

		modefunc();
		sweep_all();

		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			main_cont[i] = temp_cont[i];
			temp_cont[i].clear();
		}

		mode_name = modename;

		framework_changing_mode = false;
	}


	void add_object(FUNCTION*&& object, int layer) {
		if (popup_mode_enable)
			return;
	
		if (framework_changing_mode)
			temp_cont[layer].push_back(object);

		else
			main_cont[layer].push_back(object);
	}


	void delete_object(FUNCTION* object, int layer) {
		auto target = std::find(main_cont[layer].begin(), main_cont[layer].end(), object);

		if (target != main_cont[layer].end()) {
			delete* target;
			*target = nullptr;
		}
	}


	FUNCTION* connect_ptr(int layer, int index) {
		if (index >= main_cont[layer].size())
			return nullptr;
		else
			return main_cont[layer][index];
	}


	size_t layer_size(int layer) {
		return main_cont[layer].size();
	}


	void sweep_layer(int layer) {
		for (auto it = main_cont[layer].begin(); it != main_cont[layer].end();) {
			auto target = std::find(main_cont[layer].begin(), main_cont[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}


	void sweep_all() {
		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			for (auto it = main_cont[i].begin(); it != main_cont[i].end();) {
				auto target = std::find(main_cont[i].begin(), main_cont[i].end(), *it);

				delete* target;
				*target = nullptr;

				++it;
			}
		}
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


	void close_popup_mode() {
		if (!popup_mode_enable)
			return;

		sweep_popup_all();
		mode_name = prev_mode_name;

		framework_pause = false;
		popup_mode_enable = false;
	}


	void add_popup_object(POP_FUNCTION*&& object, int layer) {
		pop_cont[layer].push_back(object);
	}


	void delete_popup_object(POP_FUNCTION* object, int layer) {
		auto target = std::find(pop_cont[layer].begin(), pop_cont[layer].end(), object);

		if (target != pop_cont[layer].end()) {
			delete* target;
			*target = nullptr;
		}
	}


	POP_FUNCTION* connect_popup_ptr(int layer, int index) {
		if (index >= pop_cont[layer].size())
			return nullptr;
		else
			return pop_cont[layer][index];
	}


	size_t popup_layer_size(int layer) {
		return pop_cont[layer].size();
	}


	void sweep_popup_layer(int layer) {
		if (!popup_mode_enable)
			return;

		for (auto it = pop_cont[layer].begin(); it != pop_cont[layer].end();) {
			auto target = std::find(pop_cont[layer].begin(), pop_cont[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}


	void sweep_popup_all() {
		if (!popup_mode_enable)
			return;

		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			for (auto it = pop_cont[i].begin(); it != pop_cont[i].end();) {
				auto target = std::find(pop_cont[i].begin(), pop_cont[i].end(), *it);

				delete* target;
				*target = nullptr;

				++it;
			}
		}
	}

#endif
#endif
};

#endif
#endif
#endif
