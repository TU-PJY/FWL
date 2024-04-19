#include "Framework.h"
#include "modes.h"

// true일 시 각 객체 클래스 메시지 출력, false일 시 connect_ptr()관련 메시지 출력 
bool class_message = true;  

// true일 시 하단의 모드 변경 코드 실행
bool mode_change = false;

// 프레임워크 최초 생성
Framework fw;

int main() {
	// 함수를 인자로 받음. 실행하고자 하는 코드들을 함수로 작성하여 인자에 전달
	// 모드 시작 코드 실행
	// 모드 시작 코드는 modes.cpp에 있음
	if(std::string(START_MODE) == "example_mode")
		fw.init_start_mode(example_mode, "example_mode");

	// mode_change 활성화 시 example_mode에서 추가되었던 객체가 즉시 모두 삭제되고 변경된 모드 시작 코드에 있는 메시지가 출력된다
	// 실행 모드 이름과 실행 함수 이름이 같도록 작성하는 것을 권장함
	if(mode_change)
		fw.change_mode(other_mode, "other_mode");

	// 전체 게임 루프
	while (true) {
		fw.routine();
	}
}
