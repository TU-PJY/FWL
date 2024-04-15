#pragma once
#include "Framework.h"
// ���� ������Ʈ Ŭ���� ����

// ��� ��ü�� update(), check_collision(), render(), check_delete_flag()�� Framework class�κ��� ��� �޾ƾ� ��
// �Ʒ��� �ڵ���� ���� �ڵ���

std::default_random_engine dre;
std::uniform_int_distribution<int> uid{ 1, 10 };


class Human : public Framework { 
private:
	int type, num, cnt{};

	// layer ������ ��� ���� ������Ʈ���� �ݵ�� ������ �־�� ��
	// ��� ���̾ �����ϴ��� fw_delete() �Լ����� �˷�����ϱ� ����
	int layer;

	// ��ü ���� �÷���
	// �ٸ� �̸����ε� ���� �÷��׸� ������ �� ������ 'delete_flag'�� �����ϴ� ���� ������
	// true�� �Ǹ� check_delete_flag()���� ��ü ���� �ڵ� ����
	bool delete_flag{};

public:
	// ��ü ���� �� �ݵ�� layer ��ȣ�� �����ؾ���
	Human(int i, int l) {
		layer = l;
		type = i;

		// ���� ���� ����
		num = uid(dre); 

		std::cout << "added Human " << type << std::endl;
	}

	~Human() {
		std::cout  << "deleted Human " << type << std::endl << std::endl;
	}


	// Framework class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	// ��ü ������Ʈ
	void update() {
		cnt++;

		std::cout << "updated Human " << type << "   " << cnt << " times" << std::endl;

		// cnt�� num���� ũ�� ��ü ���� �÷��װ� Ȱ��ȭ ��
		// * ���� �÷��� Ȱ��ȭ�� check_delete_flag()�� ������ ��� ��ü �Լ����� ������
		if (cnt > num)
			delete_flag = true;
	}

	// Framework class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	// ��ü ������
	void render() {
		std::cout << "rendered Human " << type << "   " << cnt << " times" << std::endl << std::endl;
	}

	// Framework class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	// �浹 ���� ������ public���� ���� �� ���� ������
    void check_collision() {
		std::cout << "checked collision of Human " << type << "   " << cnt << " times" << std::endl;
	}

	// Framework class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	// ��ü ������ �� �Լ����� �����ؾ���
	void check_delete_flag() {
		if (delete_flag)  // ��ü �÷��װ� Ȱ��ȭ �Ǿ��ٸ� ��ü ������ ���� ����
			fw_delete(this, layer);

		Sleep(100);
	}
};


class Monster : public Framework {
private:
	int type, num, cnt{};

	int layer;
	bool delete_flag{};

public:
	Monster(int i, int l) {
		layer = l;
		type = i;
		num = uid(dre);
		
		std::cout << "added Monster " << type << std::endl;
	}

	~Monster() {
		std::cout << "deleted Monster " << type << std::endl << std::endl;
	}

	void update() {
		cnt++;

		std::cout << "updated Monster " << type << "   " << cnt << " times" << std::endl;

		if (cnt > num)
			delete_flag = true;
	}

	void render() {
		std::cout << "rendered Monster " << type << "   " << cnt << " times" << std::endl << std::endl;
	}

	void check_collision() {
		std::cout << "checked collision of Monster " << type << "   " << cnt << " times" << std::endl;
	}

	void check_delete_flag() {
		if (delete_flag)
			fw_delete(this, layer);

		Sleep(100);
	}
};
