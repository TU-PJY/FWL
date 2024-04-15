#include "Framework.h"


// 프레임워크 벡터 배열
// Framework.h에 설정한 LAYER 만큼 프레임워크 벡터 배열 생성
std::array<std::vector<Framework*>, LAYER> framework;

clock_t start_time, end_time;  // 게임 루프에 소요되는 시간 측정
double ft;                     // 프레임 타임


// 전체 게임 루프 (예: 출력, 이동 등...)
void fw_routine() {

    // 게임 루프 소요시간 측정 시작
	start_time = clock();  	

	// 하위 레이어부터 순서대로 오브젝트 코드를 실행함
	for(int i = 0; i < LAYER; ++i) {

		// 메모리가 과하게 할당되어있을 경우 할당량 최적화
		// 옵션 활성화 시에만 실행
		#ifdef OPT_OPTIMIZING
		if (framework[i].size() * 2 < framework[i].capacity())
			framework[i].shrink_to_fit();
		#endif

		for (auto it = framework[i].begin(); it != framework[i].end();) {
			auto& ptr = *it;

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
			// 뒤의 모든 인덱스들이 앞으로 밀리기 때문에 삭제 후 자동으로 다음 인덱스 순서가 됨
			else
				it = framework[i].erase(remove(framework[i].begin(), framework[i].end(), ptr));
		}
	}

	// 게임 루프 소요시간 측정 종료
	end_time = clock();  

	// 측정 값으로 프레임 타임 계산
	ft = (double)(end_time - start_time) / 1000;
}


// 게임 오브젝트 추가
void fw_add(Framework*&& object, int layer) {
	framework[layer].push_back(object);
}


// 다른 레이어에 존재하는 오브젝트 인덱스 추적 설정
// 특정 레이어의 특정 번호의 인덱스만 고정적으로 추적한다.
Framework* fw_set_tracking(int layer, int index) {

	// 존재하지 않는 레이어를 추적 설정할 경우 nullptr을 리턴한다
	if (index >= framework[layer].size())
		return nullptr;
	else
		return framework[layer][index];
}


// 다른 레이어에 존재하는 오브젝트 인덱스 추적 유효성 검사
bool fw_check_tracking_valid(int layer, int index) {

	// 더 이상 가리킬 특정 레이어의 인덱스가 존재하지 않으면 false, 존재하면 true를 리턴한다
	if (index >= framework[layer].size())
		return false;
	else
		return true;
}


// 특정 레이어에 존재하는 오브젝트 수 리턴
int fw_layer_size(int layer) {
	return framework[layer].size();
}


// 게임 오브젝트 삭제
void fw_delete(Framework* object, int layer) {

	// 게임 오브젝트가 정말로 존재하는지 확인
	auto target = std::find(framework[layer].begin(), framework[layer].end(), object);

	// 객체가 존재하는 것으로 판단되면 삭제 코드 실행
	if (target != framework[layer].end()) {
		// 오브젝트 삭제
		delete* target; 

		// 오브젝트는 더 이상 존재하지 않음
		// 남은 프레임워크 인덱스는 fw_routine() 함수에서 삭제함
		*target = nullptr;
	}
} 


// 특정 레이어의 모든 게임 오브젝트 삭제
void fw_sweep_layer(int layer) {
	for (auto it = framework[layer].begin(); it != framework[layer].end();) {
		auto target = std::find(framework[layer].begin(), framework[layer].end(), *it);

		delete* target;
		*target = nullptr;

		++it;
	}
}


// 모든 게임 오브젝트 삭제
void fw_sweep_all() {
	for (int i = 0; i < LAYER; i++) {
		for (auto it = framework[i].begin(); it != framework[i].end();) {
			auto target = std::find(framework[i].begin(), framework[i].end(), *it);

			delete* target;
			*target = nullptr;

			++it;
		}
	}
}
