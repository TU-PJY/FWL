#include "Objects.h"  // Framework.h�� ����

// ���ø� ���� ����
// Header.h - Framework.h - Objects.h
//                 |             |
//           Framework.cpp    main.cpp

int main() {
	// ������ ��ũ�� ������Ʈ �߰�
	// ���� �߰��� ���� �Ǵ� ���� ���̾���� ������Ʈ ��
	// ��ü �߰� �� �ݵ�� �����ڰ� ���̾� ��ȣ�� �����ϵ��� �ؾ� ��
	// Monster�� ���� ���̾ �����Ƿ� Monster���� ��ü �ڵ� ����
	__add__(new Human(1, 2), 2);
	__add__(new Human(2, 2), 2);
	__add__(new Human(3, 2), 2);
	__add__(new Monster(1, 1), 1);
	__add__(new Monster(2, 1), 1);
	__add__(new Monster(3, 1), 1);

	std::cout << std::endl;
	
	// ��ü ���� ����
	while (true) {
		__routine__();
	}
}
