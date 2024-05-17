#ifndef FWL_H
#define FWL_H
#pragma warning(disable: 28020)

#include <algorithm>
#include <array>
#include <deque>
#include <vector>
#include <ctime>

#include "FWL_config.h"

#ifdef USING_FWL
#if N_MAIN_LAYER

class FWL {
private:
	std::array<std::deque<MAIN_CLS*>, N_MAIN_LAYER> MainCont{};
	std::array<std::deque<MAIN_CLS*>, N_MAIN_LAYER> MainTempCont{};

	std::vector<std::string> MainModeList;

	std::string              PrevModeName{};
	std::string              CurrentModeName{};
	std::string              CurrentMainModeName{};

	bool					 MainModeInitState{};


#ifdef USING_SUB_MODE
#if N_SUB_LAYER
	std::array<std::deque<SUB_CLS*>, N_SUB_LAYER> SubCont{};
	std::array<std::deque<SUB_CLS*>, N_SUB_LAYER> SubTempCont{};

	std::vector<std::string> SubModeList;

	std::string              CurrentSubModeName{};
	bool					 InEndSubMode{};
#endif
#endif

	bool					 ModeSwitchState{};
	bool					 PauseState{};


// frame time
#ifdef USING_FRAME_TIME
	clock_t					 StartTime{}, EndTime{};
	double					 FrameTime{};
	double					 FrameTimeMulValue = 1;
#endif

	typedef void (*func)(void);



public:

#ifdef USING_FRAME_TIME
	// set frame time mul value
	void SetFrameTimeMul(double value) { FrameTimeMulValue = value; }

	//reset frame time mul value to defalut
	void ResetFrameTimeMul() { FrameTimeMulValue = 1; }

#ifdef USING_FRAME_TIME_OUTSIDE
	// input frame time from outside
	void InputFrameTime(double time) { FrameTime = time; }
#endif

	// multiply frame time
	double FT(double movement, double additional_value = 1) { 
		return movement * FrameTime * FrameTimeMulValue * additional_value; 
	}

#endif

	// get current mode name
	std::string CurrentMode() const { return CurrentModeName; }

	// get current main mode name
	std::string CurrentMainMode() const { return CurrentMainModeName; }

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
	// get current sub mode name
	std::string CurrentSubMode() const { return CurrentSubModeName; }
#endif
#endif

	void Routine() {
		if (MainModeInitState) {
#ifdef USING_FRAME_TIME
#ifndef USING_FRAME_TIME_OUTSIDE
			StartTime = clock();
#endif
#endif
			for (int i = 0; i < N_MAIN_LAYER; ++i) {
				for (auto It = MainCont[i].begin(); It != MainCont[i].end();) {
					auto Ptr = *It;
					if (Ptr != nullptr) {
						if (!PauseState && !ModeSwitchState) {
							Ptr->Update();
							Ptr->CheckCollision();
						}
						Ptr->Render();
						Ptr->CheckDelete();

						++It; 
					}

					else
						It = MainCont[i].erase(remove(MainCont[i].begin(), MainCont[i].end(), Ptr));
				}
			}

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
			if (SubModeInitState) {
				for (int i = 0; i < N_SUB_LAYER; ++i) {
					for (auto It = SubCont[i].begin(); It != SubCont[i].end();) {
						auto Ptr = *It;
						if (Ptr != nullptr) {
							if (!ModeSwitchState) {
								Ptr->Update();
								Ptr->CheckCollision();
							}
							Ptr->Render();
							Ptr->CheckDelete();

							++It;
						}

						else
							It = SubCont[i].erase(remove(SubCont[i].begin(), SubCont[i].end(), Ptr));
					}
				}
			}
#endif
#endif

#ifdef USING_FRAME_TIME
#ifndef USING_FRAME_TIME_OUTSIDE
			EndTime = clock();
			FrameTime = (double)(EndTime - StartTime) / 1000;
#endif
#endif
		}
	}




	//init FWL
	void InitMainMode(func MainModeFunc, std::string MainModeName) {
		MODELIST M;
		MainModeList = M.MainModeList;

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
		SubModeList = M.SubModeList;
#endif 
#endif

		auto Target = std::find(MainModeList.begin(), MainModeList.end(), MainModeName);
		if (Target == MainModeList.end())
			exit(1);

		MainModeFunc();

		CurrentModeName = MainModeName;
		CurrentMainModeName = MainModeName;

		MainModeInitState = true;
	}




	// change current mode
	void SwitchMainMode(func MainModeFunc, std::string MainModeName) {
		auto Target = std::find(MainModeList.begin(), MainModeList.end(), MainModeName);
		if (Target == MainModeList.end())
			exit(1);

		ModeSwitchState = true;

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
		if (SubModeInitState)
			EndSubMode();
#endif
#endif

		MainModeFunc();

		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			ClearMainLayer(i);
			MainCont[i] = MainTempCont[i];
			MainTempCont[i].clear();
		}

		CurrentModeName = MainModeName;
		CUrrentMainModeName = MainModeName;

		ModeSwitchState = false;
	}




	// add object
	void AddMainObj(MAIN_CLS*&& Object, int Layer) {
		if (ModeSwitchState)
			MainTempCont[Layer].push_back(Object);
		
		else 
			MainCont[Layer].push_back(Object);
	}




	// delete object
	void DeleteMainObj(MAIN_CLS* Object, int Layer) {
		auto Target = std::find(MainCont[Layer].begin(), MainCont[Layer].end(), Object);
		if (Target != MainCont[Layer].end()) {
			delete* Target;
			*Target = nullptr;
		}
		else 
			exit(1);
	}




	// delete main object outside of object class
	void DeleteMainObj_Layer_Single(int Layer, std::string Tag) {
		size_t num = MainCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Target = MainObjPtr(Layer, i);
			if (Target != nullptr && Target->GetTag() == Tag) {
				Target->ActivateDeleteFlag(true);
				break;
			}
		}
	}




	void DeleteMainObj_Layer_All(int Layer, std::string Tag) {
		size_t num = MainCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Target = MainObjPtr(Layer, i);
			if (Target != nullptr && Target->GetTag() == Tag)
				Target->ActivateDeleteFlag(true);
		}
	}




	void DeleteMainObj_Entire_Single(std::string Tag) {
		bool ObjFind{};

		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			if (ObjFind)
				break;

			size_t num = MainCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Target = MainObjPtr(i, j);
				if (Target != nullptr && Target->GetTag() == Tag) {
					Target->ActivateDeleteFlag(true);
					ObjFind = true;
					break;
				}
			}
		}
	}




	void DeleteMainObj_Entire_All(std::string Tag) {
		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			size_t num = MainCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Target = MainObjPtr(i, j);
				if (Target != nullptr && Target->GetTag() == Tag)
					Target->ActivateDeleteFlag(true);
			}
		}
	}




	void DeleteMainObj_Layer_Index(int Layer, int Index, std::string Tag) {
		auto Target = MainObjPtr(Layer, Index);

		if (Target != nullptr && Target->GetTag() == Tag)
			Target->ActivateDeleteFlag(true);
	}




	// return number of objects of specific layer
	size_t MainLayerSize(int Layer) {
		return MainCont[Layer].size();
	}




	// find single object ptr on layer
	MAIN_CLS* FindMainObj_Layer_Single(int Layer, std::string Tag) {
		MAIN_CLS* Obj{};
		bool ObjFind{};

		size_t num = MainCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Ptr = MainObjPtr(Layer, i);

			if (Ptr != nullptr && Ptr->GetTag() == Tag) {
				Obj = Ptr;
				ObjFind = true;
				break;
			}
		}

		if (ObjFind)
			return Obj;
		else
			return nullptr;
	}




	// find single object on entire container
	MAIN_CLS* FindMainObj_Entire_Single(std::string Tag) {
		MAIN_CLS* Obj{};
		bool ObjFind{};

		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			if (ObjFind)
				break;

			size_t num = MainCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Ptr = MainObjPtr(i, j);
				if (Ptr != nullptr && Ptr->GetTag() == Tag) {
					Obj = Ptr;
					ObjFind = true;
					break;
				}
			}
		}

		if (ObjFind)
			return Obj;
		else
			return nullptr;
	}




	// find many objects ptr on layer, use with for()
	MAIN_CLS* FindMainObj_Layer_Index(int Layer, int Index, std::string Tag) {
		auto Ptr = MainObjPtr(Layer, Index);

		if (Ptr != nullptr && Ptr->GetTag() == Tag)
			return Ptr;
		else
			return nullptr;
	}


#ifdef USING_SUB_MODE
#if N_SUB_LAYER

	//init sub mode
	void InitSubMode(func SubModeFunc, std::string SubModeName, bool MainModePauseOption = false) {
		InEndSubMode = false;

		auto Target = std::find(SubModeList.begin(), SubModeList.end(), SubModeName);
		if (Target == SubModeList.end())
			exit(1);

		PauseState = true;

		SubModeFunc();

		PrevModeName = CurrentModeName; // save main mode name
		CurrentModeName = SubModeName;
		CurrentSubModeName = SubModeName;


		if (!MainModePauseOption)  // stop main mode's update if pause option is true
			PauseState = false;

		SubModeInitState = true;
	}




	// change mode of popup
	void SwitchSubMode(func SubModeFunc, std::string SubModeName) {
		auto Target = std::find(SubModeList.begin(), SubModeList.end(), SubModeName);
		if (Target == SubModeList.end())
			exit(1);

		ModeSwitchState = true;

		SubModeFunc();

		for (int i = 0; i < N_SUB_LAYER; ++i) {
			ClearSubLayer(i);
			SubCont[i] = SubTempCont[i];
			SubTempCont[i].clear();
		}

		CurrentModeName = SubModeName;
		CurrentSubModeName = SubModeName;

		ModeSwitchState = false;
	}




	// end popup mode
	void EndSubMode() {
		InEndSubMode = true;

		ClearSubAll();

		CurrentModeName = PrevModeName;
		CurrentSubModeName = "";

		PauseState = false;
		InStartSubMode = false;
		SubModeInitState = false;
	}




	//add popup object
	void AddSubObj(SUB_CLS*&& Object, int Layer) {
		if (ModeSwitchState) 
			SubTempCont[Layer].push_back(Object);
		else
			SubCont[Layer].push_back(Object);
	}




	// delete popup object
	void DeleteSubObj(SUB_CLS* Object, int Layer) {
		auto Target = std::find(SubCont[Layer].begin(), SubCont[Layer].end(), Object);
		if (Target != SubCont[Layer].end()) {
			delete* Target;
			*Target = nullptr;
		}

		else
			exit(1);
	}




	// delete main object outside of object class
	void DeleteSubObj_Layer_Single(int Layer, std::string Tag) {
		size_t num = SubCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Target = SubObjPtr(Layer, i);
			if (Target != nullptr && Target->GetTag() == Tag) {
				Target->ActivateDeleteFlag(true);
				break;
			}
		}
	}




	void DeleteSubObj_Layer_All(int Layer, std::string Tag) {
		size_t num = SubCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Target = SubObjPtr(Layer, i);
			if (Target != nullptr && Target->GetTag() == Tag)
				Target->ActivateDeleteFlag(true);
		}
	}




	void DeleteSubObj_Entire_Single(std::string Tag) {
		bool ObjFind{};

		for (int i = 0; i < N_SUB_LAYER; ++i) {
			if (ObjFind)
				break;

			size_t num = SubCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Target = SubObjPtr(i, j);
				if (Target != nullptr && Target->GetTag() == Tag) {
					Target->ActivateDeleteFlag(true);
					ObjFind = true;
					break;
				}
			}
		}
	}





	void DeleteSubObj_Entire_All(std::string Tag) {
		for (int i = 0; i < N_SUB_LAYER; ++i) {
			size_t num = SubCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Target = SubObjPtr(i, j);
				if (Target != nullptr && Target->GetTag() == Tag)
					Target->ActivateDeleteFlag(true);
			}
		}
	}




	void DeleteSubObj_Layer_Index(int Layer, int Index, std::string Tag) {
		auto Target = SubObjPtr(Layer, Index);

		if (Target != nullptr && Target->GetTag() == Tag)
			Target->ActivateDeleteFlag(true);
	}




	// return number of popup objects of specific popup layer
	size_t SubLayerSize(int Layer) {
		return SubCont[Layer].size();
	}




	// find popup object ptr on layer
	SUB_CLS* FindSubObj_Layer_Single(int Layer, std::string Tag) {
		SUB_CLS* Obj{};
		bool ObjFind{};

		size_t num = SubCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Ptr = SubObjPtr(Layer, i);

			if (Ptr != nullptr && Ptr->GetTag() == Tag) {
				Obj = Ptr;
				ObjFind = true;
				break;
			}
		}

		if (ObjFind)
			return Obj;
		else
			return nullptr;
	}




	// find popup object on entire container
	SUB_CLS* FindSubObj_Entire_Single(std::string Tag) {
		SUB_CLS* Obj{};
		bool ObjFind{};

		for (int i = 0; i < N_SUB_LAYER; ++i) {
			if (ObjFind)
				break;

			size_t num = SubCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Ptr = SubObjPtr(i, j);
				if (Ptr != nullptr && Ptr->GetTag() == Tag) {
					Obj = Ptr;
					ObjFind = true;
					break;
				}
			}
		}

		if (ObjFind)
			return Obj;
		else
			return nullptr;
	}




	// find many popup object ptr on layer, use with for()
	SUB_CLS* FindSubObj_Layer_Index(int Layer, int Index, std::string Tag) {
		auto Ptr = SubObjPtr(Layer, Index);

		if (Ptr != nullptr && Ptr->GetTag() == Tag)
			return Ptr;
		else
			return nullptr;
	}

#endif
#endif


private:
	// get obj ptr from other object
	MAIN_CLS* MainObjPtr(int Layer, int Index) {
		if (Index >= MainCont[Layer].size())
			return nullptr;
		else
			return MainCont[Layer][Index];
	}




	// delete objects of specific layer
	void ClearMainLayer(int Layer) {
		for (auto It = MainCont[Layer].begin(); It != MainCont[Layer].end();) {
			auto Target = std::find(MainCont[Layer].begin(), MainCont[Layer].end(), *It);

			delete* Target;
			*Target = nullptr;

			++It;
		}
	}




	// delete all object
	void ClearMainAll() {
		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			for (auto It = MainCont[i].begin(); It != MainCont[i].end();) {
				auto Target = std::find(MainCont[i].begin(), MainCont[i].end(), *It);

				delete* Target;
				*Target = nullptr;

				++It;
			}
		}
	}



#ifdef USING_SUB_MODE
#if N_SUB_LAYER

	// get ptr from other popup object
	SUB_CLS* SubObjPtr(int Layer, int Index) {
		if (Index >= SubCont[Layer].size())
			return nullptr;
		else
			return SubCont[Layer][Index];
	}




	// delete popup objects of specific popup layer
	void ClearSubLayer(int Layer) {
		for (auto It = SubCont[Layer].begin(); It != SubCont[Layer].end();) {
			auto Target = std::find(SubCont[Layer].begin(), SubCont[Layer].end(), *It);

			delete* Target;
			*Target = nullptr;

			++It;
		}

		if (InEndSubMode)
			SubCont[Layer].clear();
	}




	// delete popup object all
	void ClearSubAll() {
		for (int i = 0; i < N_SUB_LAYER; ++i) {
			for (auto It = SubCont[i].begin(); It != SubCont[i].end();) {
				auto Target = std::find(SubCont[i].begin(), SubCont[i].end(), *It);

				delete* Target;
				*Target = nullptr;

				++It;
			}
		}

		if (InEndSubMode)
			for (int i = 0; i < N_SUB_LAYER; ++i)
				SubCont[i].clear();
	}

#endif
#endif

};

#endif
#endif

#endif