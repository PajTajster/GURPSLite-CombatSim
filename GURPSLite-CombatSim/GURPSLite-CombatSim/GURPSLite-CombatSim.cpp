#include "SimTUI.h"

//		!!!		!!!		!!!		!!!		//
//	THIS MENU HERE IS SUBJECT TO CHANGE	//
//		!!!		!!!		!!!		!!!		//

// Soon (tm).


class MenuUIHelper
{
private:
	GameMaster gm;
public:
	void showItems()
	{
		WINDOW* itemShowWin = newwin(0, 0, 0, 0);

		int menuOptionsPos = 5;

		int previousPos = 0;
		int currentPos = 2;

		clear();
		keypad(itemShowWin, true);
		wrefresh(itemShowWin);

		std::vector<std::string> Options =
		{
			// 2
			"Show Characters",
			// 5
			"Show Skills",
			// 7
			"Show Armours",
			// 9
			"Show Weapons",
			// 11
			"Show Shields",
			// 13
			"Go back"
		};

		while (true)
		{
			for (auto& i : Options)
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
					delwin(itemShowWin);
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

	void showCharacters()
	{
		// T
	}
	void showSkills()
	{
		// O
	}
	void showArmours()
	{
		// D
	}
	void showWeapons()
	{
		// O
	}
	void showShields()
	{
		// !
	}

	MenuUIHelper()
	{
		gm.InitializeGameMaster();
	}
};

int main()
{	
	initscr();
	clear();
	keypad(curscr, true);
	noecho();
	cbreak();
	
	std::vector<std::string> Options =
	{
		"Prepare the battle",
		"Show me Items",
		"Exit"
	};

	int menuOptionsPos = 5;

	int previousPos = 0;
	int currentPos = 5;

	MenuUIHelper menu;

	while (true)
	{
		for (auto& i : Options)
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
		
		int choosenOption = wgetch(curscr);
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
			if (!(currentPos == 9))
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

				break;
			case 7:
				menu.showItems();
				break;
			case 9:
				endwin();
				return 0;
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
    return 0;
}


