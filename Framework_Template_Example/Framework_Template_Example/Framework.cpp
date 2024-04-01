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
	for(int i = 0; i < framework.size(); i ++) {

		// 한 레이어를 방문할 때마다 레이어가 가지는 벡터에 필요 이상으로 메모리가 할당되었는지 확인
		// 메모리 할당량을 최소한으로 줄인다.
		if(framework[i].size() < framework[i].capacity())
			framework[i].shrink_to_fit();

		for (auto it = framework[i].begin(); it != framework[i].end();) {
			auto& ptr = *it;

			// 객체가 존재하면 오브젝트 코드를 실행
			if (ptr != nullptr) {
				ptr->render();
				ptr->check_collision();

				// 각 객체의 update() 함수에서 fw_delete() 함수가 실행되기 때문에 반드시 가장 마지막에 실행되어야 함
				// 삭제된 객체 참조를 방지하기 위함
				ptr->update();	

				++it;  // 반복자 증가
			}

			// 객체가 존재하지 않는다면 해당 객체가 있던 벡터 인덱스를 삭제
			// 뒤의 모든 인덱스들이 앞으로 밀리기 때문에 삭제 후 자동으로 다음 인덱스 순서가 됨
			else 
				it = framework[i].erase(it);
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
		auto& ptr = *it;
		auto target = std::find(framework[layer].begin(), framework[layer].end(), ptr);

		delete* target;
		*target = nullptr;

		it++;
	}
}


// 모든 게임 오브젝트 삭제
void fw_sweep_all() {
	for (int i = 0; i < framework.size(); i++) {
		for (auto it = framework[i].begin(); it != framework[i].end();) {
			auto& ptr = *it;
			auto target = std::find(framework[i].begin(), framework[i].end(), ptr);

			delete* target;
			*target = nullptr;

			it++;
		}
	}
}
