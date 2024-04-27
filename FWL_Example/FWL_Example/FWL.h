#ifndef FWL_H
#define FWL_H

#include <algorithm>
#include <iostream>
#include <string>
#include <array>
#include <deque>
#include <vector>
#include <ctime>

// 프레임워크 설정 헤더
#include "FWL_config.h"

#ifdef USING_FWL
#if NUMBER_OF_LAYER
#ifdef START_MODE

class FWL {
public:
	std::array<std::deque<FUNCTION*>, NUMBER_OF_LAYER> main_cont{};  // NUMBER_OF_LAYER 만큼 레이어가 생성
	std::array<std::deque<FUNCTION*>, NUMBER_OF_LAYER> temp_cont{};  // 모드 전환 시 사용하는 임시 컨테이너

	std::vector<std::string> mode_list;


	#ifdef USING_POPUP_MODE
	#if NUMBER_OF_POPUP_LAYER

	std::array<std::deque<POPUP_FUNCTION*>, NUMBER_OF_POPUP_LAYER> popup_cont{};  // NUMBER_OF_POPUP_LAYER 만큼 레이어가 생성
	std::array<std::deque<POPUP_FUNCTION*>, NUMBER_OF_POPUP_LAYER> temp_popup_cont{};
	
	std::vector<std::string> popup_mode_list;
	bool popup_mode_initialization{}; // 팝업모드 활성화 여부, true일 시 팝업 모드 구동

	#endif
	#endif


	bool framework_initialization{};  // 프레임워크 초기화 여부, true일 시 초기화 완료

	bool framework_start{}; // 프레임워크 시작 여부
	bool framework_pause{}; // framework_pause: 프레임워크 일시정지 여부, true일 시 일시정지

	bool framework_changing_mode{}; // 모드 변경 여부

	std::string mode_name{}, prev_mode_name{};  // 현재 실행 중인 모드 이름, 이전까지 실행했던 모드 이름
	std::string mem_mode_name{};

	double    ft{};  // 프레임 타임, 모든 오브젝트 움직임 또는 수치 업데이트에 곱해야 함
	clock_t   start_time{}, end_time{};  // 루틴 실행 소요 시간 측정 변수

	typedef void (*func)(void); // callable type 함수 인자


	// 전체 게임 루프 (예: 출력, 이동 등...)
	void routine() {

		// 시작 모드 설정이 완료되어야 루틴 실행 가능
		if (framework_initialization) {

			// 게임 루프 소요시간 측정 시작
			start_time = clock();

			// 하위 레이어부터 순서대로 오브젝트 코드를 실행함
			for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
				for (auto it = main_cont[i].begin(); it != main_cont[i].end();) {
					auto ptr = *it;

					// 객체가 존재하면 오브젝트 코드를 실행
					if (ptr != nullptr) {

						// 프레임워크 일시정지 모드 변경 중에는 업데이트 정지
						if (!framework_pause && !framework_changing_mode) {
							ptr->update();
							ptr->check_collision();
						}
						ptr->render();

						// 삭제 플래그 검사
						// 반드시 check_delete()를 통해 삭제할 것, 이외의 함수에서 삭제 시 오류가 발생할 수 있음
						ptr->check_delete();

						++it;  // 반복자 증가
					}

					// 객체가 존재하지 않는다면 해당 객체가 있던 인덱스를 삭제
					else
						it = main_cont[i].erase(remove(main_cont[i].begin(), main_cont[i].end(), ptr));
				}
			}

			#ifdef USING_POPUP_MODE
			#if NUMBER_OF_POPUP_LAYER

			// 팝업 모드 활성화 시 팝업 모드 업데이트
			if (popup_mode_initialization) {
				for (int i = 0; i < NUMBER_OF_POPUP_LAYER; ++i) {
					for (auto it = popup_cont[i].begin(); it != popup_cont[i].end();) {
						auto ptr = *it;

						if (ptr != nullptr) {
							if (!framework_changing_mode) {
								ptr->update();
								ptr->check_collision();
							}
							ptr->render();
							ptr->check_delete();

							++it;
						}

						else
							it = popup_cont[i].erase(remove(popup_cont[i].begin(), popup_cont[i].end(), ptr));
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



	// 프레임워크 시작
	void init(func startmode, std::string modename) {
		if (framework_initialization)
			process_err("FWL init error::Reapeted initialization", "");

		framework_start = true;

		MODELIST m;

		// FW_config에 입력한 모드 목록을 프레임워크에 저장한다
		mode_list = m.mode_list;

		#ifdef USING_POPUP_MODE
		#if NUMBER_OF_POPUP_LAYER

		popup_mode_list = m.popup_mode_list;

		#endif
		#endif

		// 인자로 입력한 모드 이름이 모드 목록에 없을경우 실행하지 않는다.
		auto target = std::find(mode_list.begin(), mode_list.end(), modename);
		if (target == mode_list.end())
			process_err("FWL init error::Invalid mode", modename);

		mem_mode_name = modename;

		startmode();
		mode_name = modename;

		framework_initialization = true;  // 시작 모드 설정이 완료되면 프레임워크 루틴 시작
	}


	// 모드 변경
	void change_mode(func modefunc, std::string modename) {
		if (!framework_initialization)
			process_err("FWL init error::Invalid initialization");

		if (mode_name == modename)
			process_err("FWL mode error::Same mode as current mode");

		// 인자로 입력한 모드 이름이 모드 목록에 없을경우 실행하지 않는다.
		auto target = std::find(mode_list.begin(), mode_list.end(), modename);
		if (target == mode_list.end())
			process_err("FWL mode error::Invalid mode", modename);

		mem_mode_name = modename;

		// 프레임워크가 모드 변경 상황이 되어 일시정지된다
		framework_changing_mode = true;

		// 모드 시작 함수를 실행한다
		modefunc();

		// 팝업모드가 활성화 되어있을경우 팝업모드를 비활성화 후 모드 전환한다
		#ifdef USING_POPUP_MODE
		#if NUMBER_OF_POPUP_LAYER

		if (popup_mode_initialization)
			close_popup_mode();

		#endif
		#endif
		
		// 메인 컨테이너를 비우고나서 임시 컨테이너의 객체들을 메인 컨테이너로 복사하고, 임시 컨테이너의 인덱스를 비운다
		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			sweep_layer(i);
			main_cont[i] = temp_cont[i];
			temp_cont[i].clear();
		}

		mode_name = modename;

		// 프레임워크가 재개된다
		framework_changing_mode = false;
	}


	// 게임 오브젝트 추가, 모드 전환 시 임시 컨테이너에 추가된다
	void add_object(FUNCTION*&& object, int layer) {
		if (!framework_start)
			process_err("FWL init error::Invalid initialization");

		if (framework_changing_mode) {
			if (layer >= NUMBER_OF_LAYER || layer < 0)
				process_err("FWL object addition error::Layer out of bounds::temp container");

			temp_cont[layer].push_back(object);
		}

		else {
			if (layer >= NUMBER_OF_LAYER || layer < 0) {
				if(framework_initialization)
					process_err("FWL object addition error::Layer out of bounds::main container");
				else
					process_err("FWL init error::Layer out of bounds::main container");
			}

			main_cont[layer].push_back(object);
		}
	}


	// 게임 오브젝트 삭제
	void delete_object(FUNCTION* object, int layer) {
		if (!framework_initialization)
			process_err("FWL init error::Invalid initialization");

		if (layer >= NUMBER_OF_LAYER || layer < 0)
			process_err("FWL object deletion error::Layer out of bounds");

		// 게임 오브젝트가 정말로 존재하는지 확인
		auto target = std::find(main_cont[layer].begin(), main_cont[layer].end(), object);

		// 객체가 존재하는 것으로 판단되면 삭제 코드 실행
		if (target != main_cont[layer].end()) {
			// 오브젝트 삭제
			delete* target;

			// 오브젝트는 더 이상 존재하지 않음
			// 남은 인덱스는 routine() 함수에서 삭제함
			*target = nullptr;
		}

		else 
			process_err("FWL object deletion error::Can not find object");
	}


	// 특정 오브젝트에 대한 포인터 연결 설정
	// 특정 레이어, 특정 번호의 인덱스에 포인터를 연결한다
	// ex) auto ptr = fw.connect_ptr(2, 3);
	FUNCTION* connect_ptr(int layer, int index) {
		if (!framework_initialization)
			process_err("FWL init error::Invalid initialization");

		if (layer >= NUMBER_OF_LAYER || layer < 0)
			process_err("FWL ptr error::Layer out of bounds");

		// 존재하지 않는 레이어에 연결할 경우 nullptr을 리턴한다
		if (index >= main_cont[layer].size())
			return nullptr;
		else
			return main_cont[layer][index];
	}


	// 특정 레이어에 존재하는 오브젝트 수 리턴
	size_t layer_size(int layer) {
		if (!framework_initialization)
			process_err("FWL init error::Invalid initialization");

		if (layer >= NUMBER_OF_LAYER || layer < 0)
			process_err("FWL layer size error::Layer out of bounds");

		return main_cont[layer].size();
	}


	// 특정 레이어의 모든 게임 오브젝트 삭제
	void sweep_layer(int layer) {
		if (!framework_initialization)
			process_err("FWL init error::Invalid initialization");

		if (layer >= NUMBER_OF_LAYER || layer < 0)
			process_err("FWL layer sweep error::Layer out of bounds");

		for (auto it = main_cont[layer].begin(); it != main_cont[layer].end();) {
			auto target = std::find(main_cont[layer].begin(), main_cont[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}


	// 모든 게임 오브젝트 삭제
	void sweep_all() {
		if (!framework_initialization)
			process_err("FWL init error::Invalid initialization");

		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			for (auto it = main_cont[i].begin(); it != main_cont[i].end();) {
				auto target = std::find(main_cont[i].begin(), main_cont[i].end(), *it);
				
				delete* target;
				*target = nullptr;

				++it;
			}
		}
	}



	#ifdef USING_POPUP_MODE
	#if NUMBER_OF_POPUP_LAYER

	// 팝업 모드 실행
	// 한 번에 한 개의 팝업 모드만 실행 가능함
	void init_popup_mode(func modefunc, std::string modename, bool pause_option = false) {
		if (popup_mode_initialization)
			return;

		// 인자로 입력한 모드 이름이 모드 목록에 없을경우 실행하지 않는다.
		auto target = std::find(popup_mode_list.begin(), popup_mode_list.end(), modename);
		if (target == popup_mode_list.end())
			return;

		modefunc();
		prev_mode_name = mode_name;  // 최근 실행한 모드 이름을 저장
		mode_name = modename;

		// 프레임워크 일시정지 옵션 설정 시 팝업모드를 제외한 나머지 모드는 일시정지 됨
		if (pause_option)
			framework_pause = true;

		popup_mode_initialization = true;
	}

	// 팝업 모드 전환
	// 팝업 모드를 다른 팝업 모드로 전환한다
	void change_popup_mode(func modefunc, std::string modename) {
		if (!popup_mode_initialization)
			return;

		// 모드 리스트에서 인자로 받은 이름을 찾지 못하면 실행하지 않는다
		auto target = std::find(popup_mode_list.begin(), popup_mode_list.end(), modename);
		if (target == popup_mode_list.end())
			return;

		framework_changing_mode = true;

		modefunc();

		for (int i = 0; i < NUMBER_OF_POPUP_LAYER; ++i) {
			sweep_popup_layer(i);
			popup_cont[i] = temp_popup_cont[i];
			temp_popup_cont[i].clear();
		}

		mode_name = modename;

		framework_changing_mode = false;
	}


	// 팝업 모드 끝내기
	void close_popup_mode() {
		if (!popup_mode_initialization)
			return;

		// 저장된 팝업 모드의 모든 객체 삭제
		sweep_popup_all();

		mode_name = prev_mode_name;

		framework_pause = false;
		popup_mode_initialization = false;
	}


	// 팝업 모드 객체 추가
	// 팝업 모드 변경 시 임시 컨테이너에 추가된다
	void add_popup_object(POPUP_FUNCTION*&& object, int layer) {
		if (framework_changing_mode)
			temp_popup_cont[layer].push_back(object);
		else
			popup_cont[layer].push_back(object);
	}


	// 팝업 모드 객체 삭제
	void delete_popup_object(POPUP_FUNCTION* object, int layer) {

		// 게임 오브젝트가 정말로 존재하는지 확인
		auto target = std::find(popup_cont[layer].begin(), popup_cont[layer].end(), object);

		// 객체가 존재하는 것으로 판단되면 삭제 코드 실행
		if (target != popup_cont[layer].end()) {
			// 오브젝트 삭제
			delete* target;

			// 오브젝트는 더 이상 존재하지 않음
			// 남은 인덱스는 routine() 함수에서 삭제함
			*target = nullptr;
		}
	}


	// 특정 팝업 레이어에 존재하는 오브젝트 수 리턴
	size_t popup_layer_size(int layer) {
		return popup_cont[layer].size();
	}


	// 특정 팝업 레이어에 존재하는 객체에 포인터 연결
	POPUP_FUNCTION* connect_popup_ptr(int layer, int index) {

		// 존재하지 않는 레이어에 연결할 경우 nullptr을 리턴한다
		if (index >= popup_cont[layer].size())
			return nullptr;
		else
			return popup_cont[layer][index];
	}


	// 특정 팝업 레이어 객체 삭제
	void sweep_popup_layer(int layer) {
		for (auto it = popup_cont[layer].begin(); it != popup_cont[layer].end();) {
			auto target = std::find(popup_cont[layer].begin(), popup_cont[layer].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}


	// 모든 팝업 모드 객체 제거
	void sweep_popup_all() {
		for (int i = 0; i < NUMBER_OF_LAYER; ++i) {
			for (auto it = popup_cont[i].begin(); it != popup_cont[i].end();) {
				auto target = std::find(popup_cont[i].begin(), popup_cont[i].end(), *it);

				delete* target;
				*target = nullptr;

				++it;
			}
		}
	}

	#endif
	#endif


	// 잘못된 프레임워크 사용 에러 처리
	void process_err(std::string err_type, std::string invalid_mode = "") {
		#ifdef USING_DEBUG_MESSEGE

		std::cout << std::endl << "[ERROR OCCURED]" << std::endl << std::endl;
		std::cout << "Error type: " << err_type << std::endl << std::endl;

		// init error
		if (err_type == "FWL init error::Reapeted init")
			std::cout << "Repeated FWL initialization attempt has been detected." << std::endl << std::endl;

		else if (err_type == "FWL init error::Invalid mode") {
			std::cout << "Invalid mode initialization attempt has been detected." << std::endl << std::endl;
			std::cout << "Invalid mode: '" << invalid_mode << "'" << std::endl;
		}

		else if (err_type == "FWL init error::Layer out of bounds::main container") {
			std::cout << "Addition of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While start mode: '" << mem_mode_name << "'" << std::endl;
		}

		else if (err_type == "FWL init error::Invalid initialization") {
			std::cout << "FWL is not prepared yet." << std::endl;
			std::cout << "Execute FWL's function 'init(func startmode, std::string modename)' first." << std::endl;
		}


		// mode error
		else if (err_type == "FWL mode error::Invalid mode") {
			std::cout << "Invalid mode change attempt has been detected." << std::endl << std::endl;
			std::cout << "While mode change: '" << mode_name << "' to invalid mode '" << invalid_mode << "'" << std::endl << std::endl;
			std::cout << "[List of valid modes]" << std::endl;

			for (auto& s : mode_list)
				std::cout << "" << s << "" << std::endl;
		}

		else if (err_type == "FWL mode error::Same mode as current mode") {
			std::cout << "An attempt to change to the same mode as the current mode has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}


		// object addition error
		else if (err_type == "FWL object addition error::Layer out of bounds::temp container") {
			std::cout << "Addition of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While mode change: '" << mode_name << "' to '" << mem_mode_name << "'" << std::endl;
		}

		else if (err_type == "FWL object addition error::Layer out of bounds::main container") {
			std::cout << "Addition of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}


		// object deletion error
		else if (err_type == "FWL object deletion error::Layer out of bounds") {
			std::cout << "Deletion of object attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}
		
		else if (err_type == "FWL object deletion error::Can not find object") {
			std::cout << "Deletion of unknown object attempt has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}


		// ptr error
		else if (err_type == "FWL ptr error::Layer out of bounds") {
			std::cout << "Connection of ptr attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}


		// layer size errpr
		else if (err_type == "FWL layer size error::Layer out of bounds") {
			std::cout << "Getting layer size attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}


		// sweep layer error
		else if (err_type == "FWL layer sweep error::Layer out of bounds") {
			std::cout << "Layer sweep attempt out of layer bounds has been detected." << std::endl << std::endl;
			std::cout << "While mode: '" << mode_name << "'" << std::endl;
		}

		#endif

		exit(1);
	}

};

#endif
#endif
#endif

#endif