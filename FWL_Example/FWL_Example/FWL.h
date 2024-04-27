#ifndef FWL_H
#define FWL_H

#include <algorithm>
#include <iostream>
#include <string>
#include <array>
#include <deque>
#include <vector>
#include <ctime>

// �����ӿ�ũ ���� ���
#include "FWL_config.h"

#ifdef USING_FWL
#if NUMBER_OF_LAYER
#ifdef START_MODE

class FWL {
public:
	std::array<std::deque<FUNCTION*>, NUMBER_OF_LAYER> main_cont{};  // NUMBER_OF_LAYER ��ŭ ���̾ ����
	std::array<std::deque<FUNCTION*>, NUMBER_OF_LAYER> temp_cont{};  // ��� ��ȯ �� ����ϴ� �ӽ� �����̳�

	std::vector<std::string> mode_list;


	#ifdef USING_POPUP_MODE
	#if NUMBER_OF_POPUP_LAYER

	std::array<std::deque<POPUP_FUNCTION*>, NUMBER_OF_POPUP_LAYER> popup_cont{};  // NUMBER_OF_POPUP_LAYER ��ŭ ���̾ ����
	std::array<std::deque<POPUP_FUNCTION*>, NUMBER_OF_POPUP_LAYER> temp_popup_cont{};
	
	std::vector<std::string> popup_mode_list;
	bool popup_mode_initialization{}; // �˾���� Ȱ��ȭ ����, true�� �� �˾� ��� ����

	#endif
	#endif


	bool framework_initialization{};  // �����ӿ�ũ �ʱ�ȭ ����, true�� �� �ʱ�ȭ �Ϸ�

	bool framework_start{}; // �����ӿ�ũ ���� ����
	bool framework_pause{}; // framework_pause: �����ӿ�ũ �Ͻ����� ����, true�� �� �Ͻ�����

	bool framework_changing_mode{}; // ��� ���� ����

	std::string mode_name{}, prev_mode_name{};  // ���� ���� ���� ��� �̸�, �������� �����ߴ� ��� �̸�
	std::string mem_mode_name{};

	double    ft{};  // ������ Ÿ��, ��� ������Ʈ ������ �Ǵ� ��ġ ������Ʈ�� ���ؾ� ��
	clock_t   start_time{}, end_time{};  // ��ƾ ���� �ҿ� �ð� ���� ����

	typedef void (*func)(void); // callable type �Լ� ����


	// ��ü ���� ���� (��: ���, �̵� ��...)
	void routine() {

		// ���� ��� ������ �Ϸ�Ǿ�� ��ƾ ���� ����
		if (framework_initialization) {

			// ���� ���� �ҿ�ð� ���� ����
			start_time = clock();

			// ���� ���̾���� ������� ������Ʈ �ڵ带 ������
			for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
				for (auto it = main_cont[i].begin(); it != main_cont[i].end();) {
					auto ptr = *it;

					// ��ü�� �����ϸ� ������Ʈ �ڵ带 ����
					if (ptr != nullptr) {

						// �����ӿ�ũ �Ͻ����� ��� ���� �߿��� ������Ʈ ����
						if (!framework_pause && !framework_changing_mode) {
							ptr->update();
							ptr->check_collision();
						}
						ptr->render();

						// ���� �÷��� �˻�
						// �ݵ�� check_delete()�� ���� ������ ��, �̿��� �Լ����� ���� �� ������ �߻��� �� ����
						ptr->check_delete();

						++it;  // �ݺ��� ����
					}

					// ��ü�� �������� �ʴ´ٸ� �ش� ��ü�� �ִ� �ε����� ����
					else
						it = main_cont[i].erase(remove(main_cont[i].begin(), main_cont[i].end(), ptr));
				}
			}

			#ifdef USING_POPUP_MODE
			#if NUMBER_OF_POPUP_LAYER

			// �˾� ��� Ȱ��ȭ �� �˾� ��� ������Ʈ
			if (popup_mode_initialization) {
				for (int i = 0; i < NUMBER_OF_POPUP_LAYER; ++i) {
					for (auto it = popup_cont[i].begin(); it != popup_cont[i].end();) {
						auto ptr = *it;

						if (ptr != nullptr) {
							if (!framework_changing_mode) {
								ptr->update();
								ptr->check_collision();
							}
							ptr->render();
							ptr->check_delete();

							++it;
						}

						else
							it = popup_cont[i].erase(remove(popup_cont[i].begin(), popup_cont[i].end(), ptr));
					}
				}
			}

			#endif
			#endif


			// ���� ���� �ҿ�ð� ���� ����
			end_time = clock();

			// ���� ������ ������ Ÿ�� ���
			ft = (double)(end_time - start_time) / 1000;
		}
	}



	// �����ӿ�ũ ����
	void init(func startmode, std::string modename) {
		if (framework_initialization)
			process_err("FWL init error::Reapeted initialization", "");

		framework_start = true;

		MODELIST m;

		// FW_config�� �Է��� ��� ����� �����ӿ�ũ�� �����Ѵ�
		mode_list = m.mode_list;

		#ifdef USING_POPUP_MODE
		#if NUMBER_OF_POPUP_LAYER

		popup_mode_list = m.popup_mode_list;

		#endif
		#endif

		// ���ڷ� �Է��� ��� �̸��� ��� ��Ͽ� ������� �������� �ʴ´�.
		auto target = std::find(mode_list.begin(), mode_list.end(), modename);
		if (target == mode_list.end())
			process_err("FWL init error::Invalid mode", modename);

		mem_mode_name = modename;

		startmode();
		mode_name = modename;

		framework_initialization = true;  // ���� ��� ������ �Ϸ�Ǹ� �����ӿ�ũ ��ƾ ����
	}


	// ��� ����
	void change_mode(func modefunc, std::string modename) {
		if (!framework_initialization)
			process_err("FWL init error::Invalid initialization");

		if (mode_name == modename)
			process_err("FWL mode error::Same mode as current mode");

		// ���ڷ� �Է��� ��� �̸��� ��� ��Ͽ� ������� �������� �ʴ´�.
		auto target = std::find(mode_list.begin(), mode_list.end(), modename);
		if (target == mode_list.end())
			process_err("FWL mode error::Invalid mode", modename);

		mem_mode_name = modename;

		// �����ӿ�ũ�� ��� ���� ��Ȳ�� �Ǿ� �Ͻ������ȴ�
		framework_changing_mode = true;

		// ��� ���� �Լ��� �����Ѵ�
		modefunc();

		// �˾���尡 Ȱ��ȭ �Ǿ�������� �˾���带 ��Ȱ��ȭ �� ��� ��ȯ�Ѵ�
		#ifdef USING_POPUP_MODE
		#if NUMBER_OF_POPUP_LAYER

		if (popup_mode_initialization)
			close_popup_mode();

		#endif
		#endif
		
		// ���� �����̳ʸ� ������ �ӽ� �����̳��� ��ü���� ���� �����̳ʷ� �����ϰ�, �ӽ� �����̳��� �ε����� ����
		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			sweep_layer(i);
			main_cont[i] = temp_cont[i];
			temp_cont[i].clear();
		}

		mode_name = modename;

		// �����ӿ�ũ�� �簳�ȴ�
		framework_changing_mode = false;
	}


	// ���� ������Ʈ �߰�, ��� ��ȯ �� �ӽ� �����̳ʿ� �߰��ȴ�
	void add_object(FUNCTION*&& object, int layer) {
		if (!framework_start)
			process_err("FWL init error::Invalid initialization");

		if (framework_changing_mode) {
			if (layer >= NUMBER_OF_LAYER || layer < 0)
				process_err("FWL object addition error::Layer out of bounds::temp container");

			temp_cont[layer].push_back(object);
		}

		else {
			if (layer >= NUMBER_OF_LAYER || layer < 0) {
				if(framework_initialization)
					process_err("FWL object addition error::Layer out of bounds::main container");
				else
					process_err("FWL init error::Layer out of bounds::main container");
			}

			main_cont[layer].push_back(object);
		}
	}


	// ���� ������Ʈ ����
	void delete_object(FUNCTION* object, int layer) {
		if (!framework_initialization)
			process_err("FWL init error::Invalid initialization");

		if (layer >= NUMBER_OF_LAYER || layer < 0)
			process_err("FWL object deletion error::Layer out of bounds");

		// ���� ������Ʈ�� ������ �����ϴ��� Ȯ��
		auto target = std::find(main_cont[layer].begin(), main_cont[layer].end(), object);

		// ��ü�� �����ϴ� ������ �ǴܵǸ� ���� �ڵ� ����
		if (target != main_cont[layer].end()) {
			// ������Ʈ ����
			delete* target;

			// ������Ʈ�� �� �̻� �������� ����
			// ���� �ε����� routine() �Լ����� ������
			*target = nullptr;
		}

		else 
			process_err("FWL object deletion error::Can not find object");
	}


	// Ư�� ������Ʈ�� ���� ������ ���� ����
	// Ư�� ���̾�, Ư�� ��ȣ�� �ε����� �����͸� �����Ѵ�
	// ex) auto ptr = fw.connect_ptr(2, 3);
	FUNCTION* connect_ptr(int layer, int index) {
		if (!framework_initialization)
			process_err("FWL init error::Invalid initialization");

		if (layer >= NUMBER_OF_LAYER || layer < 0)
			process_err("FWL ptr error::Layer out of bounds");

		// �������� �ʴ� ���̾ ������ ��� nullptr�� �����Ѵ�
		if (index >= main_cont[layer].size())
			return nullptr;
		else
			return main_cont[layer][index];
	}


	// Ư�� ���̾ �����ϴ� ������Ʈ �� ����
	size_t layer_size(int layer) {
		if (!framework_initialization)
			process_err("FWL init error::Invalid initialization");

		if (layer >= NUMBER_OF_LAYER || layer < 0)
			process_err("FWL layer size error::Layer out of bounds");

		return main_cont[layer].size();
	}


	// Ư�� ���̾��� ��� ���� ������Ʈ ����
	void sweep_layer(int layer) {
		if (!framework_initialization)
			process_err("FWL init error::Invalid initialization");

		if (layer >= NUMBER_OF_LAYER || layer < 0)
			process_err("FWL layer sweep error::Layer out of bounds");

		for (auto it = main_cont[layer].begin(); it != main_cont[layer].end();) {
			auto target = std::find(main_cont[layer].begin(), main_cont[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}


	// ��� ���� ������Ʈ ����
	void sweep_all() {
		if (!framework_initialization)
			process_err("FWL init error::Invalid initialization");

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
	#if NUMBER_OF_POPUP_LAYER

	// �˾� ��� ����
	// �� ���� �� ���� �˾� ��常 ���� ������
	void init_popup_mode(func modefunc, std::string modename, bool pause_option = false) {
		if (popup_mode_initialization)
			return;

		// ���ڷ� �Է��� ��� �̸��� ��� ��Ͽ� ������� �������� �ʴ´�.
		auto target = std::find(popup_mode_list.begin(), popup_mode_list.end(), modename);
		if (target == popup_mode_list.end())
			return;

		modefunc();
		prev_mode_name = mode_name;  // �ֱ� ������ ��� �̸��� ����
		mode_name = modename;

		// �����ӿ�ũ �Ͻ����� �ɼ� ���� �� �˾���带 ������ ������ ���� �Ͻ����� ��
		if (pause_option)
			framework_pause = true;

		popup_mode_initialization = true;
	}

	// �˾� ��� ��ȯ
	// �˾� ��带 �ٸ� �˾� ���� ��ȯ�Ѵ�
	void change_popup_mode(func modefunc, std::string modename) {
		if (!popup_mode_initialization)
			return;

		// ��� ����Ʈ���� ���ڷ� ���� �̸��� ã�� ���ϸ� �������� �ʴ´�
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


	// �˾� ��� ������
	void close_popup_mode() {
		if (!popup_mode_initialization)
			return;

		// ����� �˾� ����� ��� ��ü ����
		sweep_popup_all();

		mode_name = prev_mode_name;

		framework_pause = false;
		popup_mode_initialization = false;
	}


	// �˾� ��� ��ü �߰�
	// �˾� ��� ���� �� �ӽ� �����̳ʿ� �߰��ȴ�
	void add_popup_object(POPUP_FUNCTION*&& object, int layer) {
		if (framework_changing_mode)
			temp_popup_cont[layer].push_back(object);
		else
			popup_cont[layer].push_back(object);
	}


	// �˾� ��� ��ü ����
	void delete_popup_object(POPUP_FUNCTION* object, int layer) {

		// ���� ������Ʈ�� ������ �����ϴ��� Ȯ��
		auto target = std::find(popup_cont[layer].begin(), popup_cont[layer].end(), object);

		// ��ü�� �����ϴ� ������ �ǴܵǸ� ���� �ڵ� ����
		if (target != popup_cont[layer].end()) {
			// ������Ʈ ����
			delete* target;

			// ������Ʈ�� �� �̻� �������� ����
			// ���� �ε����� routine() �Լ����� ������
			*target = nullptr;
		}
	}


	// Ư�� �˾� ���̾ �����ϴ� ������Ʈ �� ����
	size_t popup_layer_size(int layer) {
		return popup_cont[layer].size();
	}


	// Ư�� �˾� ���̾ �����ϴ� ��ü�� ������ ����
	POPUP_FUNCTION* connect_popup_ptr(int layer, int index) {

		// �������� �ʴ� ���̾ ������ ��� nullptr�� �����Ѵ�
		if (index >= popup_cont[layer].size())
			return nullptr;
		else
			return popup_cont[layer][index];
	}


	// Ư�� �˾� ���̾� ��ü ����
	void sweep_popup_layer(int layer) {
		for (auto it = popup_cont[layer].begin(); it != popup_cont[layer].end();) {
			auto target = std::find(popup_cont[layer].begin(), popup_cont[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}


	// ��� �˾� ��� ��ü ����
	void sweep_popup_all() {
		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
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


	// �߸��� �����ӿ�ũ ��� ���� ó��
	void process_err(std::string err_type, std::string invalid_mode = "") {
		#ifdef USING_DEBUG_MESSEGE

		std::cout << std::endl << "[ERROR OCCURED]" << std::endl << std::endl;
		std::cout << "Error type: " << err_type << std::endl << std::endl;

		// init error
		if (err_type == "FWL init error::Reapeted init")
			std::cout << "Repeated FWL initialization attempt has been detected." << std::endl << std::endl;

		else if (err_type == "FWL init error::Invalid mode") {
			std::cout << "Invalid mode initialization attempt has been detected." << std::endl << std::endl;
			std::cout << "Invalid mode: '" << invalid_mode << "'" << std::endl;
		}

		else if (err_type == "FWL init error::Layer out of bounds::main container") {
			std::cout << "Addition of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While start mode: '" << mem_mode_name << "'" << std::endl;
		}

		else if (err_type == "FWL init error::Invalid initialization") {
			std::cout << "FWL is not prepared yet." << std::endl;
			std::cout << "Execute FWL's function 'init(func startmode, std::string modename)' first." << std::endl;
		}


		// mode error
		else if (err_type == "FWL mode error::Invalid mode") {
			std::cout << "Invalid mode change attempt has been detected." << std::endl << std::endl;
			std::cout << "While mode change: '" << mode_name << "' to invalid mode '" << invalid_mode << "'" << std::endl << std::endl;
			std::cout << "[List of valid modes]" << std::endl;

			for (auto& s : mode_list)
				std::cout << "" << s << "" << std::endl;
		}

		else if (err_type == "FWL mode error::Same mode as current mode") {
			std::cout << "An attempt to change to the same mode as the current mode has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}


		// object addition error
		else if (err_type == "FWL object addition error::Layer out of bounds::temp container") {
			std::cout << "Addition of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While mode change: '" << mode_name << "' to '" << mem_mode_name << "'" << std::endl;
		}

		else if (err_type == "FWL object addition error::Layer out of bounds::main container") {
			std::cout << "Addition of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}


		// object deletion error
		else if (err_type == "FWL object deletion error::Layer out of bounds") {
			std::cout << "Deletion of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}
		
		else if (err_type == "FWL object deletion error::Can not find object") {
			std::cout << "Deletion of unknown object attempt has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}


		// ptr error
		else if (err_type == "FWL ptr error::Layer out of bounds") {
			std::cout << "Connection of ptr attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}


		// layer size errpr
		else if (err_type == "FWL layer size error::Layer out of bounds") {
			std::cout << "Getting layer size attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}


		// sweep layer error
		else if (err_type == "FWL layer sweep error::Layer out of bounds") {
			std::cout << "Layer sweep attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}

		#endif

		exit(1);
	}

};

#endif
#endif
#endif

#endif