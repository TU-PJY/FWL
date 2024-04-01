#include "Framework.h"


// �����ӿ�ũ ���� �迭
// Framework.h�� ������ LAYER ��ŭ �����ӿ�ũ ���� �迭 ����
std::array<std::vector<Framework*>, LAYER> framework;

clock_t start_time, end_time;  // ���� ������ �ҿ�Ǵ� �ð� ����
double ft;                     // ������ Ÿ��


// ��ü ���� ���� (��: ���, �̵� ��...)
void fw_routine() {

    // ���� ���� �ҿ�ð� ���� ����
	start_time = clock();  	

	// ���� ���̾���� ������� ������Ʈ �ڵ带 ������
	for(int i = 0; i < framework.size(); i ++) {

		// �� ���̾ �湮�� ������ ���̾ ������ ���Ϳ� �ʿ� �̻����� �޸𸮰� �Ҵ�Ǿ����� Ȯ��
		// �޸� �Ҵ緮�� �ּ������� ���δ�.
		if(framework[i].size() < framework[i].capacity())
			framework[i].shrink_to_fit();

		for (auto it = framework[i].begin(); it != framework[i].end();) {
			auto& ptr = *it;

			// ��ü�� �����ϸ� ������Ʈ �ڵ带 ����
			if (ptr != nullptr) {
				ptr->render();
				ptr->check_collision();

				// �� ��ü�� update() �Լ����� fw_delete() �Լ��� ����Ǳ� ������ �ݵ�� ���� �������� ����Ǿ�� ��
				// ������ ��ü ������ �����ϱ� ����
				ptr->update();	

				++it;  // �ݺ��� ����
			}

			// ��ü�� �������� �ʴ´ٸ� �ش� ��ü�� �ִ� ���� �ε����� ����
			// ���� ��� �ε������� ������ �и��� ������ ���� �� �ڵ����� ���� �ε��� ������ ��
			else 
				it = framework[i].erase(it);
		}
	}

	// ���� ���� �ҿ�ð� ���� ����
	end_time = clock();  

	// ���� ������ ������ Ÿ�� ���
	ft = (double)(end_time - start_time) / 1000;
}


// ���� ������Ʈ �߰�
void fw_add(Framework*&& object, int layer) {
	framework[layer].push_back(object);
}


// ���� ������Ʈ ����
void fw_delete(Framework* object, int layer) {

	// ���� ������Ʈ�� ������ �����ϴ��� Ȯ��
	auto target = std::find(framework[layer].begin(), framework[layer].end(), object);

	// ��ü�� �����ϴ� ������ �ǴܵǸ� ���� �ڵ� ����
	if (target != framework[layer].end()) {
		// ������Ʈ ����
		delete* target; 

		// ������Ʈ�� �� �̻� �������� ����
		// ���� �����ӿ�ũ �ε����� fw_routine() �Լ����� ������
		*target = nullptr;
	}
} 


// Ư�� ���̾��� ��� ���� ������Ʈ ����
void fw_sweep_layer(int layer) {
	for (auto it = framework[layer].begin(); it != framework[layer].end();) {
		auto& ptr = *it;
		auto target = std::find(framework[layer].begin(), framework[layer].end(), ptr);

		delete* target;
		*target = nullptr;

		it++;
	}
}


// ��� ���� ������Ʈ ����
void fw_sweep_all() {
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
