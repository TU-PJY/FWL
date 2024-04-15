#pragma once
#include "Header.h"


#define          LAYER 3  // ���̾� ������ �̰����� ����

// �Ʒ� �ɼ��� Ȱ��ȭ/��Ȱ��ȭ ����
//#define          OPT_OPTIMIZING  //  �����ӿ�ũ ��ƾ ���� ���� �ǽð� �޸� ����ȭ �ɼ�


// �����ӿ�ũ Ŭ���� �� ���� ����, �Լ�
// '����� - ���� ������Ʈ' �Ǵ� '���� ������Ʈ - ���� ������Ʈ' ����� �����ϰ� ���ִ� �������̽�
class Framework {
public:

	// �Ʒ��� �Լ����� �ݵ�� ���� ������Ʈ Ŭ�������� ��� �޵��� �ؾ���
	virtual void update()            {}  // ���� ������Ʈ ������Ʈ
	virtual void check_collision()   {}  // ���� ������Ʈ �浹 ó��
	virtual void render()            {}  // ���� ������Ʈ ���
	virtual void check_delete_flag() {}  // ���� ������Ʈ ���� �÷��� �˻�

	// �̿ܿ��� ��ü ���� ��� ���� �Լ��� ���� ���� ����

	virtual      ~Framework() {}           //�Ҹ���, Framework Ŭ������ ���� ��ü�� �Ҹ��ڸ� ȣ����
};


extern           clock_t start_time, end_time;  // ���� ������ �ҿ�Ǵ� �ð� ����
extern           double ft;                     // ������ Ÿ��

// �����ӿ�ũ ���� �Լ��� fw_function() ���� ���
void             fw_routine();                             // ��ü ���� ����

void             fw_add(Framework*&& object, int layer);   // ���� ������Ʈ �߰�
Framework*       fw_set_tracking(int layer, int index);
bool             fw_check_tracking_valid(int layer, int index);
int              fw_layer_size(int layer);
void             fw_delete(Framework* object, int layer);  // ���� ������Ʈ ����
void             fw_sweep_layer(int layer);                // Ư�� ���̾��� ��� ���� ������Ʈ ����
void             fw_sweep_all();                           // ��� ���� ������Ʈ ����
