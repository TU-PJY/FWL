#pragma once
#include <windows.h>
#include <iostream>
#include <random>

#include "FWL.h"
#include "global.h"
// ���� ������Ʈ Ŭ���� ����

// ��� ��ü�� update(), check_collision(), render(), check_delete()�� FUNCTION �Ǵ� POP_FUNCTION�κ��� ��� �޾ƾ� ��
// �Ʒ��� �ڵ���� ���� �ڵ���

std::default_random_engine dre;
std::uniform_int_distribution<int> uid{ 1, 10 };

// FUNCTION Ŭ�����κ��� ��ӹ޾ƾ���
class Human : public FUNCTION { 
private:
	int type, num, cnt{};

	// layer ������ ��� ���� ������Ʈ���� �ݵ�� ������ �־�� ��
	// ��� ���̾ �����ϴ��� FWL::delete_object() �Լ����� �˷�����ϱ� ����
	int layer;

	// ��ü ���� �÷���
	// �ٸ� �̸����ε� ���� �÷��׸� ������ �� ������ 'delete_flag'�� �����ϴ� ���� ������
	// true�� �Ǹ� check_delete()���� ��ü ���� �ڵ� ����
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

	// FUNCTION class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	// �ܺο��� ��ü�� ��ȣ �����͸� ��� �Լ�
	int get_info() const { return type; }


	// FUNCTION class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
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


	// FUNCTION class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	// �浹 ���� ������ public���� ���� �� ���� ������
    void check_collision() {
		if (class_message)
			std::cout << "checked collision of Human " << type << "   " << cnt << " times" << std::endl;
	}


	// FUNCTION class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	// ��ü ������
	void render() {
		if (class_message)
			std::cout << "rendered Human " << type << "   " << cnt << " times" << std::endl << std::endl;
	}


	// FUNCTION class�κ��� ��ӹ޾� �ش� Ŭ�������� ������
	// ��ü ������ �� �Լ����� �����ؾ���
	void check_delete() {
		if (delete_flag)  // ��ü ���� �÷��װ� Ȱ��ȭ �Ǿ��ٸ� ��ü ������ ���� ����
			fw.delete_object(this, layer);

		if (class_message)
			Sleep(100);
	}
};


class Monster : public FUNCTION {
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

	void check_collision() {
		if (class_message)
			std::cout << "checked collision of Monster " << type << "   " << cnt << " times" << std::endl;
	}

	void render() {
		if (class_message)
			std::cout << "rendered Monster " << type << "   " << cnt << " times" << std::endl << std::endl;
	}

	void check_delete() {
		if (delete_flag)
			fw.delete_object(this, layer);

		if (class_message)
			Sleep(100);
	}
};


class ConnectPtrExample : public FUNCTION {
public:
	int layer;
	bool delete_flag{};

	ConnectPtrExample(int l) {
		layer = l;

		std::cout << std::endl << "added ConnectPtrExample" << std::endl;
	}

	~ConnectPtrExample() {
		std::cout << std::endl << "deleted ConnectPtrExample" << std::endl;
	}

	void update() {
		// connect_ptr()�� Ư�� ���̾ ���ϴ� Ư�� �ε����� �����Ͱ� ����ǵ��� �Ѵ�

		if (!class_message) {
			for (int i = 0; i < fw.layer_size(0); ++i) {
				auto ptr = fw.connect_ptr(0, i);  // 0�� ���̾��� ��ü�鿡 ������ ����

				if (ptr != nullptr)  // ptr->get_info()�� ���� �� ��ü�� type�� �� ����Ѵ�
					std::cout << "ptr got number '" << ptr->get_info() << "' from objects in layer 0" << std::endl;
			}


			for (int i = 0; i < fw.layer_size(1); ++i) {
				auto ptr = fw.connect_ptr(1, i);  // 1�� ���̾��� ��ü�鿡 ������ ����

				if (ptr != nullptr)
					std::cout << "ptr got number '" << ptr->get_info() << "' from objects in layer 1" << std::endl;
			}

			// ��� ����ߴٸ� ���� �÷��� Ȱ��ȭ
			delete_flag = true;
		}
	}

	void check_collision() {}
	void render() {}

	void check_delete() {
		if (delete_flag)
			fw.delete_object(this, layer);
	}
};