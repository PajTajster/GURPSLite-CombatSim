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
			clear();
			refresh();
		}
		break;
		default:
			break;
		}


	}
}


void MenuUIHelper::showCharacters()
{
	clear();
	refresh();

	std::vector<std::string> showOptions =
	{
		// 8
		"Previous",
		// 18
		"Go back",
		// 28
		"Next"
	};


	std::vector<Character> allCharacters = gm.getCharacters();

	// Currently showed character
	int currentChar = 0;
	int maxChar = allCharacters.size() - 1;


	// Menu Positions

	// Horizontal line for options
	int menuOptionsPos = 8;
	// Positions for all 3 options
	int optionPos[] = { 9, 19, 29 };
	// Current option indicated by an arrow
	int currentOption = 0;
	// Holds previous arrow position
	int previousPos = 0;
	// Current arrow position
	int currentPos = 8;

	while (true)
	{

		move(0, 0);

		// Print Character
		printw(allCharacters[currentChar].PrintCharacter().c_str());

		int j = 0;
		for (auto &i : showOptions)
		{
			if ((optionPos[j] - 1) == currentPos)
			{
				move(menuOptionsPos, previousPos);
				addch(' ');
				move(menuOptionsPos, currentPos);
				addch('>');
			}

			mvprintw(menuOptionsPos, optionPos[j], i.c_str());

			j++;
		}

		refresh();

		int choosenOption = getch();
		switch (choosenOption)
		{
		case KEY_LEFT:
			if (currentOption != 0)
			{
				--currentOption;
				previousPos = currentPos;
				currentPos = optionPos[currentOption] - 1;
			}
			break;
		case KEY_RIGHT:
			if (currentOption != 2)
			{
				++currentOption;
				previousPos = currentPos;
				currentPos = optionPos[currentOption] - 1;
			}
			break;
		case 10:
		{
			switch (currentOption)
			{
			// Previous
			case 0:
				if (currentChar != 0)
					--currentChar;
				break;

			// Go back
			case 1:
				return;

			// Next
			case 2:
				if (currentChar != maxChar)
					++currentChar;
				break;

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
void MenuUIHelper::showSkills()
{
	// T
}
void MenuUIHelper::showArmours()
{
	// O
}
void MenuUIHelper::showWeapons()
{
	// D
}
void MenuUIHelper::showShields()
{
	// O
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
