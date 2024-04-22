#ifndef FWL_H
#define FWL_H

#include <algorithm>
#include <string>
#include <array>
#include <deque>
#include <vector>
#include <ctime>

#include "FW_config.h"

#ifdef USING_FWL
#ifdef NUMBER_OF_LAYER
#ifdef START_MODE

class FWL {
public:
	std::array<std::deque<FUNCTION*>, NUMBER_OF_LAYER> main_cont{};
	std::array<std::deque<FUNCTION*>, NUMBER_OF_LAYER> temp_cont{};

	std::vector<std::string> mode_list;


	#ifdef USING_POPUP_MODE
	#ifdef NUMBER_OF_POPUP_LAYER

	std::array<std::deque<POPUP_FUNCTION*>, NUMBER_OF_POPUP_LAYER> popup_cont{};
	std::array<std::deque<POPUP_FUNCTION*>, NUMBER_OF_POPUP_LAYER> temp_popup_cont{};

	std::vector<std::string> popup_mode_list;
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
			#ifdef NUMBER_OF_POPUP_LAYER

			if (popup_mode_enable) {
				for (int i = 0; i < NUMBER_OF_POPUP_LAYER; ++i) {
					for (auto it = popup_cont[i].begin(); it != popup_cont[i].end();) {
						auto ptr = *it;

						if (ptr != nullptr) {
							if (!framework_changing_mode) {
								ptr->update();
								ptr->check_collision();
							}
							ptr->render();
							ptr->check_delete_flag();

							++it;
						}

						else
							it = popup_cont[i].erase(remove(popup_cont[i].begin(), popup_cont[i].end(), ptr));
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

		MODELIST m;

		mode_list = m.mode_list;

		auto target = std::find(mode_list.begin(), mode_list.end(), modename);
		if (target == mode_list.end())
			return;

		#ifdef USING_POPUP_MODE
		#ifdef NUMBER_OF_POPUP_LAYER

		popup_mode_list = m.popup_mode_list;

		#endif
		#endif


		startmode();
		mode_name = modename;
		framework_enable = true;
	}


	void change_mode(func modefunc, std::string modename) {
		if (mode_name == modename)
			return;

		auto target = std::find(mode_list.begin(), mode_list.end(), modename);
		if (target == mode_list.end())
			return;

		framework_changing_mode = true;
  
		modefunc();

		#ifdef USING_POPUP_MODE
		#ifdef NUMBER_OF_POPUP_LAYER

		if (popup_mode_enable)
			close_popup_mode();

		#endif
		#endif

		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			sweep_layer(i);
			main_cont[i] = temp_cont[i];
			temp_cont[i].clear();
		}

		mode_name = modename;

		framework_changing_mode = false;
	}


	void add_object(FUNCTION*&& object, int layer) {
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
#ifdef NUMBER_OF_POPUP_LAYER

	void init_popup_mode(func modefunc, std::string modename, bool pause_option = false) {
		if (popup_mode_enable)
			return;

		auto target = std::find(popup_mode_list.begin(), popup_mode_list.end(), modename);
		if (target == popup_mode_list.end())
			return;

		modefunc();
		prev_mode_name = mode_name;
		mode_name = modename;

		if (pause_option)
			framework_pause = true;

		popup_mode_enable = true;
	}

	
	void change_popup_mode(func modefunc, std::string modename) {
		if (!popup_mode_enable)
			return;

		auto target = std::find(popup_mode_list.begin(), popup_mode_list.end(), modename);
		if (target == popup_mode_list.end())
			return;

		framework_changing_mode = true;

		modefunc();

		for (int i = 0; i < NUMBER_OF_POPUP_LAYER; ++i) {
			sweep_popup_layer(i);
			popup_cont[i] = temp_popup_cont[i];
			temp_popup_cont[i].clear();
		}

		mode_name = modename;

		framework_changing_mode = false;
	}


	void close_popup_mode() {
		if (!popup_mode_enable)
			return;

		sweep_popup_all();
		mode_name = prev_mode_name;

		framework_pause = false;
		popup_mode_enable = false;
	}


	void add_popup_object(POPUP_FUNCTION*&& object, int layer) {
		if (framework_changing_mode)
			temp_popup_cont[layer].push_back(object);
		else
			popup_cont[layer].push_back(object);
	}


	void delete_popup_object(POPUP_FUNCTION* object, int layer) {
		auto target = std::find(popup_cont[layer].begin(), popup_cont[layer].end(), object);

		if (target != popup_cont[layer].end()) {
			delete* target;
			*target = nullptr;
		}
	}


	POPUP_FUNCTION* connect_popup_ptr(int layer, int index) {
		if (index >= popup_cont[layer].size())
			return nullptr;
		else
			return popup_cont[layer][index];
	}


	size_t popup_layer_size(int layer) {
		return popup_cont[layer].size();
	}


	void sweep_popup_layer(int layer) {
		for (auto it = popup_cont[layer].begin(); it != popup_cont[layer].end();) {
			auto target = std::find(popup_cont[layer].begin(), popup_cont[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}


	void sweep_popup_all() {
		for (int i = 0; i < NUMBER_OF_POPUP_LAYER; ++i) {
			for (auto it = popup_cont[i].begin(); it != popup_cont[i].end();) {
				auto target = std::find(popup_cont[i].begin(), popup_cont[i].end(), *it);

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

#endif
