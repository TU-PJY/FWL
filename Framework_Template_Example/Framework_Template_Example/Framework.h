#pragma once
#include "Header.h"
#define LAYER 3  // ���̾� ������ �̰����� ����


// �����ӿ�ũ Ŭ���� �� ���� ����, �Լ�
// Ŭ���� ��ü�� �ƹ� ���ҵ� ���� ������, ���� ������Ʈ���� ��� �Ѳ����� ��ü �ڵ带 �����ϴ� �����̳� ������ ��
class Framework {
public:
	// �Ʒ��� �Լ����� �ݵ�� ���� ������Ʈ Ŭ�������� ��� �޵��� �ؾ���
	virtual void render() {}          // ���� ������Ʈ ���
	virtual void check_collision() {} // ���� ������Ʈ �浹 ó��
	virtual void update() {}          // ���� ������Ʈ ������Ʈ
};


extern clock_t start_time, end_time;  // ���� ������ �ҿ�Ǵ� �ð� ����
extern double ft;                     // ������ Ÿ��

// �������� ������ ���� __function()__ ���� ���
void __routine__();                             // ��ü ���� ����

void __add__(Framework*&& object, int layer);   // ���� ������Ʈ �߰�
void __delete__(Framework* object, int layer);  // ���� ������Ʈ ����
void __sweep__();                               // ��� ���� ������Ʈ ����
