#pragma once
// �����ӿ�ũ �긴�� ����

// �ʼ� �ɼ��� ���� �ؾ� �����ӿ�ũ�� Ȱ��ȭ ��
// �ݵ�� �� ���Ͽ� ������ �� 

// �ʼ� �ɼ�, �ϳ��� ���� �� �ϸ� �����ӿ�ũ�� Ȱ��ȭ ���� ����
#define          USING_FRAMEWORK            // �����ӿ�ũ ��� Ȱ��ȭ
#define          NUMBER_OF_LAYER 3          // ���̾� ���� ����
#define          START_MODE "example_mode"  // ���� ��� ����, ���ڿ��� ����

// ���� �ɼ�, ��Ȱ��ȭ �Ǵ� Ȱ��ȭ ����
#define          USING_POPUP_MODE           // �˾� ��� ��� ����
#define          NUMBER_OF_LAYER_POPUP 3    // �˾���� ���̾� ���� ����

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

	virtual     ~Bridge() {}             //�Ҹ���, Bridge Ŭ������ ���� ��ü�� �Ҹ��ڸ� ȣ����
};


#ifdef USING_POPUP_MODE
#ifdef NUMBER_OF_LAYER_POPUP

// �˾� ��� ���� Bridge Ŭ����
class POP_Bridge {
public:
	// Bridge ����Լ����� �ݵ�� ���� ������Ʈ Ŭ�������� ��� �޵��� �ؾ���
	virtual void update() {}             // ���� ������Ʈ ������Ʈ
	virtual void check_collision() {}    // ���� ������Ʈ �浹 ó��
	virtual void render() {}             // ���� ������Ʈ ���
	virtual void check_delete_flag() {}  // ���� ������Ʈ ���� �÷��� �˻�

	// �̰��� ����� ���� �Լ� ���� ����

	virtual     ~POP_Bridge() {}             //�Ҹ���, Bridge Ŭ������ ���� ��ü�� �Ҹ��ڸ� ȣ����
};

#endif
#endif