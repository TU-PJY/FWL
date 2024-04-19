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


// ���� ��带 �����ϴ� �ڵ�
// ���� �ڵ�� void type �Լ��� �ۼ��� ��
// ��� ���� �ڵ�� ������ ���Ϸ� �ۼ��ϴ� ���� ������. �ش� ���������� main.cpp�� �ۼ�
void example_mode() {
	// ������ ��ũ�� ������Ʈ �߰�
	// ���� ���̾����, ���̾ �߰��� �������� ������Ʈ ��

	// �� ���������� fw.add_object(new ��ü(��ü ��ȣ, ���̾� ��ȣ), ���̾� ��ȣ) �������� �Ǿ�����
	// ��ü �߰� �� �ݵ�� �����ڿ� ���̾� ��ȣ�� �Է� �ؾ� ��
	// ��ü ��ȣ�� �� �������� ��ü ������ ���� ����ϴ� �����̸�, ���� ��� �� ��ü ��ȣ�� ��� ��
	// Monster�� ���� ���̾ �����Ƿ� Monster���� ��ü �ڵ� ����
	fw.add_object(new Human(1, 1), 1);
	fw.add_object(new Human(2, 1), 1);
	fw.add_object(new Human(3, 1), 1);
	fw.add_object(new Monster(1, 0), 0);
	fw.add_object(new Monster(2, 0), 0);
	fw.add_object(new Monster(3, 0), 0);

	// connect_ptr() ����� ��ü
	if (!class_message)
		fw.add_object(new ConnectPtrExample(2), 2);

	std::cout << std::endl;
}


int main() {
	// ��� ���� �ڵ� ����
	if(START_MODE == "example")
		fw.init_start_mode(example_mode);  // �Լ��� ���ڷ� ����. �����ϰ��� �ϴ� �ڵ���� �Լ��� �ۼ��Ͽ� ���ڿ� ����

	// ��ü ���� ����
	while (true) {
		fw.routine();
	}
}
