#pragma once

namespace MySmallRadioApp
{
	using namespace FmodStaticLib;

	class Scene
	{
	public:
		Scene(std::initializer_list<std::pair<unsigned int, std::string>>,bool);
		~Scene();
		std::string& GetScreen();
		bool GetInputMode();
		void PrintMainMenu();
		void TakeInput(unsigned int);
		void TakeInput(std::string&, bool);
		void UpdateMenuScene();

		bool sceneHasBeenUpdated = false;
		bool inputHasbeenUpdated = true;
	private:
		std::map<unsigned int, std::string> menuOptions;
		std::string screenMenu;

		void SetInputMode(bool, unsigned int selection=0);
		bool inputStringEnabled;
		unsigned int currentSelection = 0;

		MyWrapperLib* myWrapper = new MyWrapperLib();


	};
}
