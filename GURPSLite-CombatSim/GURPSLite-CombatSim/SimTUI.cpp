#include "SimTUI.h"

void MenuUIHelper::init()
{
	gm.InitializeGameMaster();

	mainMenu();
}

void MenuUIHelper::mainMenu()
{
	keypad(menu, true);

	wprintw(logo, bigLogo.c_str());
	wrefresh(logo);

	noecho();
	cbreak();

	std::vector<std::string> mainMenuOptions =
	{
		"Create Character",
		"Prepare the battle",
		"Show all Items",
		"Exit"
	};

	int currentOption = 0;

	int optionPos[] = { 2, 4, 6, 8 };

	int previousPos = 0;
	int currentPos = 2;

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

		int j = 0;
		for (auto& i : mainMenuOptions)
		{
			if (optionPos[j] == currentPos)
			{
				mvwaddch(menu, previousPos, menuOptionXPos - 1, ' ');
				mvwaddch(menu, currentPos, menuOptionXPos - 1, '>');
			}

			mvwprintw(menu, optionPos[j], menuOptionXPos, i.c_str());

			++j;
		}
		
		wrefresh(menu);

		int choosenOption = getch();
		switch (choosenOption)
		{
		case KEY_UP:
			if (currentOption != 0)
			{
				--currentOption;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			break;
		case KEY_DOWN:
			if (currentOption != 3)
			{
				++currentOption;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			break;

			// ENTER
		case 10:
		{
			switch (currentOption)
			{
			case 0:
				//playerCreationMenu();
				break;
			case 1:
				if (isPlayerInit)
				//prepareTeamMenu();
				break;
			case 2:
				showItemsMenu();
				break;
			case 3:
				delwin(menu);
				delwin(logo);
				endwin();
				isGameRunning = false;
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
	delwin(menu);
	delwin(logo);

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
	wclear(menu);

	std::vector<std::string> showItemsMenuOptions =
	{
		"Show Characters",
		"Show Skills",
		"Show Armours",
		"Show Weapons",
		"Show Shields",
		"Go back"
	};


	int currentOption = 0;

	int optionPos[] = { 2, 4, 6, 8, 10, 12 };

	int previousPos = 0;
	int currentPos = 2;

	while (true)
	{
		int j = 0;
		for (auto& i : showItemsMenuOptions)
		{
			if (optionPos[j] == currentPos)
			{
				mvwaddch(menu, previousPos, menuOptionXPos - 1, ' ');
				mvwaddch(menu, currentPos, menuOptionXPos - 1, '>');
			}

			mvwprintw(menu, optionPos[j], menuOptionXPos, i.c_str());

			++j;
		}


		wrefresh(menu);

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
				wclear(menu);
				return;
			default:
				break;
			}
			wclear(menu);
		}
		break;
		default:
			break;
		}


	}
}


void MenuUIHelper::showCharacters()
{
	wclear(menu);

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

	WINDOW* characterDesc = subwin(menu, 20, 8, 2, 20);

	// Menu Positions

	// Horizontal line for options
	int menuOptionsPos = 2;
	// Positions for all 3 options
	int optionPos[] = { 29, 39, 49 };
	// Current option indicated by an arrow
	int currentOption = 0;
	// Holds previous arrow position
	int previousPos = 0;
	// Current arrow position
	int currentPos = optionPos[0] - 1;

	while (true)
	{

		// Print Character
		//mvwprintw(menu, 5, menuOptionXPos, allCharacters[currentChar].PrintCharacter().c_str());
		wprintw(characterDesc, allCharacters[currentChar].PrintCharacter().c_str());
		box(menu, 0, 0);

		int j = 0;
		for (auto &i : showOptions)
		{
			if ((optionPos[j] - 1) == currentPos)
			{
				mvwaddch(menu, menuOptionsPos, previousPos, ' ');
				mvwaddch(menu, menuOptionsPos, currentPos, '>');
			}

			mvwprintw(menu, menuOptionsPos, optionPos[j], i.c_str());

			j++;
		}


		touchwin(menu);
		wrefresh(menu);
		
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
				wclear(characterDesc);
				delwin(characterDesc);
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
	initscr();
	clear();
	keypad(stdscr, true);

	refresh();

	logo = newwin(7, 30, 1, 30);
	menu = newwin(0, 0, 8, 0);

	wrefresh(logo);
	wrefresh(menu);

	curs_set(0);

	menuOptionXPos = 30;

	isPlayerInit = false;
	isGameRunning = true;

	bigLogo = R"(
 _____    _____    _____ 
/ ____|  / ____|  / ____|
| |  __  | |      | (___  
| | |_ | | |       \___ \ 
| |__| | | |____   ____) |
 \_____|  \_____| |_____/ )";

	smallLogo = R"(
  _____  _____   ____
 / ___/ / ___/  / __/
/ (_ / / /__   _\ \  
\___/  \___/  /___/  )";


}
MenuUIHelper::~MenuUIHelper()
{}
