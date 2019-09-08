#pragma once
#include "InputManager.h"
#include "DisplayBuffer.h"
//#include "Scene.h"

namespace MySmallRadioApp
{
	class MyLoop
	{
	public:
		MyLoop();
		~MyLoop();

		void Loop();
	private:

		void Input();
		void Update();
		void Render();

		//Scene* currentMenu;
		InputManager* IMinstance;
		DisplayBuffer* BufferInstance;
		unsigned char selectedOpt;

		std::string filePathStr= std::string();
		std::wstring soundPath;

		bool inputHasbeenUpdated = true;
		bool sceneHasBeenUpdated = false;

		std::map<unsigned int, std::wstring> menuOptions;
		bool inputStringEnabled = false;
		unsigned int currentSelection = 0;

		std::wstring menuScreen;
		void PrintMainMenu();

		void TakeInput(unsigned int);
		void TakeInputString(const std::string&);

		bool GetInputMode();
		void SetInputMode(bool, unsigned int selection = 0);

		std::wstring folderPath = L"D:\\sound_programming\\MyFMODsolution\\audioSamples\\";		

		FmodStaticLib::MyWrapperLib* myPlayer = new FmodStaticLib::MyWrapperLib();

		void SetLoadSoundOptions();
		void ChangeSoundOptions(const std::string&);
		

		bool changeSoundOpts;
		bool activate3DsoundOnLoad =false;
		bool activateLoopOnLoad = false;
		bool activateStreamOnLoad = false;

		void SetPosition();
		void SetdBvolume();
		float volumedB = 3.0f;
		FmodStaticLib::Vector3* soundPosition = new FmodStaticLib::Vector3(0.0f,0.0f,0.0f);

		int selectedChannel = 0;
		int channelToSelect = 0;

	};

}
