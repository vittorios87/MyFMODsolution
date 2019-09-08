#include "pch.h"
#include "Scene.h"

namespace MySmallRadioApp
{
	Scene::Scene(std::initializer_list<std::pair<unsigned int, std::string>> optionsList, bool hasInput)
	{
		for (std::pair<unsigned int, std::string> option : optionsList) 
		{
			menuOptions.emplace(option);
		}
		inputStringEnabled = hasInput;
	}


	Scene::~Scene()
	{
	}
	std::string& Scene::GetScreen()
	{
		return screenMenu;
	}
	bool Scene::GetInputMode()
	{
		return inputStringEnabled;
	}

	void Scene::PrintMainMenu() {
		
		for (auto it = menuOptions.begin(), itEnd = menuOptions.end(); it != itEnd; ++it)
		{
			
			screenMenu.append(std::to_string(it->first) + ". " + it->second + "\n\n");

		}
	}

	void Scene::TakeInput(unsigned int selection)
	{
		std::map<unsigned int, std::string>::iterator foundIt = menuOptions.find(selection);
		if (foundIt!=menuOptions.cend())
		{
			switch (foundIt->first)
			{
			case 1:
				SetInputMode(true, selection);
				break;
			case 2:
				SetInputMode(true, selection);
				break;
			case 0:
				exit(0);
			default:
				
				break;
			}
		}
	}
	void Scene::TakeInput(std::string& filePath, bool parseInput)
	{
		if( GetInputMode() && parseInput)
		{
			switch (currentSelection)
			{
			case 1:

				SetInputMode(false);
				break;

			case 2:
				SetInputMode(false);
				break;

			default:
				break;
			}
		}
	}

	void Scene::SetInputMode(bool enableStringInput, unsigned int selection) {
		inputStringEnabled=enableStringInput;
		currentSelection = selection;
		/*
		if(enableStringInput){currentSelection = selection;}
		else {currentSelection = 0;}
		*/
	}
	void Scene::UpdateMenuScene() {
		PrintMainMenu();
	}

}
