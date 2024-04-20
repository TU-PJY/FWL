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
	std::array<std::deque<Bridge*>, NUMBER_OF_LAYER> bridge{};  // NUMBER_OF_LAYER 만큼 레이어가 생성
	std::array<std::deque<Bridge*>, NUMBER_OF_LAYER> temp_bridge{};  // 모드 전환 시 사용하는 임시 컨테이너


	#ifdef USING_POPUP_MODE
	#ifdef NUMBER_OF_LAYER_POPUP

	std::array<std::deque<POP_Bridge*>, NUMBER_OF_LAYER_POPUP> pop_bridge{};  // NUMBER_OF_LAYER_POPUP 만큼 레이어가 생성
	bool popup_mode_enable{}; // 팝업모드 활성화 여부, true일 시 팝업 모드 구동

	#endif
	#endif


	// framework_enable: 프레임워크 구동 활성화 여부, true가 되어야 routine()함수 실행
	// framework_pause: 프레임워크 일시정지 여부, true일 시 일시정지
	bool framework_enable{}, framework_pause{};
	std::string mode_name{}, prev_mode_name{};  // 현재 실행 중인 모드 이름, 이전까지 실행했던 모드 이름

	double    ft{};  // 프레임 타임, 모든 오브젝트 움직임 또는 수치 업데이트에 곱해야 함
	clock_t   start_time{}, end_time{};  // 루틴 실행 소요 시간 측정 변수

	using func = void(*)();   // callable type 함수 인자


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

						// 프레임워크 일시정지 시 업데이트 정지
						if (!framework_pause) {
							ptr->update();
							ptr->check_collision();
						}
						ptr->render();

						// 삭제 플래그 검사
						// 반드시 check_delete_flag()를 통해 삭제할 것, 이외의 함수에서 삭제 시 오류가 발생할 수 있음
						ptr->check_delete_flag();

						++it;  // 반복자 증가
					}

					// 객체가 존재하지 않는다면 해당 객체가 있던 인덱스를 삭제
					else
						it = bridge[i].erase(remove(bridge[i].begin(), bridge[i].end(), ptr));
				}
			}

			#ifdef USING_POPUP_MODE
			#ifdef NUMBER_OF_LAYER_POPUP

			// 팝업 모드 활성화 시 팝업 모드 업데이트
			if (popup_mode_enable) {
				for (int i = 0; i < NUMBER_OF_LAYER_POPUP; ++i) {
					for (auto it = pop_bridge[i].begin(); it != pop_bridge[i].end();) {
						auto ptr = *it;

						if (ptr != nullptr) {
							ptr->update();
							ptr->check_collision();
							ptr->render();
							ptr->check_delete_flag();

							++it;
						}

						else
							it = pop_bridge[i].erase(remove(pop_bridge[i].begin(), pop_bridge[i].end(), ptr));
					}
				}
			}

			#endif
			#endif


			// 게임 루프 소요시간 측정 종료
			end_time = clock();

			// 측정 값으로 프레임 타임 계산
			ft = (double)(end_time - start_time) / 1000;
		}
	}



	// 시작 모드 설정
	void init_start_mode(func modefunc, std::string modename) {
		if (framework_enable)
			return;

		modefunc();

		mode_name = modename;
		framework_enable = true;  // 시작 모드 설정이 완료되면 프레임워크 루틴 시작
	}


	// 게임 오브젝트 추가, 모드 전환 시 임시 컨테이너에 추가된다
	void add_object(Bridge*&& object, int layer) {
		if (popup_mode_enable)
			return;

		if (framework_pause)
			temp_bridge[layer].push_back(object);

		else
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
	size_t layer_size(int layer) {
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
			// 남은 인덱스는 routine() 함수에서 삭제함
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


	// 모드 변경
	void change_mode(func modefunc, std::string modename) {
		if (mode_name == modename)
			return;

		// 프레임워크가 일시정지된다
		framework_pause = true;

		// 모드 시작 함수를 실행한다
		modefunc();

		// 임시 컨테이너에 객체 추가 및 기타 작업이 완료되면 메인 컨테이너의 객체들을 모두 삭제한다
		sweep_all();

		// 임시 컨테이너의 객체들을 메인 컨테이너로 복사하고, 임시 컨테이너의 인덱스를 비운다
		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			bridge[i] = temp_bridge[i];
			temp_bridge[i] = {};
		}

		mode_name = modename;

		// 프레임워크가 재개된다
		framework_pause = false;
	}





	#ifdef USING_POPUP_MODE
	#ifdef NUMBER_OF_LAYER_POPUP

	// 팝업 모드 실행, 반드시 팝업 모드를 위한 별도의 레이어가 있어야 함
	// 한 개의 팝업 모드만 실행 가능함
	void init_popup_mode(func modefunc, std::string modename, bool pause_option = false) {
		if (popup_mode_enable)
			return;

		modefunc();
		prev_mode_name = mode_name;  // 최근 실행한 모드 이름을 저장
		mode_name = modename;

		// 프레임워크 일시정지 옵션 설정 시 팝업모드를 제외한 나머지 모드는 일시정지 됨
		if (pause_option)
			framework_pause = true;

		popup_mode_enable = true;
	}


	// 팝업 모드 객체 추가
	void add_popup_object(POP_Bridge*&& object, int layer) {
		pop_bridge[layer].push_back(object);
	}


	// 팝업 모드 객체 삭제
	void delete_popup_object(POP_Bridge* object, int layer) {

		// 게임 오브젝트가 정말로 존재하는지 확인
		auto target = std::find(pop_bridge[layer].begin(), pop_bridge[layer].end(), object);

		// 객체가 존재하는 것으로 판단되면 삭제 코드 실행
		if (target != pop_bridge[layer].end()) {
			// 오브젝트 삭제
			delete* target;

			// 오브젝트는 더 이상 존재하지 않음
			// 남은 인덱스는 routine() 함수에서 삭제함
			*target = nullptr;
		}
	}


	// 특정 팝업 레이어에 존재하는 오브젝트 수 리턴
	size_t popup_layer_size(int layer) {
		return pop_bridge[layer].size();
	}


	// 특정 팝업 레이어에 존재하는 객체에 포인터 연결
	POP_Bridge* connect_popup_ptr(int layer, int index) {

		// 존재하지 않는 레이어에 연결할 경우 nullptr을 리턴한다
		if (index >= pop_bridge[layer].size())
			return nullptr;
		else
			return pop_bridge[layer][index];
	}


	// 특정 팝업 레이어 객체 삭제
	void sweep_popup_layer(int layer) {
		if (!popup_mode_enable)
			return;

		for (auto it = pop_bridge[layer].begin(); it != pop_bridge[layer].end();) {
			auto target = std::find(pop_bridge[layer].begin(), pop_bridge[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}


	// 모든 팝업 모드 객체 제거
	void sweep_popup_all() {
		if (!popup_mode_enable)
			return;

		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			for (auto it = pop_bridge[i].begin(); it != pop_bridge[i].end();) {
				auto target = std::find(pop_bridge[i].begin(), pop_bridge[i].end(), *it);

				delete* target;
				*target = nullptr;

				++it;
			}
		}
	}


	// 팝업 모드 끝내기
	void close_popup_mode() {
		if (!popup_mode_enable)
			return;

		// 저장된 팝업 모드의 모든 객체 삭제
		sweep_popup_all();

		mode_name = prev_mode_name;

		framework_pause = false;
		popup_mode_enable = false;
	}

	#endif
	#endif
};

#endif
#endif
#endif