// FWL Version 2

#pragma once
#include "FWL_CONF.h"
#include "FWL_MESSEGE.h"
#include <algorithm>
#include <array>
#include <ctime>
#include <deque>
#include <ranges>
#include <string>


// Store the number of layers set in FWL_CONF at compile time
constexpr int NUM_MAIN_LAYER{ static_cast<int>(MainLayer::END) };
constexpr int NUM_SUB_LAYER { static_cast<int>(SubLayer::END)  };


// Enumerations for FWL functions
enum class RoutineType 
{ Main, Sub };

enum class LayerRange  
{ Single, All };

enum class ObjectRange 
{ Single, All };


// FWL class
class FWL {
private:
	std::array<std::deque<MAIN_OBJ*>, NUM_MAIN_LAYER> MainCont;
	std::array<std::deque<SUB_OBJ*>,   NUM_SUB_LAYER> SubCont;

	std::string CurrentMainMode;
	std::string CurrentSubMode;
	std::string CurrentRunningMode;

	bool		MainRoutineRunningState;
	bool		SubRoutineRunningState;

	bool		ModeSwitchingState;
	bool		MainRoutinePauseState;

	time_t		StartTime;
	time_t		EndTime;
	double		DeltaTime;

	Messege		MSG;

	typedef std::string (*Function)(void);


public:
	// Return the name of the currently running mode name
	std::string NowRunning() { return CurrentRunningMode; }


	// Return the name of the currently running main mode name
	std::string NowMain() { return CurrentMainMode; }


	// Return the name of the currently running sub mode name
	std::string NowSub() { return CurrentSubMode; }


#ifdef USE_FRAME_TIME
#ifdef USE_FRAME_TIME_EXTERNAL
	// Input the frame time from external value
	void InputFT(double ElaspedTime) { DeltaTime = ElaspedTime; }
#endif

	// // Multiply the frame time with movement
	double FT(double Value, double MulValue=1.0) { return Value * MulValue * DeltaTime; }
#endif


	// FWL Routine
	void Routine() {
#ifdef USE_FRAME_TIME
#ifndef USE_FRAME_TIME_EXTERNAL
		StartTime = clock();
#endif
#endif

		if (MainRoutineRunningState)
			MainRoutine();

		if (SubRoutineRunningState)
			SubRoutine();

#ifdef USE_FRAME_TIME
#ifndef USE_FRAME_TIME_EXTERNAL
		EndTime = clock();
		DeltaTime = double(EndTime - StartTime);
#endif
#endif
	}


	// Initialize routine
	void InitRoutine(RoutineType Type, Function ModeFunction, bool PauseOPT=false) {
		std::string ModeName{};

		switch (Type) {
		case RoutineType::Main:
			if (MainRoutineRunningState)
				MSG.FWL_ERROR(MsgType::Main, Error::RPT_INIT, __func__);

			ModeName = ModeFunction();

			CurrentMainMode = ModeName;
			CurrentRunningMode = ModeName;

			MSG.TransmitCurrMain(ModeName);
			MSG.FWL_MSG(MsgType::Main, State::Init);

			MainRoutineRunningState = true;
			break;


		case RoutineType::Sub:
			if (SubRoutineRunningState)  
				MSG.FWL_ERROR(MsgType::Sub, Error::RPT_INIT, __func__);

			ModeSwitchingState = true;

			ModeName = ModeFunction();

			if (PauseOPT) {
				MainRoutinePauseState = true;
				MSG.TransmitPauseState(MainRoutinePauseState);
			}

			CurrentSubMode = ModeName;
			CurrentRunningMode = ModeName;

			MSG.TransmitCurrSub(ModeName);
			MSG.FWL_MSG(MsgType::Sub, State::Init);

			SubRoutineRunningState = true;
			ModeSwitchingState = false;
			break;
		}
	}


	// Switch the currently running mode to another mode
	void SwitchMode(RoutineType Type, Function ModeFunction) {
		std::string ModeName{};

		switch (Type) {
		case RoutineType::Main:
			if(!MainRoutineRunningState)
				MSG.FWL_ERROR(MsgType::Main, Error::INV_INIT, __func__);

			if (SubRoutineRunningState)  
				EndSubRoutine();

			ModeSwitchingState = true;

			ClearRoutine(RoutineType::Main);

			ModeName = ModeFunction();

			MSG.TransmitPrevMain(CurrentMainMode);
			CurrentRunningMode = ModeName;
			CurrentMainMode = ModeName;
			
			MSG.TransmitNextMain(ModeName);
			MSG.TransmitCurrMain(ModeName);
			MSG.FWL_MSG(MsgType::Main, State::Switch);

			ModeSwitchingState = false;
			break;


		case RoutineType::Sub:
			if (!SubRoutineRunningState)  
				MSG.FWL_ERROR(MsgType::Sub, Error::INV_INIT, __func__);

			ModeSwitchingState = true;

			ClearRoutine(RoutineType::Sub);

			ModeName = ModeFunction();

			MSG.TransmitPrevSub(CurrentSubMode);
			CurrentRunningMode = ModeName;
			CurrentSubMode = ModeName;

			MSG.TransmitNextSub(ModeName);
			MSG.TransmitCurrSub(ModeName);
			MSG.FWL_MSG(MsgType::Sub, State::Switch);

			ModeSwitchingState = false;
			break;
		}
	}


	// Delete all of objects in sub routine and exit the sub routine
	void EndSubRoutine() {
		if (!SubRoutineRunningState)  
			MSG.FWL_ERROR(MsgType::Sub, Error::INV_INIT, __func__);

		ModeSwitchingState = true;

		ClearRoutine(RoutineType::Sub);

		CurrentSubMode = "";
		CurrentRunningMode = CurrentMainMode;

		MSG.FWL_MSG(MsgType::Sub, State::SubEnd);
		MSG.TransmitPauseState(false);

		SubRoutineRunningState = false;
		MainRoutinePauseState = false;
		ModeSwitchingState = false;
	}


	// Add an object to main routine
	void AddObject(MAIN_OBJ*&& Object, MainLayer Layer) { 
		MainCont[static_cast<int>(Layer)].push_back(Object); 
	}


	// Add an object to sub routine
	void AddObject(SUB_OBJ*&& Object, SubLayer Layer) { 
		SubCont[static_cast<int>(Layer)].push_back(Object); 
	}


	// Deletes the object itself from main routine
	void DeleteObject(MAIN_OBJ* Object, MainLayer Layer) {
		int Num = static_cast<int>(Layer);
		auto It = std::ranges::find(MainCont[Num], Object);

		if (It != std::ranges::end(MainCont[Num])) {
			delete* It;
			*It = nullptr;
			It = MainCont[Num].erase(It);
		}
	}


	// Deletes the object itself from sub routine
	void DeleteObject(SUB_OBJ* Object, SubLayer Layer) {
		if (!SubRoutineRunningState)
			MSG.FWL_ERROR(MsgType::Sub, Error::INV_INIT, __func__);

		int Num = static_cast<int>(Layer);
		auto It = std::ranges::find(SubCont[Num], Object);

		if (It != std::ranges::end(SubCont[Num])) {
			delete* It;
			*It = nullptr;
			It = SubCont[Num].erase(It);
		}
	}


	// Delete an object from main routine
	void DeleteObject(std::string ObjTag, ObjectRange ObjRange, LayerRange LyrRange, MainLayer Layer=static_cast<MainLayer>(0)) {
		int Num = static_cast<int>(Layer);

		switch (LyrRange) {
		case LayerRange::Single:
			switch (ObjRange) {
			case ObjectRange::Single:
				{
					auto It = std::ranges::find_if(MainCont[Num], [&ObjTag](MAIN_OBJ*& Obj) { return Obj->Tag() == ObjTag; });

					if (It != std::ranges::end(MainCont[Num])) {
						delete* It;
						*It = nullptr;
						It = MainCont[Num].erase(It);
					}
				}
				break;

			case ObjectRange::All:
				{
					auto It = std::ranges::begin(MainCont[Num]);
					auto SubRange = std::ranges::subrange(It, std::ranges::end(MainCont[Num]));

					while (It != std::ranges::end(MainCont[Num])) {
						It = std::ranges::find_if(SubRange, [&ObjTag](MAIN_OBJ*& Obj) { return Obj->Tag() == ObjTag; });

						if (It != std::ranges::end(MainCont[Num])) {
							delete* It;
							*It = nullptr;
							It = MainCont[Num].erase(It);
							SubRange = std::ranges::subrange(It, std::ranges::end(MainCont[Num]));
						}
					}
				}
				break;
			}
			break;


		case LayerRange::All:
			switch (ObjRange) {
			case ObjectRange::Single:
				for (int i = 0; i < NUM_MAIN_LAYER; ++i) {
					auto It = std::ranges::find_if(MainCont[i], [&ObjTag](MAIN_OBJ*& Obj) { return Obj->Tag() == ObjTag; });

					if (It != std::ranges::end(MainCont[i])) {
						delete* It;
						*It = nullptr;
						It = MainCont[i].erase(It);
						break;
					}
				}
				break;

			case ObjectRange::All:
				for (int i = 0; i < NUM_MAIN_LAYER; ++i) {
					auto It = std::ranges::begin(MainCont[i]);
					auto SubRange = std::ranges::subrange(It, std::ranges::end(MainCont[i]));

					while (It != std::ranges::end(MainCont[i])) {
						It = std::ranges::find_if(SubRange, [&ObjTag](MAIN_OBJ*& Obj) { return Obj->Tag() == ObjTag; });

						if (It != std::ranges::end(MainCont[i])) {
							delete* It;
							*It = nullptr;
							It = MainCont[i].erase(It);

							SubRange = std::ranges::subrange(It, std::ranges::end(MainCont[i]));
						}
					}
				}
				break;
			}
			break;
		}
	}


	// Delete an object from sub routine
	void DeleteObject(std::string ObjTag, ObjectRange ObjRange, LayerRange LyrRange, SubLayer Layer=static_cast<SubLayer>(0)) {
		int Num = static_cast<int>(Layer);

		switch (LyrRange) {
		case LayerRange::Single:
			switch (ObjRange) {
			case ObjectRange::Single:
				{
					auto It = std::ranges::find_if(SubCont[Num], [&ObjTag](SUB_OBJ*& Obj) { return Obj->Tag() == ObjTag; });

					if (It != std::ranges::end(SubCont[Num])) {
						delete* It;
						*It = nullptr;
						It = SubCont[Num].erase(It);
					}
				}
				break;

			case ObjectRange::All:
				{
					auto It = std::ranges::begin(SubCont[Num]);
					auto SubRange = std::ranges::subrange(It, std::ranges::end(SubCont[Num]));

					while (It != std::ranges::end(SubCont[Num])) {
						It = std::ranges::find_if(SubRange, [&ObjTag](SUB_OBJ*& Obj) { return Obj->Tag() == ObjTag; });

						if (It != std::ranges::end(SubCont[Num])) {
							delete* It;
							*It = nullptr;
							It = SubCont[Num].erase(It);

							SubRange = std::ranges::subrange(It, std::ranges::end(SubCont[Num]));
						}
					}
				}
				break;
			}
			break;


		case LayerRange::All:
			switch (ObjRange) {
			case ObjectRange::Single:
				for (int i = 0; i < NUM_SUB_LAYER; ++i) {
					auto It = std::ranges::find_if(SubCont[i], [&ObjTag](SUB_OBJ*& Obj) { return Obj->Tag() == ObjTag; });

					if (It != std::ranges::end(SubCont[i])) {
						delete* It;
						*It = nullptr;
						It = SubCont[i].erase(It);
						break;
					}
				}
				break;

			case ObjectRange::All:
				for (int i = 0; i < NUM_SUB_LAYER; ++i) {
					auto It = std::ranges::begin(SubCont[i]);
					auto SubRange = std::ranges::subrange(It, std::ranges::end(SubCont[i]));

					while (It != std::ranges::end(SubCont[i])) {
						It = std::ranges::find_if(SubRange, [&ObjTag](SUB_OBJ*& Obj) { return Obj->Tag() == ObjTag; });

						if (It != std::ranges::end(SubCont[i])) {
							delete* It;
							*It = nullptr;
							It = SubCont[i].erase(It);

							std::ranges::subrange(It, std::ranges::end(SubCont[i]));
						}
					}
				}
				break;
			}
			break;
		}
	}


	// Find an object in main routine
	MAIN_OBJ* FindObject(std::string ObjTag, LayerRange LyrRange, MainLayer Layer = static_cast<MainLayer>(0)) {
		switch (LyrRange) {
			case LayerRange::Single:
				{
					int Num = static_cast<int>(Layer);
					auto It = std::ranges::find_if(MainCont[Num], [&ObjTag](MAIN_OBJ*& Obj) { return Obj->Tag() == ObjTag; });

					if (It != std::ranges::end(MainCont[Num]))
						return *It;
					else
						return nullptr;
				}
				break;

		case LayerRange::All:
			for (int i = 0; i < NUM_MAIN_LAYER; ++i) {
				auto It = std::ranges::begin(MainCont[i]);

				while (It != std::ranges::end(MainCont[i])) {
					It = std::ranges::find_if(MainCont[i], [&ObjTag](MAIN_OBJ*& Obj) { return Obj->Tag() == ObjTag; });

					if (It != std::ranges::end(MainCont[i]))
						return *It;
				}
			}
			return nullptr;
			break;
		}

		return nullptr;
	}


	// Find an object in sub routine
	SUB_OBJ* FindObject(std::string ObjTag, LayerRange LyrRange, SubLayer Layer = static_cast<SubLayer>(0)) {
		switch (LyrRange) {
		case LayerRange::Single:
			{
				int Num = static_cast<int>(Layer);
				auto It = std::ranges::find_if(SubCont[Num], [&ObjTag](SUB_OBJ*& Obj) { return Obj->Tag() == ObjTag; });

				if (It != std::ranges::end(SubCont[Num]))
					return *It;
				else
					return nullptr;
			}
			break;

		case LayerRange::All:
			for (int i = 0; i < NUM_SUB_LAYER; ++i) {
				auto It = std::ranges::begin(SubCont[i]);

				while (It != std::ranges::end(SubCont[i])) {
					It = std::ranges::find_if(SubCont[i], [&ObjTag](SUB_OBJ*& Obj) { return Obj->Tag() == ObjTag; });

					if (It != std::ranges::end(SubCont[i]))
						return *It;
				}
			}
			return nullptr;
			break;
		}

		return nullptr;
	}


	// Find an object in main routine with index value
	MAIN_OBJ* FindObject(std::string ObjTag, MainLayer Layer, int Index) {
		int Num = static_cast<int>(Layer);

		if(Index >= MainCont[Num].size())
			return nullptr;

		if (MainCont[Num][Index] && MainCont[Num][Index]->Tag() == ObjTag)
			return MainCont[Num][Index];
		else
			return nullptr;
	}


	// Find an object in sub routine with index value
	SUB_OBJ* FindObject(std::string ObjTag, SubLayer Layer, int Index) {
		int Num = static_cast<int>(Layer);

		if(Index >= SubCont[Num].size())
			return nullptr;

		if (SubCont[Num][Index] && SubCont[Num][Index]->Tag() == ObjTag)
			return SubCont[Num][Index];
		else
			return nullptr;
	}


	// Return a number of objects in specific main routine layer
	size_t LayerSize(MainLayer Layer) { 
		return std::ranges::size(MainCont[static_cast<int>(Layer)]);
	}


	// Return a number of objects in specific sub routine layer
	size_t LayerSize(SubLayer Layer) { 
		return std::ranges::size(SubCont[static_cast<int>(Layer)]);
	}


private:
	// Update objects in main routine
	void MainRoutine() {
		for (int i = 0; i < NUM_MAIN_LAYER; ++i) {
			for (auto It = std::ranges::begin(MainCont[i]); It != std::ranges::end(MainCont[i]);) {
				if (!MainRoutinePauseState && !ModeSwitchingState) {
					if(*It) (*It)->Update();
					if(*It) (*It)->Collision();
				}

				if (*It) (*It)->Render();
				if (*It) ++It;
			}
		}
	}

	
	// Update objects in sub routine
	void SubRoutine() {
		for (int i = 0; i < NUM_SUB_LAYER; ++i) {
			for (auto It = std::ranges::begin(SubCont[i]); It != std::ranges::end(SubCont[i]);) {
				if (!ModeSwitchingState) {
					if (*It) (*It)->Update();
					if (*It) (*It)->Collision();
				}

				if (*It) (*It)->Render();
				if (*It) ++It;
			}
		}
	}


	// Delete all of objects from main routine or sub routine
	void ClearRoutine(RoutineType RoutineType) {
		switch (RoutineType) {
		case RoutineType::Main:
			for (int i = 0; i < NUM_MAIN_LAYER; ++i) {
				for (auto It = std::ranges::begin(MainCont[i]); It != std::ranges::end(MainCont[i]);) {
					delete* It;
					*It = nullptr;

					It = MainCont[i].erase(It);
				}
			}
			break;

		case RoutineType::Sub:
			for (int i = 0; i < NUM_SUB_LAYER; ++i) {
				for (auto It = std::ranges::begin(SubCont[i]); It != std::ranges::end(SubCont[i]);) {
					delete* It;
					*It = nullptr;

					It = SubCont[i].erase(It);
				}
			}
			break;
		}
	}
};