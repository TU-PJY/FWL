#pragma once
// �����ӿ�ũ �긴�� ����

// �ʼ� �ɼ��� ���� �ؾ� �����ӿ�ũ�� Ȱ��ȭ ��
// �ݵ�� �� ���Ͽ� ������ �� 

// �ʼ� �ɼ�
#define          USING_FRAMEWORK       // �����ӿ�ũ ��� Ȱ��ȭ
#define          NUMBER_OF_LAYER 3         // ���̾� ���� ����

// ���� �ɼ�
//#define        USING_OPTIMIZING  // �ǽð� �޸� ����ȭ ��� �ɼ�


// ���� ������Ʈ - �����ӿ�ũ ����� ������
// Game object <----> Bridge <----> Framework <----> User

// Bridge Ŭ����
class Bridge {
public:
	// Bridge ����Լ����� �ݵ�� ���� ������Ʈ Ŭ�������� ��� �޵��� �ؾ���
	virtual void update() {}             // ���� ������Ʈ ������Ʈ
	virtual void check_collision() {}    // ���� ������Ʈ �浹 ó��
	virtual void render() {}             // ���� ������Ʈ ���
	virtual void check_delete_flag() {}  // ���� ������Ʈ ���� �÷��� �˻�

	// �̰��� ����� ���� �Լ� ���� ����
	// ���� ���� �Լ��� ��� �⺻ ���� �����ϴ� �ڵ�� ������ ��. ��ü Ŭ�������� ������ �ǹǷ� Bridge ��� �Լ��� ������ �ǹ̰� ����
	virtual int get_info() const {return {};}

	virtual      ~Bridge() {}            //�Ҹ���, Bridge Ŭ������ ���� ��ü�� �Ҹ��ڸ� ȣ����
};