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

	if (!PartialDeleteState && !ModeSwitchState && RunningState) {
		for (int i = 0; i < Num; ++i) {
			if (Container[i].empty())
				continue;

			for (auto It = begin(Container[i]); It != end(Container[i]);) {
				if (CheckDeleteFlag(It, i))
					continue;

				if (!PartialExecutionState) {
					(*It)->InputControl();
					(*It)->Update(FrameTime);
				}

				else if (PartialExecutionState) {
					if ((*It)->PartialExecuteObject) {
						(*It)->InputControl();
						(*It)->Update(FrameTime);
					}
				}

				(*It)->Render();

				if (CheckDeleteFlag(It, i))
					continue;

				++It;
			}

			if (PartialDeleteReserveState) {
				PartialDeleteState = true;
				break;
			}

			if (ReserveState) {
				ModeSwitchState = true;
				break;
			}
		}
	}

	if (PartialDeleteReserveState) {
		RemovePartialObject();
		PartialDeleteReserveState = false;
	}

	if (ReserveState) {
		ChangeMode();
		ReserveState = false;
	}
}


void FWM::SwitchMode(Function ModeFunction) {
	if (!RunningState)
		return;

	Buffer = ModeFunction;
	FLog.PrevMode = RunningMode;

	ReserveState = true;
}

void FWM::StartPartialExecution() {
	if (PartialExecutionState)
		return;

	PartialExecutionState = true;
	FLog.IsPartialExecutionState = PartialExecutionState;
	FLog.Log(LogType::EVENT_PARTIAL_EXECUTION);
}

void FWM::StopPartialExecution() {
	if (!PartialExecutionState)
		return;

	PartialExecutionState = false;
	FLog.IsPartialExecutionState = PartialExecutionState;
	FLog.Log(LogType::EVENT_PARTIAL_EXECUTION);
}

void FWM::ClearPartialObject() {
	PartialDeleteReserveState = true;
}

void FWM::AddObject(OBJ_BASE* Object, std::string Tag, Layer AddLayer, bool PartialExecutionOpt) {
	Container[static_cast<int>(AddLayer)].push_back(Object);
	Object->ObjectTag = Tag;

	FLog.ObjectTag = Tag;
	FLog.Log(LogType::ADD_OBJECT);

	if (PartialExecutionOpt) {
		Object->PartialExecuteObject = true;
		FLog.Log(LogType::SET_NO_STOP_AT_PARTIAL_EXECUTION);
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



//////// private ///////////////

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

void FWM::RemovePartialObject() {
	for (int i = 0; i < Num; ++i) {
		for (auto It = begin(Container[i]); It != end(Container[i]);) {
			if ((*It)->PartialExecuteObject) {
				delete* It;
				*It = nullptr;
				It = Container[i].erase(It);
				continue;
			}

			++It;
		}
	}

	PartialExecutionState = false;
	PartialDeleteState = false;

	FLog.IsPartialExecutionState = PartialExecutionState;
	FLog.Log(LogType::EVENT_PARTIAL_EXECUTION);
	FLog.Log(LogType::DELETE_PARTIAL_EXECUTION_OBJECT);
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