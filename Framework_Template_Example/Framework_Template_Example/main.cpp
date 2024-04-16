#include "Objects.h"
#include "Framework.h"

// ���ø� ���� ����
// FW_config.h - Framework.h - Objects.h
//                   |             |
//             Framework.cpp    main.cpp

// �����ӿ�ũ ����. ���� ���� �� extern Ű���带 ���� ��
extern Framework fw;

// true�� �� �� ��ü Ŭ���� �޽��� ���, false�� �� connect_ptr()���� �޽��� ��� 
bool class_message = true;  


int main() {
	// ������ ��ũ�� ������Ʈ �߰�
	// �� ���������� fw.add_object(new ��ü(��ü ��ȣ, ���̾� ��ȣ), ���̾� ��ȣ) �������� �Ǿ�����
	// ���� ���̾����, ���̾ �߰��� �������� ������Ʈ ��
	
	// ��ü ��ȣ�� �� �������� ��ü ������ ���� ����ϴ� �����̸�, ���� ��� �� ��ü ��ȣ�� ��� ��
	// ��ü �߰� �� �ݵ�� �����ڿ� ���̾� ��ȣ�� �Է� �ؾ� ��
	
	// Monster�� ���� ���̾ �����Ƿ� Monster���� ��ü �ڵ� ����
	fw.add_object(new Human(1, 1), 1);
	fw.add_object(new Human(2, 1), 1);
	fw.add_object(new Human(3, 1), 1);
	fw.add_object(new Monster(1, 0), 0);
	fw.add_object(new Monster(2, 0), 0);
	fw.add_object(new Monster(3, 0), 0);

	// connect_ptr() ����� ��ü
	fw.add_object(new ConnectPtrExample(2), 2);

	std::cout << std::endl;
	
	// ��ü ���� ����
	while (true) {
		fw.routine();
	}
}
