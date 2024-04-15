#pragma once
#include "Framework.h"
// 게임 오브젝트 클래스 예제

// 모든 객체는 update(), check_collision(), render(), check_delete_flag()를 Framework class로부터 상속 받아야 함
// 아래의 코드들은 예제 코드임

std::default_random_engine dre;
std::uniform_int_distribution<int> uid{ 1, 10 };


class Human : public Framework { 
private:
	int type, num, cnt{};

	// layer 변수는 모든 게임 오브젝트들이 반드시 가지고 있어야 함
	// 어느 레이어에 존재하는지 fw_delete() 함수에게 알려줘야하기 때문
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

		std::cout << "added Human " << type << std::endl;
	}

	~Human() {
		std::cout  << "deleted Human " << type << std::endl << std::endl;
	}


	// Framework class로부터 상속받아 해당 클래스에서 재정의
	// 객체 업데이트
	void update() {
		cnt++;

		std::cout << "updated Human " << type << "   " << cnt << " times" << std::endl;

		// cnt가 num보다 크면 객체 삭제 플래그가 활성화 됨
		// * 삭제 플래그 활성화는 check_delete_flag()를 제외한 모든 객체 함수에서 가능함
		if (cnt > num)
			delete_flag = true;
	}

	// Framework class로부터 상속받아 해당 클래스에서 재정의
	// 객체 랜더링
	void render() {
		std::cout << "rendered Human " << type << "   " << cnt << " times" << std::endl << std::endl;
	}

	// Framework class로부터 상속받아 해당 클래스에서 재정의
	// 충돌 관련 변수는 public으로 설정 할 것을 권장함
    void check_collision() {
		std::cout << "checked collision of Human " << type << "   " << cnt << " times" << std::endl;
	}

	// Framework class로부터 상속받아 해당 클래스에서 재정의
	// 객체 삭제는 이 함수에서 실행해야함
	void check_delete_flag() {
		if (delete_flag)  // 객체 플래그가 활성화 되었다면 객체 스스로 삭제 실행
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
