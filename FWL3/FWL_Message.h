// FWL Version 3
#pragma once
#include "FWL_CONF.h"
#include <iostream>

enum class LogType {
	FWL_INIT,
	MODE_SWITCH,
	ADD_OBJECT,
	SET_AS_SUB_OBJECT,
	DELETE_OBJECT,
	ERROR_SAME_MODE
};

class FWL_Log {
public:
	std::string PrevMode{};
	std::string CurrentMode{};
	std::string ObjectTag{};
	bool IsPause{};

	void Log(LogType Type) {
		using namespace std;
		if (DebugMessage) {
			cout << "======== FWL Message ========\n";
			cout << "Message Type: ";

			switch (Type) {
			case LogType::FWL_INIT:
				cout << "FWL Init\n";
				cout << "FWL Staterted running with Mode [ " << CurrentMode << " ].\n";
				break;

			case LogType::MODE_SWITCH:
				cout << "Mode Switch\n";
				cout << "Mode Switched from [ " << PrevMode << " ] to [ " << CurrentMode << " ].\n";
				if (IsPause)
					cout << "PauseState is activated.\n";
				if (!IsPause)
					cout << "PauseState is not activated.\n";
				break;

			case LogType::ADD_OBJECT:
				cout << "Object Add\n";
				cout << "Object [ " << ObjectTag << " ] added.\n";
				break;

			case LogType::SET_AS_SUB_OBJECT:
				cout << "Set Object As Sub Object\n";
				cout << "Object [ " << ObjectTag << " ] set as sub object.\n";
				break;

			case LogType::DELETE_OBJECT:
				cout << "Object Delete\n";
				cout << "Object [ " << ObjectTag << " ] deleted.\n";
				break;
			}

			cout << "\n";
		}
	}

	void ErrorLog(LogType Type) {
		using namespace std;

		cout << "======== FWL Error ========\n";
		cout << "Error Type: ";

		switch (Type) {
		case LogType::ERROR_SAME_MODE:
			cout << "Mode Switch Error\n";
			cout << "Duplicate mode switch of the same mode as the current mode detected.\n";
			cout << "While mode [ " << PrevMode << " ]\n";
			break;
		}

		cout << "\n";

		exit(EXIT_FAILURE);
	}
};