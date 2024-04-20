#include <iostream>

#include "Framework.h"
#include "Objects.h"
#include "modes.h"


// �����ӿ�ũ�� �ʿ��� ���� extern Ű���带 �ٿ� �����ӿ�ũ ����� ����� ��
extern Framework fw;

// ���� ��带 �����ϴ� �ڵ�
// ���� �ڵ�� void type �Լ��� �ۼ��� ��
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
	// 2�� ���̾ �߰�
	if (!class_message)
		fw.add_object(new ConnectPtrExample(2), 2);

	std::cout << std::endl;
}


// �ٸ� ���� ���� �� �޽����� �����
void other_mode() {
	std::cout << "mode changed to other_mode" << std::endl << std::endl;
}