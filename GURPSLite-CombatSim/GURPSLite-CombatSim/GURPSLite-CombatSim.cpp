#include "SimTUI.h"

// Soon (tm).


int main()
{	
	MenuUIHelper TUI;
	TUI.Init();

	while (TUI.isGameRunning)
	{
		TUI.MainMenu();
	}

    return 0;
}


