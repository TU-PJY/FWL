#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <ctime>

// �����ӿ�ũ ���� ���
#include "FW_config.h"

#ifdef USING_FRAMEWORK
#ifdef NUMBER_OF_LAYER

class Framework {
public:
	double    ft{};  // ������ Ÿ��, ��� ������Ʈ ������ �Ǵ� ��ġ ������Ʈ�� ���ؾ� ��
	clock_t   start_time{}, end_time{};  // ��ƾ ���� �ҿ� �ð� ���� ����

	std::array<std::vector<Bridge*>, NUMBER_OF_LAYER> bridge{};

	// ��ü ���� ���� (��: ���, �̵� ��...)
	void routine() {

		// ���� ���� �ҿ�ð� ���� ����
		start_time = clock();

		// ���� ���̾���� ������� ������Ʈ �ڵ带 ������
		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {

			// �޸𸮰� ���ϰ� �Ҵ�Ǿ����� ��� �Ҵ緮 ����ȭ
			// �ɼ� Ȱ��ȭ �ÿ��� ����
			#ifdef USING_OPTIMIZING
			if (bridge[i].size() * 2 < bridge[i].capacity())
				bridge[i].shrink_to_fit();
			#endif

			for (auto it = bridge[i].begin(); it != bridge[i].end();) {
				auto& ptr = *it;

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
		for (int i = 0; i < NUMBER_OF_LAYER; i++) {
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