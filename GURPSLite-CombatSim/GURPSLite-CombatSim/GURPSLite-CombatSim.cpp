#include "SimTUI.h"

// Soon (tm).


int main()
{	
	MenuUIHelper TUI;
	TUI.init();

	while (TUI.isGameRunning)
	{
		TUI.mainMenu();
	}

    return 0;
}


