#pragma once
#include <windows.h>
#include <iostream>
#include <random>

#include "Framework.h"
// 게임 오브젝트 클래스 예제

// 모든 객체는 update(), check_collision(), render(), check_delete_flag()를 Bridge class로부터 상속 받아야 함
// 아래의 코드들은 예제 코드임

extern bool class_message;

std::default_random_engine dre;
std::uniform_int_distribution<int> uid{ 1, 10 };

// 프레임워크가 필요한 곳은 항상 선언할 것
Framework fw;

// Bridge로 Framework클래스와 연결해야 각 객체가 사용자와 통신 가능함
class Human : public Bridge { 
private:
	int type, num, cnt{};

	// layer 변수는 모든 게임 오브젝트들이 반드시 가지고 있어야 함
	// 어느 레이어에 존재하는지 Framework::delete_object() 함수에게 알려줘야하기 때문
	int layer;

	// 객체 삭제 플래그
	// 다른 이름으로도 삭제 플래그를 지정할 수 있으나 'delete_flag'로 지정하는 것을 권장함
	// true가 되면 check_delete_flag()에서 객체 삭제 코드 실행
	bool delete_flag{};

public:
	// 객체 생성 시 반드시 layer 번호를 저장해야함
	Human(int i, int l) {
		layer = l;
		type = i;

		// 랜덤 숫자 지정
		num = uid(dre); 

		if(class_message)
			std::cout << "added Human " << type << std::endl;
	}

	~Human() {
		if (class_message)
			std::cout  << "deleted Human " << type << std::endl << std::endl;
	}

	// 외부에서 객체의 데이터를 얻는 함수
	int get_info() const { return type; }

	// Bridge class로부터 상속받아 해당 클래스에서 재정의
	// 객체 업데이트
	void update() {
		cnt++;

		if (class_message)
			std::cout << "updated Human " << type << "   " << cnt << " times" << std::endl;

		// cnt가 num보다 크면 객체 삭제 플래그가 활성화 됨
		// * 삭제 플래그 활성화는 check_delete_flag()를 제외한 모든 멤버 함수에서 가능함
		if (cnt > num)
			delete_flag = true;
	}

	// Bridge class로부터 상속받아 해당 클래스에서 재정의
	// 객체 랜더링
	void render() {
		if (class_message)
			std::cout << "rendered Human " << type << "   " << cnt << " times" << std::endl << std::endl;
	}

	// Bridge class로부터 상속받아 해당 클래스에서 재정의
	// 충돌 관련 변수는 public으로 설정 할 것을 권장함
    void check_collision() {
		if (class_message)
			std::cout << "checked collision of Human " << type << "   " << cnt << " times" << std::endl;
	}

	// Bridge class로부터 상속받아 해당 클래스에서 재정의
	// 객체 삭제는 이 함수에서 실행해야함
	void check_delete_flag() {
		if (delete_flag)  // 객체 삭제 플래그가 활성화 되었다면 객체 스스로 삭제 실행
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
		// ptr_track()은 특정 레이어의 특정 객체의 인덱스에 대해 포인터가 추적하도록 한다

		if (!class_message) {
			for (int i = 0; i < fw.layer_size(0); ++i) {
				auto ptr = fw.ptr_track(0, i);  // 0번 레이어의 객체들에 대해 포인터 의존
				if (ptr != nullptr)
					std::cout << "ptr got number '" << ptr->get_info() << "' from objects in layer 0" << std::endl;
			}

			for (int i = 0; i < fw.layer_size(1); ++i) {
				auto ptr = fw.ptr_track(1, i);  // 1번 레이어의 객체들에 대해 포인터 의존
				if (ptr != nullptr)
					std::cout << "ptr got number '" << ptr->get_info() << "' from objects in layer 1" << std::endl;
			}

			// 모두 출력했다면 삭제 플래그 활성화
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