#include "Objects.h"
#include "Framework.h"

// 템플릿 예제 구조
// FW_config.h - Framework.h - Objects.h
//                   |             |
//             Framework.cpp    main.cpp

// 프레임워크 생성. 최초 생성 시 extern 키워드를 붙일 것
extern Framework fw;

// true일 시 각 객체 클래스 메시지 출력, false일 시 connect_ptr()관련 메시지 출력 
bool class_message = true;  


int main() {
	// 프레임 워크에 오브젝트 추가
	// 이 예제에서는 fw.add_object(new 객체(객체 번호, 레이어 번호), 레이어 번호) 형식으로 되어있음
	// 낮은 레이어부터, 레이어에 추가된 순서부터 업데이트 함
	
	// 객체 번호는 이 예제에서 객체 구분을 위해 사용하는 변수이며, 실제 사용 시 객체 번호는 없어도 됨
	// 객체 추가 시 반드시 생성자에 레이어 번호를 입력 해야 함
	
	// Monster가 하위 레이어에 있으므로 Monster부터 객체 코드 실행
	fw.add_object(new Human(1, 1), 1);
	fw.add_object(new Human(2, 1), 1);
	fw.add_object(new Human(3, 1), 1);
	fw.add_object(new Monster(1, 0), 0);
	fw.add_object(new Monster(2, 0), 0);
	fw.add_object(new Monster(3, 0), 0);

	// connect_ptr() 설명용 객체
	fw.add_object(new ConnectPtrExample(2), 2);

	std::cout << std::endl;
	
	// 전체 게임 루프
	while (true) {
		fw.routine();
	}
}
