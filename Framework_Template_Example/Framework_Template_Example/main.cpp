#include "Objects.h"  // Framework.h를 포함

// 템플릿 예제 구조
// Header.h - Framework.h - Objects.h
//                 |             |
//           Framework.cpp    main.cpp

int main() {
	// 프레임 워크에 오브젝트 추가
	// 먼저 추가한 순서 또는 낮은 레이어부터 업데이트 함
	// 객체 추가 시 반드시 생성자가 레이어 번호를 저장하도록 해야 함
	// Monster가 하위 레이어에 있으므로 Monster부터 객체 코드 실행
	__add__(new Human(1, 2), 2);
	__add__(new Human(2, 2), 2);
	__add__(new Human(3, 2), 2);
	__add__(new Monster(1, 1), 1);
	__add__(new Monster(2, 1), 1);
	__add__(new Monster(3, 1), 1);

	std::cout << std::endl;
	
	// 전체 게임 루프
	while (true) {
		__routine__();
	}
}
