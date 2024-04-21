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


// �� ��ü�� �Լ��� �����Ű�� Ŭ����
// �����ӿ�ũ���� �����ϴ� �Լ���
class FUNCTION {
public:
	// �Ʒ� 4���� ����Լ����� �ݵ�� ���� ������Ʈ Ŭ�������� ��� �޵��� �ؾ���
	virtual void update() {}					   // ���� ������Ʈ ������Ʈ
	virtual void check_collision() {}			   // ���� ������Ʈ �浹 ó��
	virtual void render() {}					   // ���� ������Ʈ ���
	virtual void check_delete_flag() {}			   // ���� ������Ʈ ���� �÷��� �˻�

	// �̰��� ����� ���� �Լ� ���� ����
	// ���� ���� �Լ��� ��� �⺻ ���� �����ϴ� �ڵ�� ������ ��. ��ü Ŭ�������� ������ �ǹǷ� FUNCTION ��� �Լ��� ������ �ǹ̰� ����
	virtual int get_info() const {return {};}

	virtual     ~FUNCTION() {}             //�Ҹ���, �� ��ü�� �Ҹ��ڴ� FUNCTION Ŭ������ ���� ��ü�� �Ҹ��ڸ� ȣ����
};


#ifdef USING_POPUP_MODE
#ifdef NUMBER_OF_LAYER_POPUP

// �˾� ��� ���� FUNCTION Ŭ����
class POP_FUNCTION {
public:
	// �Ʒ� 4���� ����Լ����� �ݵ�� ���� ������Ʈ Ŭ�������� ��� �޵��� �ؾ���
	virtual void update() {}					   // ���� ������Ʈ ������Ʈ
	virtual void check_collision() {}			   // ���� ������Ʈ �浹 ó��
	virtual void render() {}					   // ���� ������Ʈ ���
	virtual void check_delete_flag() {}			   // ���� ������Ʈ ���� �÷��� �˻�

	// �̰��� ����� ���� �Լ� ���� ����

	virtual     ~POP_FUNCTION() {}
};

#endif
#endif