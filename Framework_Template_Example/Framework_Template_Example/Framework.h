#pragma once
#include "Header.h"
#define LAYER 3  // 레이어 개수를 이곳에서 설정


// 프레임워크 클래스 및 관련 변수, 함수
// 클래스 자체는 아무 역할도 하지 않으며, 게임 오브젝트들을 담아 한꺼번에 객체 코드를 실행하는 컨테이너 역할을 함
class Framework {
public:
	// 아래의 함수들은 반드시 게임 오브젝트 클래스들이 상속 받도록 해야함
	virtual void render() {}          // 게임 오브젝트 출력
	virtual void check_collision() {} // 게임 오브젝트 충돌 처리
	virtual void update() {}          // 게임 오브젝트 업데이트

	virtual ~Framework() {}           //소멸자, Framework를 통해 객체의 소멸자를 호출함
};


extern clock_t start_time, end_time;  // 게임 루프에 소요되는 시간 측정
extern double ft;                     // 프레임 타임

// 프레임워크 관련 함수는 fw_function() 형태 사용
void fw_routine();                             // 전체 게임 루프

void fw_add(Framework*&& object, int layer);   // 게임 오브젝트 추가
Framework* fw_set_tracking(int layer, int index);
bool fw_check_tracking_valid(int layer, int index);
int fw_layer_size(int layer);
void fw_delete(Framework* object, int layer);  // 게임 오브젝트 삭제
void fw_sweep_layer(int layer);                // 특정 레이어의 모든 게임 오브젝트 삭제
void fw_sweep_all();                               // 모든 게임 오브젝트 삭제
