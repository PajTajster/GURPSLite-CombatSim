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