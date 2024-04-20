#include <iostream>

#include "Framework.h"
#include "Objects.h"
#include "modes.h"


// 프레임워크가 필요한 곳은 extern 키워드를 붙여 프레임워크 사용을 명시할 것
extern Framework fw;

// 예제 모드를 시작하는 코드
// 시작 코드는 void type 함수로 작성할 것
void example_mode() {
	// 프레임 워크에 오브젝트 추가
	// 낮은 레이어부터, 레이어에 추가된 순서부터 업데이트 함

	// 이 예제에서는 fw.add_object(new 객체(객체 번호, 레이어 번호), 레이어 번호) 형식으로 되어있음
	// 객체 추가 시 반드시 생성자에 레이어 번호를 입력 해야 함
	// 객체 번호는 이 예제에서 객체 구분을 위해 사용하는 변수이며, 실제 사용 시 객체 번호는 없어도 됨
	// Monster가 하위 레이어에 있으므로 Monster부터 객체 코드 실행
	fw.add_object(new Human(1, 1), 1);
	fw.add_object(new Human(2, 1), 1);
	fw.add_object(new Human(3, 1), 1);
	fw.add_object(new Monster(1, 0), 0);
	fw.add_object(new Monster(2, 0), 0);
	fw.add_object(new Monster(3, 0), 0);

	// connect_ptr() 설명용 객체
	// 2번 레이어에 추가
	if (!class_message)
		fw.add_object(new ConnectPtrExample(2), 2);

	std::cout << std::endl;
}


// 다른 모드로 변경 시 메시지를 출력함
void other_mode() {
	std::cout << "mode changed to other_mode" << std::endl << std::endl;
}