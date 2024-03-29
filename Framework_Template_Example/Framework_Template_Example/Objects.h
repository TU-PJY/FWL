#pragma once
#include "Framework.h"
// ���� ������Ʈ Ŭ���� ����

// ��� Ŭ������ render(), check_collision(), update() �Լ��� Framework class�κ��� ��� �޾ƾ� ��
// �Ʒ��� �ڵ���� ���� �ڵ���

std::default_random_engine dre;
std::uniform_int_distribution<int> uid{ 1, 10 };


class Human : public Framework { 
private:
	int type, cnt, num;

	// layer ������ ��� ���� ������Ʈ���� �ݵ�� ������ �־�� ��
	// ��� ���̾ �����ϴ��� fw_delete() �Լ����� �˷�����ϱ� ����
	int layer;

public:
	bool collision;  // �浹 �� true

	// ��ü ���� �� �ݵ�� layer ��ȣ�� �����ؾ���
	Human(int i, int l) {
		layer = l;
		type = i;
		cnt = 0;

		// ���� ���� ����
		num = uid(dre); 

		// �浹 ���δ� �׻� false�� ������ ��
		collision = false;

		std::cout << "added Human " << type << std::endl;
	}

	// Framework class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	virtual void render() {
		std::cout << "updated Human " << type << "   " << cnt << " times" << std::endl;
	}

	// Framework class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	// �浹 ���� ������ public���� ���� �� ���� ������
    virtual void check_collision() {
		std::cout << "checked collision of Human " << type << "   " << cnt << " times" << std::endl << std::endl;
	}

	// Framework class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	virtual void update() {
		cnt++;

		Sleep(100);

		// cnt�� num���� ũ�� ��ü �����θ� ����
		// Ư�� ���ǿ� ��ü �����θ� �����Ѵ�.
		// �ܺ� �Լ����� �����ϴ� ���̱� ������ �ı��ڰ� ������� ������ ����
		if (cnt > num) { 
			std::cout << "deleted Human " << type << std::endl << std::endl;
			fw_delete(this, layer);  
		}
	}
};


class Monster : public Framework {
private:
	int type, cnt, num;
	int layer;

public:
	bool collision;

	Monster(int i, int l) {
		layer = l;
		type = i;
		cnt = 0;
		num = uid(dre);
		collision = false;
		
		std::cout << "added Monster " << type << std::endl;
	}


	virtual void render() {
		std::cout << "updated Monster " << type << "   " << cnt << " times" << std::endl;
	}

	virtual void check_collision() {
		std::cout << "checked collision of Monster " << type << "   " << cnt << " times" << std::endl << std::endl;
	}
	
	virtual void update() {
		cnt++;

		Sleep(100);

		if (cnt > num) {
			std::cout << "deleted Monster " << type << std::endl << std::endl;
			fw_delete(this, layer);
		}
	}
};
