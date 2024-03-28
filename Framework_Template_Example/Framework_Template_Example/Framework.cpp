#include "Framework.h"


// �����ӿ�ũ ���� �迭
// Framework.h�� ������ LAYER ��ŭ �����ӿ�ũ ���� �迭 ����
std::array<std::vector<Framework*>, LAYER> framework;

clock_t start_time, end_time;  // ���� ������ �ҿ�Ǵ� �ð� ����
double ft;                     // ������ Ÿ��


// ��ü ���� ���� (��: ���, �̵� ��...)
void __routine__() {

    // ���� ���� �ҿ�ð� ���� ����
	start_time = clock();  	

	// ���� ���̾���� ������� ������Ʈ �ڵ带 ������
	for(int i = 0; i < framework.size(); i ++) {
		for (auto it = framework[i].begin(); it != framework[i].end();) {
			auto& ptr = *it;

			// ��ü�� �����ϸ� ������Ʈ �ڵ带 ����
			if (ptr != nullptr) {
				ptr->render();
				ptr->check_collision();

				// �� ��ü�� update() �Լ����� __delete()__ �Լ��� ����Ǳ� ������ �ݵ�� ���� �������� ����Ǿ�� ��
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
void __add__(Framework*&& object, int layer) {
	framework[layer].push_back(object);
}


// ���� ������Ʈ ����
void __delete__(Framework* object, int layer) {

	// ���� ������Ʈ�� ������ �����ϴ��� Ȯ��
	auto target = std::find(framework[layer].begin(), framework[layer].end(), object);

	// ��ü�� �����ϴ� ������ �ǴܵǸ� ���� �ڵ� ����
	if (target != framework[layer].end()) {
		// ������Ʈ ����
		delete* target; 

		// ������Ʈ�� �� �̻� �������� ����
		// ���� �����ӿ�ũ �ε����� __routine__()�Լ����� ������
		*target = nullptr;
	}
}


// ��� ���� ������Ʈ ����
void __sweep__() {

	// ��� ���̾��� ���� ������Ʈ���� ����
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
