#include "FWM.h"
#include <ranges>
#include <algorithm>


FWM_Log FLog;

FWM::FWM() {
	if (DebugMessage) {
		std::cout << "======== FWL Message ========\n";
		std::cout << "FWL is prepared.\n";
	}
}

void FWM::SetFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

std::string FWM::Mode() {
	return RunningMode;
}

void FWM::SwitchToSubRunningState() {
	SubRunningState = true;
}

void FWM::SwitchToDefaultRunningState() {
	SubRunningState = false;
}

void FWM::Init(Function ModeFunction) {
	if (RunningState)
		return;

	RunningMode = ModeFunction();

	FLog.CurrentMode = RunningMode;
	FLog.Log(LogType::FWL_INIT);

	RunningState = true;
}

void FWM::Routine() {
	using namespace std;

	if (!ModeSwitchState && RunningState) {
		for (int i = 0; i < Num; ++i) {
			if (Container[i].empty())
				continue;

			for (auto It = begin(Container[i]); It != end(Container[i]);) {
				if (CheckDeleteFlag(It, i))
					continue;

				if (!SubRunningState)
					(*It)->Update(FrameTime);

				else if (SubRunningState) {
					if (!(*It)->StopAtPauseFlag)
						(*It)->Update(FrameTime);
				}

				(*It)->Render();

				if (CheckDeleteFlag(It, i))
					continue;

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

void FWM::SwitchMode(Function ModeFunction, bool PauseOption) {
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

void FWM::AddObject(OBJ_BASE* Object, std::string Tag, Layer AddLayer, bool AddAsSubObject) {
	Container[static_cast<int>(AddLayer)].push_back(Object);
	Object->ObjectTag = Tag;

	FLog.ObjectTag = Tag;
	FLog.Log(LogType::ADD_OBJECT);

	if (AddAsSubObject) {
		Object->StopAtPauseFlag = false;
		FLog.Log(LogType::SET_AS_SUB_OBJECT);
	}
}

void FWM::DeleteSelf(OBJ_BASE* Object) {
	Object->DeleteFlag = true;

	FLog.ObjectTag = Object->ObjectTag;
	FLog.Log(LogType::DELETE_OBJECT);
}

void FWM::DeleteObject(std::string Tag, DeleteRange deleteRange, SearchRange searchRange, Layer LayerToSearch) {
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

OBJ_BASE* FWM::Find(std::string Tag, SearchRange searchRange, Layer LayerToSearch) {
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

OBJ_BASE* FWM::Find(std::string Tag, Layer LayerToSearch, int Index) {
	int layer = static_cast<int>(LayerToSearch);

	if (Index >= Container[layer].size())
		return nullptr;

	if (Container[layer][Index]->ObjectTag == Tag)
		return Container[layer][Index];

	return nullptr;
}

size_t FWM::Size(Layer TargetLayer) {
	return Container[static_cast<int>(TargetLayer)].size();
}


bool FWM::CheckDeleteFlag(std::deque<OBJ_BASE*>::iterator& It, int Layer) {
	if ((*It)->DeleteFlag) {
		delete* It;
		*It = nullptr;
		It = Container[Layer].erase(It);
		return true;
	}
	return false;
}

void FWM::ChangeMode() {
	ClearAll();

	RunningMode = Buffer();
	Buffer = nullptr;

	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);

	FLog.CurrentMode = RunningMode;
	FLog.Log(LogType::MODE_SWITCH);

	ModeSwitchState = false;
}

void FWM::ClearAll() {
	using namespace std;

	for (int i = 0; i < Num; ++i) {
		for (auto It = begin(Container[i]); It != end(Container[i]);) {
			delete* It;
			*It = nullptr;
			It = Container[i].erase(It);
		}
	}
}