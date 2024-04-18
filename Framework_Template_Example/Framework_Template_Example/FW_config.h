#pragma once
// 프레임워크 브릿지 설정

// 필수 옵션을 설정 해야 프레임워크가 활성화 됨
// 반드시 이 파일에 설정할 것 

// 필수 옵션
#define          USING_FRAMEWORK       // 프레임워크 사용 활성화
#define          NUMBER_OF_LAYER 3         // 레이어 개수 설정

// 선택 옵션
//#define        USING_OPTIMIZING  // 실시간 메모리 최적화 기능 옵션


// 게임 오브젝트 - 프레임워크 통신을 연결함
// Game object <----> Bridge <----> Framework <----> User

// Bridge 클래스
class Bridge {
public:
	// Bridge 멤버함수들은 반드시 게임 오브젝트 클래스들이 상속 받도록 해야함
	virtual void update() {}             // 게임 오브젝트 업데이트
	virtual void check_collision() {}    // 게임 오브젝트 충돌 처리
	virtual void render() {}             // 게임 오브젝트 출력
	virtual void check_delete_flag() {}  // 게임 오브젝트 삭제 플래그 검사

	// 이곳에 사용자 정의 함수 직접 정의
	// 변수 리턴 함수의 경우 기본 값을 리턴하는 코드로 정의할 것. 객체 클래스에서 재정의 되므로 Bridge 멤버 함수의 내용은 의미가 없음
	virtual int get_info() const {return {};}

	virtual      ~Bridge() {}            //소멸자, Bridge 클래스를 통해 객체의 소멸자를 호출함
};