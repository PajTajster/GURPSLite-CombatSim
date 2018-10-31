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

	bool isPlayerInit;
	static Player player;

	static const char* UILogo;

	void mainMenu();

	void playerCreationMenu();
	void showPlayer();

	void battleMenu();

	void showItemsMenu();
	void showCharacters();
	void showSkills();
	void showArmours();
	void showWeapons();
	void showShields();

	void prepareTeamMenu();

public:
	void init();

	MenuUIHelper();
	~MenuUIHelper();
};