#pragma once
#include <algorithm>
#include <string>
#include <array>
#include <deque>
#include <vector>
#include <ctime>

// �����ӿ�ũ ���� ���
#include "FW_config.h"

#ifdef USING_FRAMEWORK
#ifdef NUMBER_OF_LAYER
#ifdef START_MODE

class Framework {
public:
	std::array<std::deque<FUNCTION*>, NUMBER_OF_LAYER> main_cont{};  // NUMBER_OF_LAYER ��ŭ ���̾ ����
	std::array<std::deque<FUNCTION*>, NUMBER_OF_LAYER> temp_cont{};  // ��� ��ȯ �� ����ϴ� �ӽ� �����̳�

	std::vector<std::string> mode_list;


	#ifdef USING_POPUP_MODE
	#ifdef NUMBER_OF_POPUP_LAYER

	std::array<std::deque<POPUP_FUNCTION*>, NUMBER_OF_POPUP_LAYER> popup_cont{};  // NUMBER_OF_POPUP_LAYER ��ŭ ���̾ ����
	std::array<std::deque<POPUP_FUNCTION*>, NUMBER_OF_POPUP_LAYER> temp_popup_cont{};
	
	std::vector<std::string> popup_mode_list;
	bool popup_mode_enable{}; // �˾���� Ȱ��ȭ ����, true�� �� �˾� ��� ����

	#endif
	#endif


	// framework_enable: �����ӿ�ũ ���� Ȱ��ȭ ����, true�� �Ǿ�� routine()�Լ� ����
	// framework_pause: �����ӿ�ũ �Ͻ����� ����, true�� �� �Ͻ�����
	bool framework_enable{}, framework_pause{};

	// ��� ���� ����
	bool framework_changing_mode{}; 

	std::string mode_name{}, prev_mode_name{};  // ���� ���� ���� ��� �̸�, �������� �����ߴ� ��� �̸�

	double    ft{};  // ������ Ÿ��, ��� ������Ʈ ������ �Ǵ� ��ġ ������Ʈ�� ���ؾ� ��
	clock_t   start_time{}, end_time{};  // ��ƾ ���� �ҿ� �ð� ���� ����

	using func = void(*)();   // callable type �Լ� ����


	// ��ü ���� ���� (��: ���, �̵� ��...)
	void routine() {

		// ���� ��� ������ �Ϸ�Ǿ�� ��ƾ ���� ����
		if (framework_enable) {  

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
						// �ݵ�� check_delete_flag()�� ���� ������ ��, �̿��� �Լ����� ���� �� ������ �߻��� �� ����
						ptr->check_delete_flag();

						++it;  // �ݺ��� ����
					}

					// ��ü�� �������� �ʴ´ٸ� �ش� ��ü�� �ִ� �ε����� ����
					else
						it = main_cont[i].erase(remove(main_cont[i].begin(), main_cont[i].end(), ptr));
				}
			}

			#ifdef USING_POPUP_MODE
			#ifdef NUMBER_OF_POPUP_LAYER

			// �˾� ��� Ȱ��ȭ �� �˾� ��� ������Ʈ
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


			// ���� ���� �ҿ�ð� ���� ����
			end_time = clock();

			// ���� ������ ������ Ÿ�� ���
			ft = (double)(end_time - start_time) / 1000;
		}
	}



	// �����ӿ�ũ ����
	void init(func startmode, std::string modename) {
		if (framework_enable)
			return;

		MODELIST m;

		// FW_config�� �Է��� ��� ����� �����ӿ�ũ�� �����Ѵ�
		mode_list = m.mode_list;

		#ifdef USING_POPUP_MODE
		#ifdef NUMBER_OF_POPUP_LAYER

		popup_mode_list = m.popup_mode_list;

		#endif
		#endif

		// ���ڷ� �Է��� ��� �̸��� ��� ��Ͽ� ������� �������� �ʴ´�.
		auto target = std::find(mode_list.begin(), mode_list.end(), modename);
		if (target == mode_list.end())
			return;

		startmode();
		mode_name = modename;

		framework_enable = true;  // ���� ��� ������ �Ϸ�Ǹ� �����ӿ�ũ ��ƾ ����
	}


	// ��� ����
	void change_mode(func modefunc, std::string modename) {
		if (mode_name == modename)
			return;

		// ���ڷ� �Է��� ��� �̸��� ��� ��Ͽ� ������� �������� �ʴ´�.
		auto target = std::find(mode_list.begin(), mode_list.end(), modename);
		if (target == mode_list.end())
			return;


		// �����ӿ�ũ�� ��� ���� ��Ȳ�� �Ǿ� �Ͻ������ȴ�
		framework_changing_mode = true;

		// ��� ���� �Լ��� �����Ѵ�
		modefunc();

		// �˾���尡 Ȱ��ȭ �Ǿ�������� �˾���带 ��Ȱ��ȭ �� ��� ��ȯ�Ѵ�
		#ifdef USING_POPUP_MODE
		#ifdef NUMBER_OF_POPUP_LAYER

		if (popup_mode_enable)
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
		if (framework_changing_mode)
			temp_cont[layer].push_back(object);

		else
			main_cont[layer].push_back(object);
	}


	// ���� ������Ʈ ����
	void delete_object(FUNCTION* object, int layer) {

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
	}


	// Ư�� ������Ʈ�� ���� ������ ���� ����
	// Ư�� ���̾�, Ư�� ��ȣ�� �ε����� �����͸� �����Ѵ�
	// ex) auto ptr = fw.connect_ptr(2, 3);
	FUNCTION* connect_ptr(int layer, int index) {

		// �������� �ʴ� ���̾ ������ ��� nullptr�� �����Ѵ�
		if (index >= main_cont[layer].size())
			return nullptr;
		else
			return main_cont[layer][index];
	}


	// Ư�� ���̾ �����ϴ� ������Ʈ �� ����
	size_t layer_size(int layer) {
		return main_cont[layer].size();
	}


	// Ư�� ���̾��� ��� ���� ������Ʈ ����
	void sweep_layer(int layer) {
		for (auto it = main_cont[layer].begin(); it != main_cont[layer].end();) {
			auto target = std::find(main_cont[layer].begin(), main_cont[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}


	// ��� ���� ������Ʈ ����
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

	// �˾� ��� ����
	// �� ���� �� ���� �˾� ��常 ���� ������
	void init_popup_mode(func modefunc, std::string modename, bool pause_option = false) {
		if (popup_mode_enable)
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

		popup_mode_enable = true;
	}

	// �˾� ��� ��ȯ
	// �˾� ��带 �ٸ� �˾� ���� ��ȯ�Ѵ�
	void change_popup_mode(func modefunc, std::string modename) {
		if (!popup_mode_enable)
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
		if (!popup_mode_enable)
			return;

		// ����� �˾� ����� ��� ��ü ����
		sweep_popup_all();

		mode_name = prev_mode_name;

		framework_pause = false;
		popup_mode_enable = false;
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
};

#endif
#endif
#endif