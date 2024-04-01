#include "Objects.h"  // Framework.h를 포함

// 템플릿 예제 구조
// Header.h - Framework.h - Objects.h
//                 |             |
//           Framework.cpp    main.cpp

int main() {
	// 프레임 워크에 오브젝트 추가
	// 이 예제에서는 fw_add(new 객체(객체 번호, 레이어 번호), 레이어 번호) 형식으로 되어있음
	// 낮은 레이어부터, 레이어에 추가된 순서부터 업데이트 함
	
	// 객체 번호는 이 예제에서 객체 구분을 위해 사용하는 변수이며, 실제 사용 시 객체 번호는 없어도 됨
	// 객체 추가 시 반드시 생성자에 레이어 번호를 입력 해야 함
	
	// Monster가 하위 레이어에 있으므로 Monster부터 객체 코드 실행
	fw_add(new Human(1, 2), 2);
	fw_add(new Human(2, 2), 2);
	fw_add(new Human(3, 2), 2);
	fw_add(new Monster(1, 1), 1);
	fw_add(new Monster(2, 1), 1);
	fw_add(new Monster(3, 1), 1);

	std::cout << std::endl;
	
	// 전체 게임 루프
	while (true) {
		fw_routine();
	}
}
