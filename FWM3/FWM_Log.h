// FWM Version 3
#pragma once
#include "FWM_CONF.h"
#include <iostream>

enum class LogType {
	FWL_INIT,
	MODE_SWITCH,
	ADD_OBJECT,
	SET_NO_STOP_AT_PARTIAL_EXECUTION,
	DELETE_OBJECT,
	EVENT_PARTIAL_EXECUTION,
	DELETE_PARTIAL_EXECUTION_OBJECT,
	ERROR_SAME_MODE
};

class FWM_Log {
public:
	std::string PrevMode{};
	std::string CurrentMode{};
	std::string ObjectTag{};
	bool IsPartialExecutionState{};

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
				break;

			case LogType::ADD_OBJECT:
				cout << "Object Add\n";
				cout << "Object [ " << ObjectTag << " ] added.\n";
				break;

			case LogType::SET_NO_STOP_AT_PARTIAL_EXECUTION:
				cout << "Set Object No Stop At Partial Execution\n";
				cout << "Object [ " << ObjectTag << " ] set no stop at partial execution.\n";
				break;

			case LogType::DELETE_OBJECT:
				cout << "Object Delete\n";
				cout << "Object [ " << ObjectTag << " ] deleted.\n";
				break;

			case LogType::EVENT_PARTIAL_EXECUTION:
				cout << "Partial Execution State Change\n";
				if (IsPartialExecutionState)
					cout << "Started partial execution.\n";
				else
					cout << "Stopped partial execution.\n";
				break;

			case LogType::DELETE_PARTIAL_EXECUTION_OBJECT:
				cout << "Partial Execution Object Delete\n";
				cout << "All of the partial execution objects deleted.\n";
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