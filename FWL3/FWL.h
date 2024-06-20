// FWL Version 3
#pragma once
#include "FWL_Message.h"
#include <array>
#include <deque>
#include <ranges>
#include <algorithm>

typedef std::string(*Function)(void);
constexpr int Num = static_cast<int>(Layer::END);

enum class DeleteRange
{One, All};

enum class SearchRange
{One, All};


FWL_Log FLog;

class FWL {
private:
	std::array<std::deque<OBJ*>, Num> Container;

	std::string						  RunningMode{};

	bool							  RunningState{};
	bool							  ReserveState{};
	bool							  ModeSwitchState{};
	bool							  SubRunningState{};

	float							  FrameTime{};

	Function						  Buffer{};

public:
	FWL() {
		if (DebugMessage) {
			std::cout << "======== FWL Message ========\n";
			std::cout << "FWL is prepared.\n";
		}
	}

	std::string Mode() {
		return RunningMode;
	}

	void SetFrameTime(float ElapsedTime) {
		FrameTime = ElapsedTime;
	}

	void SwitchToSubRunningState() {
		SubRunningState = true;
	}

	void SwitchToDefaultRunningState() {
		SubRunningState = false;
	}

	void Init(Function ModeFunction) {
		if (RunningState)
			return;

		RunningMode = ModeFunction();

		FLog.CurrentMode = RunningMode;
		FLog.Log(LogType::FWL_INIT);

		RunningState = true;
	}

	void Routine() {
		using namespace std;

		if (!ModeSwitchState && RunningState) {
			for (int i = 0; i < Num; ++i) {
				if (Container[i].empty())
					continue;

				for (auto It = begin(Container[i]); It != end(Container[i]);) {
					if ((*It)->DeleteFlag) {
						delete* It;
						*It = nullptr;
						It = Container[i].erase(It);
						continue;
					}

					if (!SubRunningState)
						(*It)->Update(FrameTime);

					else if (SubRunningState) {
						if(!(*It)->StopAtPauseFlag)
							(*It)->Update(FrameTime);
					}
					(*It)->Render();


					if ((*It)->DeleteFlag) {
						delete* It;
						*It = nullptr;
						It = Container[i].erase(It);
					}

					else
						++It;
				}

				if (ReserveState) {
					ModeSwitchState = true;
					break;
				}
			}
		}

		if (ReserveState) {
			ChangeMode();
			ReserveState = false;
		}
	}

	void SwitchMode(Function ModeFunction, bool PauseOption=false) {
		if (!RunningState)
			return;

		Buffer = ModeFunction;
		ReserveState = true;

		if (PauseOption)
			SubRunningState = true;
		else
			SubRunningState = false;

		FLog.IsPause = SubRunningState;
		FLog.PrevMode = RunningMode;
	}

	void AddObject(OBJ* Object, std::string Tag, Layer AddLayer, bool AddAsSubObject=false) {
		Container[static_cast<int>(AddLayer)].push_back(Object);
		Object->ObjectTag = Tag;

		FLog.ObjectTag = Tag;
		FLog.Log(LogType::ADD_OBJECT);

		if (AddAsSubObject) {
			Object->StopAtPauseFlag = false;
			FLog.Log(LogType::SET_AS_SUB_OBJECT);
		}
	}

	void DeleteSelf(OBJ* Object) {
		Object->DeleteFlag = true;

		FLog.ObjectTag = Object->ObjectTag;
		FLog.Log(LogType::DELETE_OBJECT);
	}

	void DeleteObject(std::string Tag, DeleteRange deleteRange, SearchRange searchRange, Layer LayerToSearch) {
		switch (searchRange) {
		case SearchRange::One:
			int layer;
			layer = static_cast<int>(LayerToSearch);

			switch (deleteRange) {
			case DeleteRange::One:
				for (auto& It : Container[layer]) {
					if (It->ObjectTag == Tag) {
						It->DeleteFlag = true;

						FLog.ObjectTag = It->ObjectTag;
						FLog.Log(LogType::DELETE_OBJECT);
						return;
					}
				}
				break;

			case DeleteRange::All:
				for (auto& It : Container[layer]) {
					if (It->ObjectTag == Tag) {
						It->DeleteFlag = true;

						FLog.ObjectTag = It->ObjectTag;
						FLog.Log(LogType::DELETE_OBJECT);
					}
				}
				break;
			}
			break;

		case SearchRange::All:
			switch (deleteRange) {
			case DeleteRange::One:
				for (auto& A : Container) {
					for (auto& It : A) {
						if (It->ObjectTag == Tag) {
							It->DeleteFlag = true;

							FLog.ObjectTag = It->ObjectTag;
							FLog.Log(LogType::DELETE_OBJECT);
							return;
						}
					}
				}
				break;

			case DeleteRange::All:
				for (auto& A : Container) {
					for (auto& It : A) {
						if (It->ObjectTag == Tag) {
							It->DeleteFlag = true;

							FLog.ObjectTag = It->ObjectTag;
							FLog.Log(LogType::DELETE_OBJECT);
						}
					}
				}
				break;
			}
			break;
		}
	}

	OBJ* Find(std::string Tag, SearchRange searchRange, Layer LayerToSearch = Layer::L1) {
		switch (searchRange) {
		case SearchRange::One:
			int layer;
			layer = static_cast<int>(LayerToSearch);

			for (auto& It : Container[layer]) {
				if (It->ObjectTag == Tag)
					return It;
			}
			break;

		case SearchRange::All:
			for (auto& A : Container) {
				for (auto& It : A) {
					if (It->ObjectTag == Tag)
						return It;
				}
			}
			break;
		}

		return nullptr;
	}

	OBJ* Find(std::string Tag, Layer LayerToSearch, int Index) {
		int layer = static_cast<int>(LayerToSearch);

		if (Index >= Container[layer].size())
			return nullptr;

		if (Container[layer][Index]->ObjectTag == Tag)
			return Container[layer][Index];

		return nullptr;
	}

	size_t Size(Layer TargetLayer) {
		return Container[static_cast<int>(TargetLayer)].size();
	}


private:
	void ChangeMode() {
		ClearAll();

		RunningMode = Buffer();
		Buffer = nullptr;

		if (FLog.CurrentMode == FLog.PrevMode)
			FLog.ErrorLog(LogType::ERROR_SAME_MODE);

		FLog.CurrentMode = RunningMode;
		FLog.Log(LogType::MODE_SWITCH);

		ModeSwitchState = false;
	}

	void ClearAll() {
		using namespace std;

		for (int i = 0; i < Num; ++i) {
			for (auto It = begin(Container[i]); It != end(Container[i]);) {
				delete* It;
				*It = nullptr;
				It = Container[i].erase(It);
			}
		}
	}
};