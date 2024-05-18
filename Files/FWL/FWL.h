#ifndef FWL_H
#define FWL_H
#pragma warning(disable: 28020)

#include <algorithm>
#include <array>
#include <deque>
#include <vector>
#include <ctime>

#include "FWL_config.h"
#include "FWL_messege.h"

#ifdef USING_FWL
#if N_MAIN_LAYER

class FWL {
private:
	std::array<std::deque<MAIN_CLS*>, N_MAIN_LAYER> MainCont{};
	std::vector<std::string> MainModeList;

	std::string              PrevModeName{};
	std::string              CurrentModeName{};
	std::string              CurrentMainModeName{};

	bool                     InStartMainMode{};
	bool					 MainModeInitState{};


#ifdef USING_SUB_MODE
#if N_SUB_LAYER
	std::array<std::deque<SUB_CLS*>, N_SUB_LAYER> SubCont{};
	std::vector<std::string> SubModeList;

	std::string              CurrentSubModeName{};

	bool					 InStartSubMode{};
	bool					 SubModeInitState{};
#endif
#endif

	bool					 ModeSwitchState{};
	bool					 PauseState{};

	// for debug messeges
	bool					 DBG_INIT_SUB_MODE{};
	bool					 DBG_SWITCH_MAIN_MODE{};
	bool					 DBG_SWITCH_SUB_MODE{};


// frame time
#ifdef USING_FRAME_TIME
	clock_t					 StartTime{}, EndTime{};
	double					 FrameTime{};
	double					 FrameTimeMulValue = 1;
#endif

	// callable function type
	typedef void (*func)(void);

	// FWL error / FWL debug messege processing class
	FWL_MESSEGE				 F_Messege;


public:

#ifdef USING_FRAME_TIME
	// Sets the frame-time multiple
	void SetFrameTimeMul(double value) { FrameTimeMulValue = value; }

	// Sets the framerate multiple to its initial setting (x1)
	void ResetFrameTimeMul() { FrameTimeMulValue = 1; }

#ifdef USING_FRAME_TIME_OUTSIDE
	// Enter the frame time from the outside
	void InputFrameTime(double time) { FrameTime = time; }
#endif

	// Multiply the frame time
	double FT(double movement, double additional_value = 1) { 
		return movement * FrameTime * FrameTimeMulValue * additional_value; 
	}

#endif

	// Returns the name of the currently running mode
	std::string CurrentMode() { return CurrentModeName; }

	// Returns the name of the main mode currently running
	std::string CurrentMainMode() { return CurrentMainModeName; }

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
	// Returns the name of the sub mode currently running
	std::string CurrentSubMode() { return CurrentSubModeName; }
#endif
#endif

	// Runs a framework routine
	void Routine() {
		if (MainModeInitState) {
#ifdef USING_FRAME_TIME
#ifndef USING_FRAME_TIME_OUTSIDE
			StartTime = clock();
#endif
#endif
			// Main mode routine
			for (int i = 0; i < N_MAIN_LAYER; ++i) {
				for (auto It = MainCont[i].begin(); It != MainCont[i].end();) {
					auto Ptr = *It;

					// If the object is not nullptr, update and output the object
					if (Ptr) {
						if (!PauseState && !ModeSwitchState) {
							Ptr->Update();
							Ptr->CheckCollision();
						}
						Ptr->Render();
					}

					// Verify that the updated object is nullptr, and if it is nullptr, go to the else statement
					if(Ptr)
						++It; 

					// If the object is nullptr, delete the pointer that pointed to it from the container
					else
						It = MainCont[i].erase(It);
				}
			}

#ifdef USING_SUB_MODE
#if N_SUB_LAYER

			// Sub mode routine
			if (SubModeInitState) {
				for (int i = 0; i < N_SUB_LAYER; ++i) {
					for (auto It = SubCont[i].begin(); It != SubCont[i].end();) {
						auto Ptr = *It;

						// If the object is not nullptr, update and output the object
						if (Ptr) {
							if (!ModeSwitchState) {
								Ptr->Update();
								Ptr->CheckCollision();
							}
							Ptr->Render();
						}

						// Verify that the updated object is nullptr, and if it is nullptr, go to the else statement
						if (Ptr)
							++It;

						// If the object is nullptr, delete the pointer that pointed to it from the container
						else
							It = SubCont[i].erase(It);
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




	// Initializes the main mode
	void InitMainMode(func MainModeFunc, std::string MainModeName) {
		if (MainModeInitState)
			F_Messege.MAIN_ERROR(RPT_M_INIT);


		// Starts main mode initialization
		InStartMainMode = true;

		// Loads a list of modes created in FWL_config into the framework
		MODELIST M;
		MainModeList = M.MainModeList;

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
		SubModeList = M.SubModeList;
#endif 
#endif
		// If the mode name passed to the parameter cannot be found in the mode list you created in FWL_config, exit the framework
		auto Target = std::find(MainModeList.begin(), MainModeList.end(), MainModeName);
		if (Target == MainModeList.end())
			F_Messege.MAIN_ERROR(INV_M_MODE_IN_M_INIT, MainModeName);

		F_Messege.SV_NXT_M_MODE_NAME(MainModeName);

		// Run the mode start function
		MainModeFunc();

		// If you successfully ran the mode start function, specify the mode name for the current mode and current main mode
		CurrentModeName = MainModeName;
		CurrentMainModeName = MainModeName;

		F_Messege.SV_CURR_M_MODE_NAME(MainModeName);
		F_Messege.FWL_INIT_MSG();

		// Enable Main Mode Routine
		MainModeInitState = true;
	}




	// Switch the currently running main mode to another main mode
	void SwitchMainMode(func MainModeFunc, std::string MainModeName) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (CurrentModeName == MainModeName)
			F_Messege.MAIN_ERROR(SAME_M_MODE_IN_SWITCH);


		F_Messege.SV_PREV_M_MODE_NAME(CurrentModeName);

		// If the mode name passed to the parameter cannot be found in the mode list you created in FWL_config, exit the framework
		auto Target = std::find(MainModeList.begin(), MainModeList.end(), MainModeName);
		if (Target == MainModeList.end())
			F_Messege.MAIN_ERROR(INV_M_MODE_IN_SWITCH, MainModeName);

		F_Messege.SV_NXT_M_MODE_NAME(MainModeName);

		// When the mode switch state is activated, all object updates in the main mode and submode are interrupted
		ModeSwitchState = true;

		// debug value
		DBG_SWITCH_MAIN_MODE = true;

#ifdef USING_SUB_MODE
#if N_SUB_LAYER
		if (SubModeInitState)
			EndSubMode();
#endif
#endif

		// Delete all objects in the main mode container
		ClearMainAll();

		// Run the mode start function
		MainModeFunc();

		// If the mode start function has been successfully executed, change the name of the current mode and current main mode
		CurrentModeName = MainModeName;
		CurrentMainModeName = MainModeName;

		F_Messege.SV_CURR_M_MODE_NAME(MainModeName);
		F_Messege.MAIN_MODE_SWITCH_MSG();

		// debug value
		DBG_SWITCH_MAIN_MODE = false;

		// The mode transition state is disabled and the main mode and submode object update resumes
		ModeSwitchState = false;
	}




	// Adds an object to a specific main mode layer
	void AddMainObj(MAIN_CLS*&& Object, int Layer) {
		if (!InStartMainMode)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (DBG_SWITCH_SUB_MODE || DBG_INIT_SUB_MODE)
			F_Messege.SUB_ERROR(INC_FUNC_IN_S_SWITCH);
		if (Layer >= N_MAIN_LAYER || Layer < 0) {
			if(MainModeInitState)
				F_Messege.MAIN_ERROR(LOB_M_IN_ADD);
			if (ModeSwitchState)
				F_Messege.MAIN_ERROR(LOB_M_IN_SWITCH);
			else
				F_Messege.MAIN_ERROR(LOB_M_IN_M_INIT);
		}

		
		MainCont[Layer].push_back(Object);
	}




	// Delete an object from the main mode container, use within the object class
	void DeleteMainObj(MAIN_CLS* Object, int Layer) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (Layer >= N_MAIN_LAYER || Layer < 0)
			F_Messege.MAIN_ERROR(LOB_M_IN_DELETE);


		auto Target = std::find(MainCont[Layer].begin(), MainCont[Layer].end(), Object);
		if (Target != MainCont[Layer].end()) {
			delete* Target;
			*Target = nullptr;
		}
		else 
			F_Messege.MAIN_ERROR(UKN_M_IN_DELETE);
	}




	// Returns the number of objects in a specific main mode layer
	size_t MainLayerSize(int Layer) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (Layer >= N_MAIN_LAYER || Layer < 0)
			F_Messege.MAIN_ERROR(LOB_M_IN_SIZE);


		return MainCont[Layer].size();
	}




	// Locate the pointer to a single object in the specific main mode layer
	MAIN_CLS* FindMainObj_Layer_Single(int Layer, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (Layer >= N_MAIN_LAYER || Layer < 0)
			F_Messege.MAIN_ERROR(LOB_M_IN_FIND);


		MAIN_CLS* Obj{};
		bool ObjFind{};

		size_t num = MainCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Ptr = MainObjPtr(Layer, i);

			if (Ptr && Ptr->GetTag() == Tag) {
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




	// Locate the pointer to a single object in the main mode container
	MAIN_CLS* FindMainObj_Entire_Single(std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);


		MAIN_CLS* Obj{};
		bool ObjFind{};

		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			if (ObjFind)
				break;

			size_t num = MainCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Ptr = MainObjPtr(i, j);
				if (Ptr && Ptr->GetTag() == Tag) {
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




	// Locate multiple objects in the specific main mode layer, use with 'for()'
	MAIN_CLS* FindMainObj_Layer_Index(int Layer, int Index, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (Layer >= N_MAIN_LAYER || Layer < 0 )
			F_Messege.MAIN_ERROR(LOB_M_IN_FIND);


		auto Ptr = MainObjPtr(Layer, Index);

		if (Ptr && Ptr->GetTag() == Tag)
			return Ptr;
		else
			return nullptr;
	}


#ifdef USING_SUB_MODE
#if N_SUB_LAYER

	// Initializes the submode
	void InitSubMode(func SubModeFunc, std::string SubModeName, bool MainModePauseOption = false) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (SubModeInitState)
			F_Messege.SUB_ERROR(RPT_S_INIT);


		// Starts sub mode initialization
		InStartSubMode = true;

		// debug value
		DBG_INIT_SUB_MODE = true;

		// If the mode name passed to the parameter cannot be found in the mode list you created in FWL_config, exit the framework
		auto Target = std::find(SubModeList.begin(), SubModeList.end(), SubModeName);
		if (Target == SubModeList.end())
			F_Messege.SUB_ERROR(INV_S_MODE_IN_S_INIT, SubModeName);

		F_Messege.SV_NXT_S_MODE_NAME(SubModeName);

		
		PauseState = true;

		// Run the mode start function
		SubModeFunc();

		// If the mode start function has been successfully executed, 
		// save the main mode that was last executed and specify the current mode and the current sub mode
		PrevModeName = CurrentModeName;
		CurrentModeName = SubModeName;
		CurrentSubModeName = SubModeName;

		F_Messege.SV_CURR_S_MODE_NAME(SubModeName);

		// If MainModePauseOption is set to true, pause the main mode update
		if (!MainModePauseOption)
			PauseState = false;

		F_Messege.SV_PAUSE_STATE(PauseState);
		F_Messege.SUB_INIT_MSG();

		// debug value
		DBG_INIT_SUB_MODE = false;

		// Enable Sub Mode Routine
		SubModeInitState = true;
	}




	// Switch the currently running sub mode to another sub mode
	void SwitchSubMode(func SubModeFunc, std::string SubModeName) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);


		F_Messege.SV_PREV_S_MODE_NAME(CurrentModeName);

		// If the mode name passed to the parameter cannot be found in the mode list you created in FWL_config, exit the framework
		auto Target = std::find(SubModeList.begin(), SubModeList.end(), SubModeName);
		if (Target == SubModeList.end())
			F_Messege.SUB_ERROR(INV_S_MODE_IN_SWITCH, SubModeName);

		F_Messege.SV_NXT_S_MODE_NAME(SubModeName);

		// When the mode switch state is activated, all object updates in the main mode and submode are interrupted
		ModeSwitchState = true;

		// debug value
		DBG_SWITCH_SUB_MODE = true;

		// Delete all objects in the sub mode container
		ClearSubAll();

		// Run the mode start function
		SubModeFunc();

		CurrentModeName = SubModeName;
		CurrentSubModeName = SubModeName;

		F_Messege.SV_CURR_S_MODE_NAME(SubModeName);
		F_Messege.SUB_MODE_SWITCH_MSG();

		// debug value
		DBG_SWITCH_SUB_MODE = false;

		// The mode transition state is disabled and the main mode and submode object update resumes
		ModeSwitchState = false;
	}




	// Delete all objects in the sub mode container and exit the submode
	void EndSubMode() {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);

		// Delete all objects in the sub mode container
		ClearSubAll();

		// Change the current mode to the last main mode you ran, and delete the current submode
		CurrentModeName = PrevModeName;
		CurrentSubModeName = "";

		F_Messege.SUB_END_MSG();

		// The main mode pause is released, and the remaining submode settings are initialized
		PauseState = false;
		InStartSubMode = false;
		SubModeInitState = false;
	}




	// Adds an object to a specific sub mode layer
	void AddSubObj(SUB_CLS*&& Object, int Layer) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (DBG_SWITCH_MAIN_MODE)
			F_Messege.MAIN_ERROR(INC_FUNC_IN_M_SWITCH);
		if (!InStartSubMode)
			F_Messege.SUB_ERROR(INV_S_INIT);
		if (Layer >= N_SUB_LAYER || Layer < 0) {
			if (SubModeInitState)
				F_Messege.SUB_ERROR(LOB_S_IN_ADD);
			if (ModeSwitchState) 
				F_Messege.SUB_ERROR(LOB_S_IN_SWITCH);
			else
				F_Messege.SUB_ERROR(LOB_S_IN_S_INIT);
		}
		

		SubCont[Layer].push_back(Object);
	}




	// Delete an object from the sub mode container, use within the object class
	void DeleteSubObj(SUB_CLS* Object, int Layer) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);
		if (Layer >= N_SUB_LAYER || Layer < 0)
			F_Messege.SUB_ERROR(LOB_S_IN_DELETE);

		
		auto Target = std::find(SubCont[Layer].begin(), SubCont[Layer].end(), Object);
		if (Target != SubCont[Layer].end()) {
			delete* Target;
			*Target = nullptr;
		}

		else
			F_Messege.SUB_ERROR(UKN_S_IN_DELETE);
	}




	// Returns the number of objects in a specific sub mode layer
	size_t SubLayerSize(int Layer) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);
		if (Layer >= N_SUB_LAYER || Layer < 0)
			F_Messege.SUB_ERROR(LOB_S_IN_SIZE);


		return SubCont[Layer].size();
	}




	// Locate the pointer to a single object in the specific sub mode layer
	SUB_CLS* FindSubObj_Layer_Single(int Layer, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);
		if (Layer >= N_SUB_LAYER || Layer < 0)
			F_Messege.SUB_ERROR(LOB_S_IN_FIND);


		SUB_CLS* Obj{};
		bool ObjFind{};

		size_t num = SubCont[Layer].size();

		for (int i = 0; i < num; ++i) {
			auto Ptr = SubObjPtr(Layer, i);

			if (Ptr && Ptr->GetTag() == Tag) {
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




	// Locate the pointer to a single object in the sub mode container
	SUB_CLS* FindSubObj_Entire_Single(std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);


		SUB_CLS* Obj{};
		bool ObjFind{};

		for (int i = 0; i < N_SUB_LAYER; ++i) {
			if (ObjFind)
				break;

			size_t num = SubCont[i].size();

			for (int j = 0; j < num; ++j) {
				auto Ptr = SubObjPtr(i, j);
				if (Ptr && Ptr->GetTag() == Tag) {
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




	// Locate multiple objects in the specific sub mode layer, use with 'for()'
	SUB_CLS* FindSubObj_Layer_Index(int Layer, int Index, std::string Tag) {
		if (!MainModeInitState)
			F_Messege.MAIN_ERROR(INV_M_INIT);
		if (!SubModeInitState)
			F_Messege.SUB_ERROR(INV_S_INIT);
		if (Layer >= N_SUB_LAYER || Layer < 0)
			F_Messege.SUB_ERROR(LOB_S_IN_FIND);


		auto Ptr = SubObjPtr(Layer, Index);

		if (Ptr && Ptr->GetTag() == Tag)
			return Ptr;
		else
			return nullptr;
	}

#endif
#endif


private:
	// Locate the object pointer through a specific index of a specific main mode layer
	MAIN_CLS* MainObjPtr(int Layer, int Index) {
		if (Index >= MainCont[Layer].size())
			return nullptr;
		else
			return MainCont[Layer][Index];
	}




	// Delete all objects that exist in a specific main mode layer
	void ClearMainLayer(int Layer) {
		for (auto It = MainCont[Layer].begin(); It != MainCont[Layer].end();) {
			delete* It;
			*It = nullptr;

			It = MainCont[Layer].erase(It);
		}
	}




	// Delete all objects that exist in the main mode container
	void ClearMainAll() {
		for (int i = 0; i < N_MAIN_LAYER; ++i) {
			for (auto It = MainCont[i].begin(); It != MainCont[i].end();) {
				delete* It;
				*It = nullptr;

				It = MainCont[i].erase(It);
			}
		}
	}



#ifdef USING_SUB_MODE
#if N_SUB_LAYER

	// Locate the object pointer through a specific index of a specific sub mode layer
	SUB_CLS* SubObjPtr(int Layer, int Index) {
		if (Index >= SubCont[Layer].size())
			return nullptr;
		else
			return SubCont[Layer][Index];
	}




	// Delete all objects that exist in a specific sub mode layer
	void ClearSubLayer(int Layer) {
		for (auto It = SubCont[Layer].begin(); It != SubCont[Layer].end();) {
			delete* It;
			*It = nullptr;

			It = SubCont[Layer].erase(It);
		}
	}




	// Delete all objects that exist in the sub mode container
	void ClearSubAll() {
		for (int i = 0; i < N_SUB_LAYER; ++i) {
			for (auto It = SubCont[i].begin(); It != SubCont[i].end();) {
				delete* It;
				*It = nullptr;

				It = SubCont[i].erase(It);
			}
		}
	}

#endif
#endif

};

#endif
#endif

#endif