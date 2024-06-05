// FWL Version 2

#pragma once
#include "FWL_CONF.h"
#include <iostream>


enum class MsgType 
{ Main, Sub };

enum class State 
{ Init, Switch, SubEnd };

enum class Error 
{ INV_INIT, RPT_INIT };


class Messege {
private:
	std::string PrevMain;
	std::string NextMain;
	std::string CurrMain;

	std::string PrevSub;
	std::string NextSub;
	std::string CurrSub;

	bool		PauseState;


public:
	void TransmitPrevMain(std::string Prev) { PrevMain = Prev; }
	void TransmitNextMain(std::string Next) { NextMain = Next; }
	void TransmitCurrMain(std::string Curr) { CurrMain = Curr; }

	void TransmitPrevSub(std::string Prev) { PrevSub = Prev; }
	void TransmitNextSub(std::string Next) { NextSub = Next; }
	void TransmitCurrSub(std::string Curr) { CurrSub = Curr; }

	void TransmitPauseState(bool State) { PauseState = State; }


	void FWL_MSG(MsgType Messege, State RunningState) {
#ifdef USE_DEBUG_MESSEGE
		using namespace std;

		switch (Messege) {
		case MsgType::Main:
			cout << "======= FWL Messege: Main routine =======\n";

			switch (RunningState) {
			case State::Init:
				cout << "Initialization completed.\n";
				cout << "Initialized with mode [ " << CurrMain << " ]\n";
				break;

			case State::Switch:
				cout << "Mode switch completed.\n";
				cout << "[ " << PrevMain << " ] to [ " << NextMain << " ]\n";
				break;
			}
			break;

		case MsgType::Sub:
			cout << "======= FWL Messege: Sub routine =======\n";

			switch (RunningState) {
			case State::Init:
				cout << "Initialization completed.\n";
				cout << "Initialized with mode [ " << CurrSub << " ]\n";
				cout << "While main routine: Mode:[ " << CurrMain << " ]\n";
				if (PauseState)
					cout << "* Main routine pause state activated *\n";
				break;

			case State::Switch:
				cout << "Mode switch completed.\n";
				cout << "[ " << PrevSub << " ] to [ " << NextSub << " ]\n";
				break;

			case State::SubEnd:
				cout << "Stopped sub routine.\n";
				cout << "While main routine: Mode:[ " << CurrMain << " ]\n";
				break;
			}
			break;
		}

		cout << "\n\n";
#endif
	}


	void FWL_ERROR(MsgType Messege, Error ErrorType, std::string FuncName) {
#ifdef USE_DEBUG_MESSEGE
		using namespace std;

		switch (Messege) {
		case MsgType::Main:
			cout << "======= Error Messege: Main Routine =======\n";
			cout << "Error Type: ";

			switch (ErrorType) {
			case Error::INV_INIT:
				cout << "Invalid Initialization\n";
				cout << "Main routine is not initialized now. Use InitRoutine() first.\n";
				break;

			case Error::RPT_INIT:
				cout << "Repeated Initilization\n";
				cout << "Repeated main routine initialization has been detected.\n";
				cout << "While main routine: Mode:[ " << CurrMain << " ]\n";
				break;
			}
			break;

		case MsgType::Sub:
			cout << "======= Error Messege: Sub Routine =======\n";
			cout << "Error Type: ";

			switch (ErrorType) {
			case Error::INV_INIT:
				cout << "Invalid Initialization\n";
				cout << "Sub routine is not initialized now. Use InitRoutine() first.\n";
				break;

			case Error::RPT_INIT:
				cout << "Repeated Initilization\n";
				cout << "Repeated sub routine initialization has been detected.\n";
				cout << "While sub routine: Mode:[ " << CurrSub << " ]\n";
			}
			break;
		}

		cout << "\nError occured in FWL function [ " << FuncName << "() ]\n\n\n";
#endif
		exit(EXIT_FAILURE);
	}


	~Messege() {
#ifdef USE_DEBUG_MESSEGE
		using namespace std;

		cout << "======= FWL Messege =======\n";
		cout << "Stop Run.\n\n\n";
#endif
	}
};