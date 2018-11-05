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
	Character player;

	// Player stats to keep

	int playerST;
	int playerDX;
	int playerHT;

	std::string bigLogo;
	std::string smallLogo;

	int menuOptionXPos;
	WINDOW* logo;
	WINDOW* menu;


	void PlayerCreationMenu();
	void ShowPlayer();

	void BattleMenu();

	void ShowItemsMenu();
	void ShowCharacters();
	void ShowSkills();
	void ShowArmours();
	void ShowWeapons();
	void ShowShields();

	void PrepareTeamMenu();

public:
	bool isGameRunning;

	void MainMenu();
	void Init();

	MenuUIHelper();
	~MenuUIHelper();
};