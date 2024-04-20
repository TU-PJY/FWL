#pragma once
#include <iostream>
#include <deque>
#include <array>
#include <algorithm>
#include <ctime>

// �����ӿ�ũ ���� ���
#include "FW_config.h"

#ifdef USING_FRAMEWORK
#ifdef NUMBER_OF_LAYER
#ifdef START_MODE

class Framework {
public:
	std::array<std::deque<Bridge*>, NUMBER_OF_LAYER> bridge{};  // NUMBER_OF_LAYER ��ŭ ���̾ ����
	std::array<std::deque<Bridge*>, NUMBER_OF_LAYER> temp_bridge{};  // ��� ��ȯ �� ����ϴ� �ӽ� �����̳�


	#ifdef USING_POPUP_MODE
	#ifdef NUMBER_OF_LAYER_POPUP

	std::array<std::deque<POP_Bridge*>, NUMBER_OF_LAYER_POPUP> pop_bridge{};  // NUMBER_OF_LAYER_POPUP ��ŭ ���̾ ����
	bool popup_mode_enable{}; // �˾���� Ȱ��ȭ ����, true�� �� �˾� ��� ����

	#endif
	#endif


	// framework_enable: �����ӿ�ũ ���� Ȱ��ȭ ����, true�� �Ǿ�� routine()�Լ� ����
	// framework_pause: �����ӿ�ũ �Ͻ����� ����, true�� �� �Ͻ�����
	bool framework_enable{}, framework_pause{};
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
				for (auto it = bridge[i].begin(); it != bridge[i].end();) {
					auto ptr = *it;

					// ��ü�� �����ϸ� ������Ʈ �ڵ带 ����
					if (ptr != nullptr) {

						// �����ӿ�ũ �Ͻ����� �� ������Ʈ ����
						if (!framework_pause) {
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
						it = bridge[i].erase(remove(bridge[i].begin(), bridge[i].end(), ptr));
				}
			}

			#ifdef USING_POPUP_MODE
			#ifdef NUMBER_OF_LAYER_POPUP

			// �˾� ��� Ȱ��ȭ �� �˾� ��� ������Ʈ
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


			// ���� ���� �ҿ�ð� ���� ����
			end_time = clock();

			// ���� ������ ������ Ÿ�� ���
			ft = (double)(end_time - start_time) / 1000;
		}
	}



	// ���� ��� ����
	void init_start_mode(func modefunc, std::string modename) {
		if (framework_enable)
			return;

		modefunc();

		mode_name = modename;
		framework_enable = true;  // ���� ��� ������ �Ϸ�Ǹ� �����ӿ�ũ ��ƾ ����
	}


	// ���� ������Ʈ �߰�, ��� ��ȯ �� �ӽ� �����̳ʿ� �߰��ȴ�
	void add_object(Bridge*&& object, int layer) {
		if (popup_mode_enable)
			return;

		if (framework_pause)
			temp_bridge[layer].push_back(object);

		else
			bridge[layer].push_back(object);
	}


	// Ư�� ������Ʈ�� ���� ������ ���� ����
	// Ư�� ���̾�, Ư�� ��ȣ�� �ε����� �����͸� �����Ѵ�
	// ex) auto ptr = fw.connect_ptr(2, 3);
	Bridge* connect_ptr(int layer, int index) {

		// �������� �ʴ� ���̾ ������ ��� nullptr�� �����Ѵ�
		if (index >= bridge[layer].size())
			return nullptr;
		else
			return bridge[layer][index];
	}


	// Ư�� ���̾ �����ϴ� ������Ʈ �� ����
	size_t layer_size(int layer) {
		return bridge[layer].size();
	}


	// ���� ������Ʈ ����
	void delete_object(Bridge* object, int layer) {

		// ���� ������Ʈ�� ������ �����ϴ��� Ȯ��
		auto target = std::find(bridge[layer].begin(), bridge[layer].end(), object);

		// ��ü�� �����ϴ� ������ �ǴܵǸ� ���� �ڵ� ����
		if (target != bridge[layer].end()) {
			// ������Ʈ ����
			delete* target;

			// ������Ʈ�� �� �̻� �������� ����
			// ���� �ε����� routine() �Լ����� ������
			*target = nullptr;
		}
	}


	// Ư�� ���̾��� ��� ���� ������Ʈ ����
	void sweep_layer(int layer) {
		for (auto it = bridge[layer].begin(); it != bridge[layer].end();) {
			auto target = std::find(bridge[layer].begin(), bridge[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}


	// ��� ���� ������Ʈ ����
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


	// ��� ����
	void change_mode(func modefunc, std::string modename) {
		if (mode_name == modename)
			return;

		// �����ӿ�ũ�� �Ͻ������ȴ�
		framework_pause = true;

		// ��� ���� �Լ��� �����Ѵ�
		modefunc();

		// �ӽ� �����̳ʿ� ��ü �߰� �� ��Ÿ �۾��� �Ϸ�Ǹ� ���� �����̳��� ��ü���� ��� �����Ѵ�
		sweep_all();

		// �ӽ� �����̳��� ��ü���� ���� �����̳ʷ� �����ϰ�, �ӽ� �����̳��� �ε����� ����
		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			bridge[i] = temp_bridge[i];
			temp_bridge[i] = {};
		}

		mode_name = modename;

		// �����ӿ�ũ�� �簳�ȴ�
		framework_pause = false;
	}





	#ifdef USING_POPUP_MODE
	#ifdef NUMBER_OF_LAYER_POPUP

	// �˾� ��� ����, �ݵ�� �˾� ��带 ���� ������ ���̾ �־�� ��
	// �� ���� �˾� ��常 ���� ������
	void init_popup_mode(func modefunc, std::string modename, bool pause_option = false) {
		if (popup_mode_enable)
			return;

		modefunc();
		prev_mode_name = mode_name;  // �ֱ� ������ ��� �̸��� ����
		mode_name = modename;

		// �����ӿ�ũ �Ͻ����� �ɼ� ���� �� �˾���带 ������ ������ ���� �Ͻ����� ��
		if (pause_option)
			framework_pause = true;

		popup_mode_enable = true;
	}


	// �˾� ��� ��ü �߰�
	void add_popup_object(POP_Bridge*&& object, int layer) {
		pop_bridge[layer].push_back(object);
	}


	// �˾� ��� ��ü ����
	void delete_popup_object(POP_Bridge* object, int layer) {

		// ���� ������Ʈ�� ������ �����ϴ��� Ȯ��
		auto target = std::find(pop_bridge[layer].begin(), pop_bridge[layer].end(), object);

		// ��ü�� �����ϴ� ������ �ǴܵǸ� ���� �ڵ� ����
		if (target != pop_bridge[layer].end()) {
			// ������Ʈ ����
			delete* target;

			// ������Ʈ�� �� �̻� �������� ����
			// ���� �ε����� routine() �Լ����� ������
			*target = nullptr;
		}
	}


	// Ư�� �˾� ���̾ �����ϴ� ������Ʈ �� ����
	size_t popup_layer_size(int layer) {
		return pop_bridge[layer].size();
	}


	// Ư�� �˾� ���̾ �����ϴ� ��ü�� ������ ����
	POP_Bridge* connect_popup_ptr(int layer, int index) {

		// �������� �ʴ� ���̾ ������ ��� nullptr�� �����Ѵ�
		if (index >= pop_bridge[layer].size())
			return nullptr;
		else
			return pop_bridge[layer][index];
	}


	// Ư�� �˾� ���̾� ��ü ����
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


	// ��� �˾� ��� ��ü ����
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

	#endif
	#endif
};

#endif
#endif
#endif