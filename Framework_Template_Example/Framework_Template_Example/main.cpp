#include "Framework.h"
#include "modes.h"

// true�� �� �� ��ü Ŭ���� �޽��� ���, false�� �� connect_ptr()���� �޽��� ��� 
bool class_message = true;  

// true�� �� �ϴ��� ��� ���� �ڵ� ����
bool mode_change = false;

// �����ӿ�ũ ���� ����
Framework fw;

int main() {
	// �Լ��� ���ڷ� ����. �����ϰ��� �ϴ� �ڵ���� �Լ��� �ۼ��Ͽ� ���ڿ� ����
	// ��� ���� �ڵ� ����
	// ��� ���� �ڵ�� modes.cpp�� ����
	if(std::string(START_MODE) == "example_mode")
		fw.init_start_mode(example_mode, "example_mode");

	// mode_change Ȱ��ȭ �� example_mode���� �߰��Ǿ��� ��ü�� ��� ��� �����ǰ� ����� ��� ���� �ڵ忡 �ִ� �޽����� ��µȴ�
	// ���� ��� �̸��� ���� �Լ� �̸��� ������ �ۼ��ϴ� ���� ������
	if(mode_change)
		fw.change_mode(other_mode, "other_mode");

	// ��ü ���� ����
	while (true) {
		fw.routine();
	}
}
