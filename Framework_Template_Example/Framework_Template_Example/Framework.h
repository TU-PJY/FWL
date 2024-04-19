#pragma once
#include <iostream>
#include <deque>
#include <array>
#include <algorithm>
#include <ctime>

// 프레임워크 설정 헤더
#include "FW_config.h"

#ifdef USING_FRAMEWORK
#ifdef NUMBER_OF_LAYER
#ifdef START_MODE

class Framework {
public:
	double    ft{};  // 프레임 타임, 모든 오브젝트 움직임 또는 수치 업데이트에 곱해야 함
	clock_t   start_time{}, end_time{};  // 루틴 실행 소요 시간 측정 변수

	bool framework_enable{};  // 프레임워크 구동 활성화 여부, true가 되어야 routine()함수 실행
	using func = void(*)();  // callable type 함수 인자

	std::array<std::deque<Bridge*>, NUMBER_OF_LAYER> bridge{};


	// 전체 게임 루프 (예: 출력, 이동 등...)
	void routine() {

		// 시작 모드 설정이 완료되어야 루틴 실행 가능
		if (framework_enable) {  

			// 게임 루프 소요시간 측정 시작
			start_time = clock();

			// 하위 레이어부터 순서대로 오브젝트 코드를 실행함
			for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
				for (auto it = bridge[i].begin(); it != bridge[i].end();) {
					auto ptr = *it;

					// 객체가 존재하면 오브젝트 코드를 실행
					if (ptr != nullptr) {
						ptr->update();
						ptr->check_collision();
						ptr->render();

						// 삭제 플래그 검사
						// 반드시 check_delete_flag()를 통해 삭제할 것, 이외의 함수에서 삭제 시 오류가 발생할 수 있음
						ptr->check_delete_flag();

						++it;  // 반복자 증가
					}

					// 객체가 존재하지 않는다면 해당 객체가 있던 벡터 인덱스를 삭제
					else
						it = bridge[i].erase(remove(bridge[i].begin(), bridge[i].end(), ptr));
				}
			}

			// 게임 루프 소요시간 측정 종료
			end_time = clock();

			// 측정 값으로 프레임 타임 계산
			ft = (double)(end_time - start_time) / 1000;
		}
	}


	// 게임 오브젝트 추가
	void add_object(Bridge*&& object, int layer) {
		bridge[layer].push_back(object);
	}


	// 특정 오브젝트에 대한 포인터 연결 설정
	// 특정 레이어, 특정 번호의 인덱스에 포인터를 연결한다
	// ex) auto ptr = fw.connect_ptr(2, 3);
	Bridge* connect_ptr(int layer, int index) {

		// 존재하지 않는 레이어에 연결할 경우 nullptr을 리턴한다
		if (index >= bridge[layer].size())
			return nullptr;
		else
			return bridge[layer][index];
	}


	// 특정 레이어에 존재하는 오브젝트 수 리턴
	int layer_size(int layer) {
		return bridge[layer].size();
	}


	// 게임 오브젝트 삭제
	void delete_object(Bridge* object, int layer) {

		// 게임 오브젝트가 정말로 존재하는지 확인
		auto target = std::find(bridge[layer].begin(), bridge[layer].end(), object);

		// 객체가 존재하는 것으로 판단되면 삭제 코드 실행
		if (target != bridge[layer].end()) {
			// 오브젝트 삭제
			delete* target;

			// 오브젝트는 더 이상 존재하지 않음
			// 남은 프레임워크 인덱스는 routine() 함수에서 삭제함
			*target = nullptr;
		}
	}


	// 특정 레이어의 모든 게임 오브젝트 삭제
	void sweep_layer(int layer) {
		for (auto it = bridge[layer].begin(); it != bridge[layer].end();) {
			auto target = std::find(bridge[layer].begin(), bridge[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}


	// 모든 게임 오브젝트 삭제
	void sweep_all() {
		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			for (auto it = bridge[i].begin(); it != bridge[i].end();) {
				auto target = std::find(bridge[i].begin(), bridge[i].end(), *it);
				
				delete* target;
				*target = nullptr;

				++it;
			}
		}
	}


	// 시작 모드 설정
	void init_start_mode(func modefunc) {
		modefunc();
		framework_enable = true;  // 시작 모드 설정이 완료되면 프레임워크 루틴 시작
	}


	// 모드 변경
	void change_mode(func modefunc) {
		sweep_all();  // 모드 전환 시 기존의 객체를 모두 삭제한 후 모드 변경

		framework_enable = false;  // 프레임워크 일시정지
		modefunc();  // 모드 시작 코드 실행
		framework_enable = true;  // 프레임워크 재개
	}
};

#endif
#endif
#endif