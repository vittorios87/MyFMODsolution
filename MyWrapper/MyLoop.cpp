#include "pch.h"
#include "MyLoop.h"

using namespace FmodStaticLib;

namespace MySmallRadioApp
{

	MyLoop::MyLoop()
	{

		std::initializer_list<std::pair<unsigned int, std::wstring>> menuOptionsList =
		{
			std::pair<unsigned int, std::wstring>(76,L"L. load sound"),
			std::pair<unsigned int, std::wstring>(108,L"l. load sound in the first empty channel (with loading)"),
			std::pair<unsigned int, std::wstring>(81, L"Q. exit"),
			std::pair<unsigned int, std::wstring>(80,L"P. Play sound on a single channel (also down arrow)"),
			std::pair<unsigned int, std::wstring>(112,L"p. Play sounds of all channels"),
			std::pair<unsigned int, std::wstring>(32,L"space. Pause all channels"),
			std::pair<unsigned int, std::wstring>(83,L"S. Stop sound on a single channel"),
			std::pair<unsigned int, std::wstring>(115,L"s. Stop sounds of all channels"),						
			std::pair<unsigned int, std::wstring>(99,L"c. Select a channel"),
			std::pair<unsigned int, std::wstring>(43,L"+. Increase volume selected channel"),
			std::pair<unsigned int, std::wstring>(45,L"-. Decrease volume selected channel"),						
			std::pair<unsigned int, std::wstring>(72,L"up arrow. Pan center selected channel"),
			std::pair<unsigned int, std::wstring>(77,L"right arrow. Pan right selected channel"),
			std::pair<unsigned int, std::wstring>(75,L"left arrow. Pan left selected channel"),
		};

		for (std::pair<unsigned int, std::wstring> option : menuOptionsList)
		{
			menuOptions.emplace(option);
		}
		inputStringEnabled = false;
		inputHasbeenUpdated = true;
		sceneHasBeenUpdated = false;

		IMinstance = InputManager::GetInstance();
		BufferInstance = DisplayBuffer::GetInstance();
	}


	MyLoop::~MyLoop()
	{
	}

	void MyLoop::Loop()
	{
		while (true)
		{
			Update();
			Render();
			Input();

		}
	}
	void MyLoop::Input()

	{
		IMinstance->Input();
		if (IMinstance->ButtonPressed())
		{
			selectedOpt = IMinstance->GetButtonPressed();
			if (GetInputMode())
			{
				if ((unsigned int)selectedOpt == 13) {
					if (currentSelection == 76 || currentSelection == 108)
					{
						filePathStr.clear();
						filePathStr.append(folderPath.begin(), folderPath.end());
						filePathStr.append(soundPath.begin(), soundPath.end());
						TakeInputString(filePathStr);

					}
					else
					{
						filePathStr.clear();
						filePathStr.append(soundPath.begin(), soundPath.end());

						TakeInputString(filePathStr);
					}
				}
				else
				{
					if ((unsigned int)selectedOpt == 8)
					{
						if (soundPath.size() != 0)
						{
							soundPath.pop_back();
						}
					}
					else if ((unsigned int)selectedOpt == 24)
					{
						soundPath.clear();
					}
					else {
						soundPath.append(1, selectedOpt);
						
					}
					inputHasbeenUpdated = true;
				}


			}
			else
			{
				TakeInput(selectedOpt);
			}


		}

	}

	void MyLoop::Update()
	{
		if (inputHasbeenUpdated)
		{
			PrintMainMenu();
			sceneHasBeenUpdated = true;
			inputHasbeenUpdated = false;
		}
	}
	void MyLoop::Render()
	{
		if (sceneHasBeenUpdated)
		{

			BufferInstance->writeBuffer(menuScreen);
			BufferInstance->printBuffer(GetInputMode());
			sceneHasBeenUpdated = false;
		}
	}

	void MyLoop::PrintMainMenu() {

		menuScreen.clear();

		for (auto it = menuOptions.begin(), itEnd = menuOptions.end(); it != itEnd; ++it)
		{

			menuScreen.append(it->second + L"\n\n");

		}
		menuScreen.append(L"SELECTED CHANNEL:  "+std::to_wstring(selectedChannel) + L"\n\n");
		menuScreen.append(myPlayer->DisplayChannelsState());

		if (GetInputMode()) {
			if (currentSelection == 76 || currentSelection == 108)
			{
				menuScreen.append(folderPath + soundPath + L"\n\n");
			}
			else if (currentSelection == 80  )
			{
				menuScreen.append(L"Channel to play: " + soundPath + L"\n\n");
			}
			else if (currentSelection == 83)
			{
				menuScreen.append(L"Channel to stop: " + soundPath + L"\n\n");

			}
			else if (currentSelection == 99)
			{
				menuScreen.append(L"Channel to control: "+soundPath + L"\n\n");
			}
		}

	}

	void MyLoop::TakeInput(unsigned int selection)
	{
		std::map<unsigned int, std::wstring>::iterator foundIt = menuOptions.find(selection);

		if (foundIt != menuOptions.cend())
		{
			switch (foundIt->first)
			{
			case 99:
				soundPath.clear();
				SetInputMode(true, selection);
				break;
			case 76:
				soundPath.clear();
				SetInputMode(true, selection);
				break;
			case 108:
				soundPath.clear();
				SetInputMode(true, selection);
				break;
			case 81:
				exit(0);
			case 80: /*P  play one channel*/
				soundPath.clear();
				SetInputMode(true, selection);
				break;

			case 112:/*p  play all channels*/
				myPlayer->PlayAllChannels();
				SetInputMode(false, selection);
				break;

			case 32: /*space bar  pause*/
				myPlayer->PauseAllChannels();
				SetInputMode(false, selection);
				break;

			case 83: /*S  stop one channel*/
				soundPath.clear();
				SetInputMode(true, selection);
				break;

			case 115:/*s stop all channels*/
				myPlayer->StopAllChannels();
				SetInputMode(false, selection);
				break;
			case 43:
				myPlayer->IncreaseChannelVolume(selectedChannel);
				SetInputMode(false, selection);
				break;

			case 45:
				myPlayer->DecreaseChannelVolume(selectedChannel);
				SetInputMode(false, selection);
				break;
			case 72:
				myPlayer->CenterChannelPan(selectedChannel);
				SetInputMode(false, selection);
				break;
			case 77:
				myPlayer->MoveChannelPanRight(selectedChannel);
				SetInputMode(false, selection);
				break;
			case 75:
				myPlayer->MoveChannelPanLeft(selectedChannel);
				SetInputMode(false, selection);
				break;
			default:
				SetInputMode(false, selection);
				break;
			}
		}
		inputHasbeenUpdated = true;
	}
	void MyLoop::TakeInputString(const std::string& filePath)
	{

		//std::cout << currentSelection << std::endl;
		//std::cout << filePathStr << std::endl;

		

		switch (currentSelection)
		{
		case 76:
			SetInputMode(false);
			SetLoadSoundOptions();
			myPlayer->LoadSound(filePath, activate3DsoundOnLoad, activateLoopOnLoad, activateStreamOnLoad);
			break;

		case 108:
			SetInputMode(false);
			if (myPlayer->SoundWasLoaded(filePath))
			{
				ChangeSoundOptions(filePath);
			}
			else
			{
				SetLoadSoundOptions();
			}
			SetdBvolume();
			SetPosition();
			myPlayer->LoadSoundOnEmptyChannel(filePath, *soundPosition, volumedB, activate3DsoundOnLoad, activateLoopOnLoad, activateStreamOnLoad);
			break;
		case 80: /*P  play one channel*/
			myPlayer->PlayChannel(std::atoi(filePath.c_str()));
			SetInputMode(false);
			break;
		case 83: /*S  stop one channel*/
			myPlayer->StopChannel(std::atoi(filePath.c_str()));
			SetInputMode(false);
			break;
		case 99:
			channelToSelect = std::atoi(filePath.c_str());
			if (myPlayer->ChannelIsUsed(channelToSelect))
			{
				selectedChannel = channelToSelect;

			}
			SetInputMode(false);
			break;
		default:
			break;
		}

		inputHasbeenUpdated = true;
	}

	void MyLoop::SetInputMode(bool enableStringInput, unsigned int selection) {
		inputStringEnabled = enableStringInput;
		currentSelection = selection;

	}
	bool MyLoop::GetInputMode()
	{
		return inputStringEnabled;
	}

	void MyLoop::SetLoadSoundOptions()
	{
		std::cout << "3D sound? 0 (N) / 1 (Y)  ";
		std::cin >> activate3DsoundOnLoad;
		std::cout << "Loop this sound? 0 (N) / 1 (Y)  ";
		std::cin >> activateLoopOnLoad;
		std::cout << "Stream this sound? 0 (N) / 1 (Y)  ";
		std::cin >> activateStreamOnLoad;

	}
	void MyLoop::ChangeSoundOptions(const std::string& filePathStr)
	{
		std::cout << "Would you change sound options? 0 (N) / 1 (Y)  ";
		std::cin >> changeSoundOpts;
		if (changeSoundOpts) {
			SetLoadSoundOptions();
			myPlayer->ChangeOptsOfLoadedSound(filePathStr,activate3DsoundOnLoad, activateLoopOnLoad, activateStreamOnLoad);
		}
		else
		{
			myPlayer->GetOptsOfLoadedSound(filePathStr, &activate3DsoundOnLoad, &activateLoopOnLoad, &activateStreamOnLoad);

		}
	}
	void  MyLoop::SetPosition() {
		if (activate3DsoundOnLoad)
		{
			std::cout << "Set position: 3 floats\n";
			std::cout << "x    ";
			std::cin >> soundPosition->x;
			std::cout << "y    ";
			std::cin >> soundPosition->y;
			std::cout << "z    ";
			std::cin >> soundPosition->z;
		}
	}
	void  MyLoop::SetdBvolume() {
		std::cout << "Set volume in dB (preferred semi integer values) - default value = 3.0f    ";
		std::cin >> volumedB;
	}


}
