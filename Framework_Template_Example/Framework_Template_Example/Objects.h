#pragma once
#include <windows.h>
#include <iostream>
#include <random>

#include "Framework.h"
// ���� ������Ʈ Ŭ���� ����

// ��� ��ü�� update(), check_collision(), render(), check_delete_flag()�� Bridge class�κ��� ��� �޾ƾ� ��
// �Ʒ��� �ڵ���� ���� �ڵ���

extern bool class_message;

std::default_random_engine dre;
std::uniform_int_distribution<int> uid{ 1, 10 };

// �����ӿ�ũ�� �ʿ��� ���� �׻� ������ ��
Framework fw;

// Bridge�� FrameworkŬ������ �����ؾ� �� ��ü�� ����ڿ� ��� ������
class Human : public Bridge { 
private:
	int type, num, cnt{};

	// layer ������ ��� ���� ������Ʈ���� �ݵ�� ������ �־�� ��
	// ��� ���̾ �����ϴ��� Framework::delete_object() �Լ����� �˷�����ϱ� ����
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

		if(class_message)
			std::cout << "added Human " << type << std::endl;
	}

	~Human() {
		if (class_message)
			std::cout  << "deleted Human " << type << std::endl << std::endl;
	}

	// �ܺο��� ��ü�� �����͸� ��� �Լ�
	int get_info() const { return type; }

	// Bridge class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	// ��ü ������Ʈ
	void update() {
		cnt++;

		if (class_message)
			std::cout << "updated Human " << type << "   " << cnt << " times" << std::endl;

		// cnt�� num���� ũ�� ��ü ���� �÷��װ� Ȱ��ȭ ��
		// * ���� �÷��� Ȱ��ȭ�� check_delete_flag()�� ������ ��� ��� �Լ����� ������
		if (cnt > num)
			delete_flag = true;
	}

	// Bridge class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	// ��ü ������
	void render() {
		if (class_message)
			std::cout << "rendered Human " << type << "   " << cnt << " times" << std::endl << std::endl;
	}

	// Bridge class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	// �浹 ���� ������ public���� ���� �� ���� ������
    void check_collision() {
		if (class_message)
			std::cout << "checked collision of Human " << type << "   " << cnt << " times" << std::endl;
	}

	// Bridge class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	// ��ü ������ �� �Լ����� �����ؾ���
	void check_delete_flag() {
		if (delete_flag)  // ��ü ���� �÷��װ� Ȱ��ȭ �Ǿ��ٸ� ��ü ������ ���� ����
			fw.delete_object(this, layer);

		if (class_message)
			Sleep(100);
	}
};


class Monster : public Bridge {
private:
	int type, num, cnt{};

	int layer;
	bool delete_flag{};

public:
	Monster(int i, int l) {
		layer = l;
		type = i;
		num = uid(dre);

		if (class_message)
			std::cout << "added Monster " << type << std::endl;
	}

	~Monster() {
		if (class_message)
			std::cout << "deleted Monster " << type << std::endl << std::endl;
	}

	int get_info() const { return type; }

	void update() {
		cnt++;

		if (class_message)
			std::cout << "updated Monster " << type << "   " << cnt << " times" << std::endl;

		if (cnt > num)
			delete_flag = true;
	}

	void render() {
		if (class_message)
			std::cout << "rendered Monster " << type << "   " << cnt << " times" << std::endl << std::endl;
	}

	void check_collision() {
		if (class_message)
			std::cout << "checked collision of Monster " << type << "   " << cnt << " times" << std::endl;
	}

	void check_delete_flag() {
		if (delete_flag)
			fw.delete_object(this, layer);

		if (class_message)
			Sleep(100);
	}
};


class DependPtr : public Bridge {
public:
	int layer;
	bool delete_flag{};

	DependPtr(int l) {
		layer = l;
	}

	~DependPtr() {
		std::cout << std::endl << "deleted DependPtr" << std::endl;
	}

	void update() {
		// ptr_track()�� Ư�� ���̾��� Ư�� ��ü�� �ε����� ���� �����Ͱ� �����ϵ��� �Ѵ�

		if (!class_message) {
			for (int i = 0; i < fw.layer_size(0); ++i) {
				auto ptr = fw.ptr_track(0, i);  // 0�� ���̾��� ��ü�鿡 ���� ������ ����
				if (ptr != nullptr)
					std::cout << "ptr got number '" << ptr->get_info() << "' from objects in layer 0" << std::endl;
			}

			for (int i = 0; i < fw.layer_size(1); ++i) {
				auto ptr = fw.ptr_track(1, i);  // 1�� ���̾��� ��ü�鿡 ���� ������ ����
				if (ptr != nullptr)
					std::cout << "ptr got number '" << ptr->get_info() << "' from objects in layer 1" << std::endl;
			}

			// ��� ����ߴٸ� ���� �÷��� Ȱ��ȭ
			delete_flag = true;
		}
	}

	void check_collision() {}
	void render() {}

	void check_delete_flag() {
		if (delete_flag)
			fw.delete_object(this, layer);
	}
};