#pragma once

#include <curses.h>
#include <string>
#include <vector>
#include "SimLogic.h"

class MenuUIHelper
{
private:
	GameMaster gm;
	std::vector<std::string> mainMenuOptions;
	std::vector<std::string> playerCreationMenuOptions;
	std::vector<std::string> battleMenuOptions;
	std::vector<std::string> showItemsMenuOptions;
	std::vector<std::string> prepareTeamMenuOptions;

	const char* UILogo;
public:
	void init();

	void mainMenu();

	void playerCreationMenu();

	void battleMenu();

	void showItemsMenu();

	void prepareTeamMenu();
};