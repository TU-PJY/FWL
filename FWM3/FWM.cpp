#include "FWM.h"
#include <ranges>
#include <algorithm>

FWM_Log FLog;

FWM::FWM() {
	if (DebugMessage) {
		std::cout << "======== FWM Message ========\n";
		std::cout << "FWM is prepared.\n\n";
	}
}

void FWM::SetFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

std::string FWM::Mode() {
	return RunningMode;
}

void FWM::Routine() {
	if (!ModeSwitchingDescriptor && RunningActivateDescriptor) {
		for (int i = 0; i < Num; ++i) {
			for (auto It = begin(Container[i]); It != end(Container[i]); ++It) {
				if (FloatingModeRunningDescriptor && FloatingOnlyDescriptor) {
					if ((*It)->FloatingDescriptor)
						(*It)->Update(FrameTime);
				}

				else
					(*It)->Update(FrameTime);
				(*It)->Render();
			}

			if (ModeSwitchReserveDescriptor) {
				ModeSwitchingDescriptor = true;
				break;
			}

			ClearDeleteTargetObject(i);
		}
	}

	if (ModeSwitchReserveDescriptor)
		ChangeMode();
}

void FWM::Init(Function ModeFunction, ControllerFunction Controller) {
	if (RunningActivateDescriptor)
		return;

	// add dummy objects
	for (int i = 0; i < Num; ++i)
		AddObject(new FWM_DUMMY, "FWM_DUMMY", static_cast<Layer>(i));
	ObjectList.clear();

	RunningMode = ModeFunction();

	if(Controller)
		Controller();

	ControllerBackUpBuffer = Controller;

	FLog.CurrentMode = RunningMode;
	FLog.Log(LogType::FWL_INIT);

	RunningActivateDescriptor = true;
}

void FWM::SwitchMode(Function ModeFunction, ControllerFunction Controller) {
	if (!RunningActivateDescriptor)
		return;

	ModeFunctionBuffer = ModeFunction;
	ControllerBuffer = Controller;
	ControllerBackUpBuffer = Controller;

	FLog.PrevMode = RunningMode;

	ModeSwitchReserveDescriptor = true;

	if (FloatingModeRunningDescriptor)
		FLog.Log(LogType::END_FLOATING_MODE);
}

void FWM::AddObject(OBJ_BASE* Object, std::string Tag, Layer AddLayer, bool SetFloatingObject) {
	Container[static_cast<int>(AddLayer)].push_back(Object);
	Object->ObjectTag = Tag;

	ObjectList.insert(std::make_pair(Tag, Object));

	FLog.ObjectTag = Tag;
	FLog.Log(LogType::ADD_OBJECT);

	if (SetFloatingObject) {
		Object->FloatingDescriptor = true;
		FLog.Log(LogType::SET_FLOATING_OBJECT);
	}
}

void FWM::DeleteSelf(OBJ_BASE* Object) {
	Object->DeleteDescriptor = true;

	FLog.ObjectTag = Object->ObjectTag;
	FLog.Log(LogType::DELETE_OBJECT);
}

void FWM::DeleteObject(std::string Tag, DeleteRange deleteRange) {
	if (deleteRange == DeleteRange::One) {
		auto It = ObjectList.lower_bound(Tag);
		if (It != end(ObjectList)) {
			It->second->DeleteDescriptor = true;
			ObjectList.erase(It);
		}
	}

	else if (deleteRange == DeleteRange::All) {
		for (auto It = begin(ObjectList); It != end(ObjectList); ++It) {
			if (It != end(ObjectList) && It->second->ObjectTag == Tag)
				It->second->DeleteDescriptor = true;
		}
	}
}

OBJ_BASE* FWM::Find(std::string Tag) {
	auto It = ObjectList.lower_bound(Tag);
	if (It != end(ObjectList))
		return It->second;

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

void FWM::StartFloatingMode(Function ModeFunction, ControllerFunction Controller, bool FloatingOnlyOption) {
	if (!RunningActivateDescriptor || FloatingModeRunningDescriptor)
		return;

	ModeFunctionBuffer = ModeFunction;

	FLog.PrevMode = RunningMode;

	FloatingModeReserveDescriptor = true;
	ModeSwitchReserveDescriptor = true;

	if (FloatingOnlyOption)
		FloatingOnlyDescriptor = true;

	FLog.IsOnlyFloating = FloatingOnlyDescriptor;
}

void FWM::EndFloatingMode() {
	if (!RunningActivateDescriptor || !FloatingModeRunningDescriptor)
		return;

	FLog.PrevMode = RunningMode;

	FloatingModeEndReserveDescriptor = true;
	ModeSwitchReserveDescriptor = true;
}

void FWM::ResetControlState(OBJ_BASE* Object) {
	Object->ResetControlState();
}

size_t FWM::Size(Layer TargetLayer) {
	return Container[static_cast<int>(TargetLayer)].size();
}

//////// private ///////////////

void FWM::ChangeMode() {
	if (FloatingModeReserveDescriptor) {
		PrevRunningMode = RunningMode;
		RunningMode = ModeFunctionBuffer();

		if(ControllerBuffer)
			ControllerBuffer();

		FloatingModeRunningDescriptor = true;

		FLog.CurrentMode = RunningMode;
		FLog.Log(LogType::START_FLOATING_MODE);
	}

	if (FloatingModeEndReserveDescriptor) {
		ClearFloatingObject();
		RunningMode = PrevRunningMode;

		if(ControllerBackUpBuffer)
			ControllerBackUpBuffer();

		FloatingModeRunningDescriptor = false;
		FloatingOnlyDescriptor = false;

		FLog.CurrentMode = RunningMode;
		FLog.Log(LogType::END_FLOATING_MODE);
	}
	
	if(!FloatingModeReserveDescriptor && !FloatingModeEndReserveDescriptor) {
		ClearAll();
		RunningMode = ModeFunctionBuffer();

		if(ControllerBuffer)
			ControllerBuffer();

		FLog.CurrentMode = RunningMode;
		FloatingOnlyDescriptor = false;
	}

	FLog.IsOnlyFloating = FloatingOnlyDescriptor;

	if (FLog.CurrentMode == FLog.PrevMode)
		FLog.ErrorLog(LogType::ERROR_SAME_MODE);

	FLog.CurrentMode = RunningMode;
	FLog.Log(LogType::MODE_SWITCH);

	FloatingModeReserveDescriptor = false;
	FloatingModeEndReserveDescriptor = false;
	ModeSwitchReserveDescriptor = false;
	ModeSwitchingDescriptor = false;
}

void FWM::ClearDeleteTargetObject(int i) {
	for (auto It = begin(Container[i]); It != end(Container[i]);) {
		if ((*It)->DeleteDescriptor) {
			delete* It;
			*It = nullptr;
			It = Container[i].erase(It);
			continue;
		}
		++It;
	}

	std::erase_if(ObjectList, [](const std::pair<std::string, OBJ_BASE*>& Object) { return !Object.second; });
}

void FWM::ClearFloatingObject() {
	for (int i = 0; i < Num; ++i) {
		for (auto It = begin(Container[i]); It != end(Container[i]);) {
			if ((*It)->FloatingDescriptor) {
				delete* It;
				*It = nullptr;
				It = Container[i].erase(It);
				continue;
			}
			++It;
		}
	}

	std::erase_if(ObjectList, [](const std::pair<std::string, OBJ_BASE*>& Object) { return !Object.second; });
}

void FWM::ClearAll() {
	for (int i = 0; i < Num; ++i) {
		for (auto It = begin(Container[i]); It != end(Container[i]);) {
			if ((*It)->ObjectTag != "FWM_DUMMY") {
				delete* It;
				*It = nullptr;
				It = Container[i].erase(It);
				continue;
			}
			++It;
		}
	}

	ObjectList.clear();
}