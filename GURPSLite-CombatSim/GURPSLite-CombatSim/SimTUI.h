#pragma once

#include <curses.h>
#include <string>
#include <vector>
#include "SimLogic.h"

class MenuUIHelper
{
private:
	GameMaster gm;

	bool isPlayerInit;
	static Player player;

	std::string bigLogo;
	std::string smallLogo;

	int menuOptionXPos;
	WINDOW* logo;
	WINDOW* menu;


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
	bool isGameRunning;

	void mainMenu();
	void init();

	MenuUIHelper();
	~MenuUIHelper();
};