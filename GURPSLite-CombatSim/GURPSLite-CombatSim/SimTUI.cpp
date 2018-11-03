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
	keypad(stdscr, true);

	noecho();
	cbreak();


	std::vector<std::string> mainMenuOptions =
	{
		"Create Character",
		"Prepare the battle",
		"Show all Items",
		"Exit"
	};

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

		refresh();

		int choosenOption = getch();
		switch (choosenOption)
		{
		case KEY_UP:
			if (!(currentPos <= 5))
			{
				previousPos = currentPos;
				currentPos -= 2;
			}
			break;
		case KEY_DOWN:
			if (!(currentPos >= 11))
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

	endwin();
}

void MenuUIHelper::playerCreationMenu()
{
	clear();
	refresh();

	std::vector<std::string> playerCreationMenuOptions =
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

	int menuOptionsPos = 2;

	int previousPos = 0;
	int currentPos = 2;

	while (true)
	{
		for (auto& i : playerCreationMenuOptions)
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

		refresh();

		int choosenOption = getch();
		switch (choosenOption)
		{
		case KEY_UP:
			if (!(currentPos <= 2))
			{
				previousPos = currentPos;
				currentPos -= 2;
			}
			break;
		case KEY_DOWN:
			if (!(currentPos >= 13))
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
			case 4:
				showSkills();
				break;
			case 6:
				showArmours();
				break;
			case 8:
				showWeapons();
				break;
			case 10:
				showShields();
				break;
			case 12:
				clear();
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
}

void MenuUIHelper::showItemsMenu()
{
	clear();
	refresh();

	std::vector<std::string> showItemsMenuOptions =
	{
		"Show Characters",
		"Show Skills",
		"Show Armours",
		"Show Weapons",
		"Show Shields",
		"Go back"
	};

	int menuOptionsPos = 2;

	int previousPos = 0;
	int currentPos = 2;

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

		refresh();

		int choosenOption = getch();
		switch (choosenOption)
		{
		case KEY_UP:
			if (!(currentPos <= 2))
			{
				previousPos = currentPos;
				currentPos -= 2;
			}
			break;
		case KEY_DOWN:
			if (!(currentPos >= 13))
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
			case 4:
				showSkills();
				break;
			case 6:
				showArmours();
				break;
			case 8:
				showWeapons();
				break;
			case 10:
				showShields();
				break;
			case 12:
				clear();
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



	/*

	battleMenuOptions =
	{
		"Attack",
		"Surrender"
	};
	prepareTeamMenuOptions =
	{
		"Select Team 1",
		"Select Team 2",
		"Team Size: 1",
		"Go back"
	};*/
}
MenuUIHelper::~MenuUIHelper()
{}
