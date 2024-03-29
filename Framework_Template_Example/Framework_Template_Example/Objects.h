#pragma once
#include "Framework.h"
// 게임 오브젝트 클래스 예제

// 모든 클래스는 render(), check_collision(), update() 함수를 Framework class로부터 상속 받아야 함
// 아래의 코드들은 예제 코드임

std::default_random_engine dre;
std::uniform_int_distribution<int> uid{ 1, 10 };


class Human : public Framework { 
private:
	int type, cnt, num;

	// layer 변수는 모든 게임 오브젝트들이 반드시 가지고 있어야 함
	// 어느 레이어에 존재하는지 fw_delete() 함수에게 알려줘야하기 때문
	int layer;

public:
	bool collision;  // 충돌 시 true

	// 객체 생성 시 반드시 layer 번호를 저장해야함
	Human(int i, int l) {
		layer = l;
		type = i;
		cnt = 0;

		// 랜덤 숫자 지정
		num = uid(dre); 

		// 충돌 여부는 항상 false로 설정할 것
		collision = false;

		std::cout << "added Human " << type << std::endl;
	}

	// Framework class로부터 상속받아 해당 클래스에서 재정의
	virtual void render() {
		std::cout << "updated Human " << type << "   " << cnt << " times" << std::endl;
	}

	// Framework class로부터 상속받아 해당 클래스에서 재정의
	// 충돌 관련 변수는 public으로 설정 할 것을 권장함
    virtual void check_collision() {
		std::cout << "checked collision of Human " << type << "   " << cnt << " times" << std::endl << std::endl;
	}

	// Framework class로부터 상속받아 해당 클래스에서 재정의
	virtual void update() {
		cnt++;

		Sleep(100);

		// cnt가 num보다 크면 객체 스스로를 삭제
		// 특정 조건에 객체 스스로를 삭제한다.
		// 외부 함수에서 삭제하는 것이기 때문에 파괴자가 실행되지 않음에 유의
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
