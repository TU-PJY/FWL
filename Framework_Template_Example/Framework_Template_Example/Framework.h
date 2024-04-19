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
	double    ft{};  // ������ Ÿ��, ��� ������Ʈ ������ �Ǵ� ��ġ ������Ʈ�� ���ؾ� ��
	clock_t   start_time{}, end_time{};  // ��ƾ ���� �ҿ� �ð� ���� ����

	bool framework_enable{};  // �����ӿ�ũ ���� Ȱ��ȭ ����, true�� �Ǿ�� routine()�Լ� ����
	using func = void(*)();  // callable type �Լ� ����

	std::array<std::deque<Bridge*>, NUMBER_OF_LAYER> bridge{};


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
						ptr->update();
						ptr->check_collision();
						ptr->render();

						// ���� �÷��� �˻�
						// �ݵ�� check_delete_flag()�� ���� ������ ��, �̿��� �Լ����� ���� �� ������ �߻��� �� ����
						ptr->check_delete_flag();

						++it;  // �ݺ��� ����
					}

					// ��ü�� �������� �ʴ´ٸ� �ش� ��ü�� �ִ� ���� �ε����� ����
					else
						it = bridge[i].erase(remove(bridge[i].begin(), bridge[i].end(), ptr));
				}
			}

			// ���� ���� �ҿ�ð� ���� ����
			end_time = clock();

			// ���� ������ ������ Ÿ�� ���
			ft = (double)(end_time - start_time) / 1000;
		}
	}


	// ���� ������Ʈ �߰�
	void add_object(Bridge*&& object, int layer) {
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
	int layer_size(int layer) {
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
			// ���� �����ӿ�ũ �ε����� routine() �Լ����� ������
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


	// ���� ��� ����
	void init_start_mode(func modefunc) {
		modefunc();
		framework_enable = true;  // ���� ��� ������ �Ϸ�Ǹ� �����ӿ�ũ ��ƾ ����
	}


	// ��� ����
	void change_mode(func modefunc) {
		sweep_all();  // ��� ��ȯ �� ������ ��ü�� ��� ������ �� ��� ����

		framework_enable = false;  // �����ӿ�ũ �Ͻ�����
		modefunc();  // ��� ���� �ڵ� ����
		framework_enable = true;  // �����ӿ�ũ �簳
	}
};

#endif
#endif
#endif