#include <curses.h>
#include "SimLogic.h"


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
		"Settings",
		"Exit"
	};


	int menuOptionsPos = 5;

	int previousPos = 0;
	int currentPos = 5;

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
		default:
			break;
		}


	}


	endwin();
    return 0;
}

