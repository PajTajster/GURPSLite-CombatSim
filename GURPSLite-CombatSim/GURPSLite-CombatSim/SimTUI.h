#pragma once

#include <curses.h>
#include <string>
#include <vector>
#include "SimLogic.h"


class MenuUIHelper
{
private:
	GameMaster gm;
	// Related to BattleSize
	int teamSize;

	// Checks whether user launched battle mode [for easier returning from it].
	bool enteredBattleMode;

	bool isPlayerInit;
	Character* player;

	// Indexes of player's items.

	int playerCurrentWeapon;
	int playerCurrentArmour;
	int playerCurrentShield;

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

	void PrepareTeamMenu();
	void SelectFightersMenu();
	void BattleMenu();

	void ShowItemsMenu();
	void ShowCharacters();
	void ShowSkills();
	void ShowArmours();
	void ShowWeapons();
	void ShowShields();


public:
	bool isGameRunning;

	void MainMenu();
	void Init();

	MenuUIHelper();
	~MenuUIHelper();
};

class LogWriter
{
private:
	int currentLogPosition;
	int logHeight;

public:
	// To writing into log screen during battle.
	void WriteToLog(WINDOW* win, bool clearScreen, const char* text);

	LogWriter(int lH);
	~LogWriter();
};