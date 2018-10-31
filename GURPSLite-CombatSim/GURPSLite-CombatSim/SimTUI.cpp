#include "SimTUI.h"

void MenuUIHelper::init()
{
	gm.InitializeGameMaster();

	mainMenu();
}

void MenuUIHelper::mainMenu()
{
	initscr();
	clear();

	WINDOW* mainMenu = newwin(0, 0, 0, 0);

	keypad(mainMenu, true);

	noecho();
	cbreak();

	int menuOptionsPos = 5;

	int previousPos = 0;
	int currentPos = 5;

	while (true)
	{
		if (isPlayerInit)
		{
			mainMenuOptions[1] = "Prepare the battle";
		}
		else
		{
			mainMenuOptions[1] = "Create character before preparing battle";
		}

		for (auto& i : mainMenuOptions)
		{
			if (menuOptionsPos == currentPos)
			{
				move(previousPos, 1);
				addch(' ');
				move(currentPos, 1);
				printw(">");
			}

			move(menuOptionsPos, 2);
			printw(i.c_str());

			menuOptionsPos += 2;
		}
		menuOptionsPos = 5;

		wrefresh(mainMenu);
		refresh();

		int choosenOption = wgetch(mainMenu);
		switch (choosenOption)
		{
		case KEY_UP:
			if (!(currentPos == 5))
			{
				previousPos = currentPos;
				currentPos -= 2;
			}
			break;
		case KEY_DOWN:
			if (!(currentPos == 11))
			{
				previousPos = currentPos;
				currentPos += 2;
			}
			break;
		case 10:
		{
			switch (currentPos)
			{
			case 5:
				//playerCreationMenu();
				break;
			case 7:
				if (isPlayerInit)
					//prepareTeamMenu();
				break;
			case 9:
				showItemsMenu();
				break;
			case 11:
				endwin();
				return;
			default:
				break;
			}
		}
		break;
		default:
			break;
		}


	}


	delwin(mainMenu);
}

void MenuUIHelper::showItemsMenu()
{
	WINDOW* itemShowWin = newwin(0, 0, 0, 0);

	int menuOptionsPos = 5;

	int previousPos = 0;
	int currentPos = 2;

	clear();
	keypad(itemShowWin, true);
	wrefresh(itemShowWin);
	refresh();

	while (true)
	{
		for (auto& i : showItemsMenuOptions)
		{
			if (menuOptionsPos == currentPos)
			{
				move(previousPos, 1);
				addch(' ');
				move(currentPos, 1);
				printw(">");
			}

			move(menuOptionsPos, 2);
			printw(i.c_str());

			menuOptionsPos += 2;
		}
		menuOptionsPos = 2;

		wrefresh(itemShowWin);
		refresh();

		int choosenOption = wgetch(itemShowWin);
		switch (choosenOption)
		{
		case KEY_UP:
			if (!(currentPos == 2))
			{
				previousPos = currentPos;
				currentPos -= 2;
			}
			break;
		case KEY_DOWN:
			if (!(currentPos == 13))
			{
				previousPos = currentPos;
				currentPos += 2;
			}
			break;
		case 10:
		{
			switch (currentPos)
			{
			case 2:
				showCharacters();
				break;
			case 5:
				showSkills();
				break;
			case 7:
				showArmours();
				break;
			case 9:
				showWeapons();
				break;
			case 11:
				showShields();
				break;
			case 13:
				wclear(itemShowWin);
				delwin(itemShowWin);
				refresh();
				return;
			default:
				break;
			}
		}
		break;
		default:
			break;
		}


	}

	delwin(itemShowWin);
}


void MenuUIHelper::showCharacters()
{
	// T
}
void MenuUIHelper::showSkills()
{
	// O
}
void MenuUIHelper::showArmours()
{
	// D
}
void MenuUIHelper::showWeapons()
{
	// O
}
void MenuUIHelper::showShields()
{
	// !
}




MenuUIHelper::MenuUIHelper()
{
	isPlayerInit = false;

	mainMenuOptions =
	{
		"Create Character",
		"Prepare the battle",
		"Show all Items",
		"Exit"
	};

	playerCreationMenuOptions =
	{
		"Name: ",
		"ST: 10",
		"DX: 10",
		"HT: 10",
		"Weapon: Bare Hands",
		"Armour: None",
		"Shield: None",
		" ",
		"Done: ",
		"Character Points left: ",

		"Go back"
	};

	battleMenuOptions =
	{
		"Attack",
		"Surrender"
	};

	showItemsMenuOptions =
	{
		"Show Characters",
		"Show Skills",
		"Show Armours",
		"Show Weapons",
		"Show Shields",
		"Go back"
	};
	prepareTeamMenuOptions =
	{
		"Select Team 1",
		"Select Team 2",
		"Team Size: 1",
		"Go back"
	};
}
MenuUIHelper::~MenuUIHelper()
{
	mainMenuOptions.clear();
	playerCreationMenuOptions.clear();
	battleMenuOptions.clear();
	showItemsMenuOptions.clear();
	prepareTeamMenuOptions.clear();
}
