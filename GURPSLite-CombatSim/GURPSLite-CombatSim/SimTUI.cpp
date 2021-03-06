#include "SimTUI.h"
#include <ctime>


void MenuUIHelper::Init()
{
	srand((unsigned)time(NULL));

	resize_term(appLINES, appCOLS);

	// If succesful init
	if (gm.InitializeGameMaster())
	{
		player = gm.InitBasePlayer();

		MainMenu();
	}
	else
	{
		mvprintw(0, 0, "***FAILED TO LOAD DATA*** Check if those files are present with executable: ");
		mvprintw(1, 0, "\narmours.json\ncharacters.json\nnames.json\nshields.json\nskills.json\nweapons.json\nPress any key to continue");
		getch();
	}

}

void MenuUIHelper::MainMenu()
{
	keypad(menu, true);

	// "Logo" window.
	logo = newwin(7, 30, 1, 40);
	// "Menu" window.
	menu = newwin(0, 0, 8, 0);

	// Refresh both new windows.

	wrefresh(logo);
	wrefresh(menu);

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
	int currentPos = optionPos[0];

	while (isGameRunning)
	{
		wclear(menu);

		if (isPlayerInit)
		{
			mainMenuOptions[0] = "Edit Character";
			mainMenuOptions[1] = "Prepare the battle";
		}
		else
		{
			mainMenuOptions[0] = "Create Character";
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

		int chosenOption = getch();
		switch (chosenOption)
		{
		case KEY_UP:
			if (currentOption != 0)
			{
				--currentOption;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			else
			{
				currentOption = mainMenuOptions.size() - 1;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			break;
		case KEY_DOWN:
			if (currentOption != mainMenuOptions.size() - 1)
			{
				++currentOption;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			else
			{
				currentOption = 0;
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
				PlayerCreationMenu();
				break;
			case 1:
				if (isPlayerInit)
				{
					PrepareTeamMenu();
				}
				if (enteredBattleMode)
				{
					enteredBattleMode = false;
					return;
				}
				break;
			case 2:
				ShowItemsMenu();
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

void MenuUIHelper::PlayerCreationMenu()
{
	wclear(menu);
	
	std::vector<std::string> playerCreationMenuOptions =
	{
		"Name: ",
		"ST: ",
		"DX: ",
		"HT: ",
		"Veterancy: ",
		"Weapon: ",
		"Armour: ",
		"Shield: ",
		"Character Points left: ",
		"Done",
		"Go back"
	};

	std::vector<Weapon> weaponsToChoose = gm.GetWeapons();
	std::vector<Shield> shieldsToChoose = gm.GetShields();
	std::vector<Armour> armoursToChoose = gm.GetArmours();

	// Checks if player has "money" to buy more attributes.
	bool outOfPoints = false;

	// Checks if the user has chosen his character name.
	bool isPlayerNameSet = true;

	// Checks whether the weapon user chooses can go with a shield.
	bool weaponWithShield = true;

	if (weaponsToChoose[playerCurrentWeapon].isTwoHanded
		|| !(weaponsToChoose[playerCurrentWeapon].isMelee))
	{
		weaponWithShield = false;
	}

	if (player->name == "Nobody")
		isPlayerNameSet = false;

	int currentOption = 0;
	int optionPos[] = { 1, 2, 3, 4, 5, 7, 8, 9, 11, 13, 15 };

	int previousPos = 0;
	int currentPos = optionPos[0];

	while (true)
	{
		if (isPlayerNameSet)
		{
			playerCreationMenuOptions[0] = "Name: " + player->name;
		}
		else
		{
			playerCreationMenuOptions[0] = "Name: ";
		}
		playerCreationMenuOptions[1] = "ST: " + std::to_string(playerST);
		playerCreationMenuOptions[2] = "DX: " + std::to_string(playerDX);
		playerCreationMenuOptions[3] = "HT: " + std::to_string(playerHT);
		playerCreationMenuOptions[4] = "Veterancy: " + std::to_string(playerVet);
		playerCreationMenuOptions[5] = "Weapon: " + weaponsToChoose[playerCurrentWeapon].name;
		playerCreationMenuOptions[6] = "Armour: " + armoursToChoose[playerCurrentArmour].name;


		if (weaponWithShield)
		{
			playerCreationMenuOptions[7] = "Shield: " + shieldsToChoose[playerCurrentShield].name;
		}
		else
		{
			playerCreationMenuOptions[7] = "Shield: None [Ranged or Two-Handed Weapon Equipped]";
		}
		if (outOfPoints)
		{
			playerCreationMenuOptions[8] = "Character Points Left: " + std::to_string(player->characterPoints);
			playerCreationMenuOptions[8].append("(Stats at minimum(maximum)/Not enough points!)");
		}
		else
		{
			playerCreationMenuOptions[8] = "Character Points Left: " + std::to_string(player->characterPoints);
		}

		if (isPlayerNameSet)
		{
			playerCreationMenuOptions[9] = "Done";
		}

		int j = 0;
		for (auto& i : playerCreationMenuOptions)
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

		int chosenOption = getch();
		switch (chosenOption)
		{
		case KEY_UP:
			if (currentOption != 0)
			{
				--currentOption;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			else
			{
				currentOption = playerCreationMenuOptions.size() - 1;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			break;
		case KEY_DOWN:
			if (currentOption != playerCreationMenuOptions.size() - 1)
			{
				++currentOption;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			else
			{
				currentOption = 0;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			break;
		case KEY_LEFT:
		{
			switch (currentOption)
			{
				// ST
			case 1:
				if (player->ModifyAttribute(-1, 'S'))
				{
					--playerST;
					outOfPoints = false;
				}
				else
				{
					outOfPoints = true;
				}
				break;

				// DX
			case 2:
				if (player->ModifyAttribute(-1, 'D'))
				{
					--playerDX;
					outOfPoints = false;
				}
				else
				{
					outOfPoints = true;
				}
				break;

				// HT
			case 3:
				if (player->ModifyAttribute(-1, 'H'))
				{
					--playerHT;
					outOfPoints = false;
				}
				else
				{
					outOfPoints = true;
				}
				break;

				// Veterancy
			case 4:
				if (player->ModifyAttribute(-1, 'V'))
				{
					--playerVet;
					outOfPoints = false;
				}
				else
				{
					outOfPoints = true;
				}
				break;

				// Weapon
			case 5:
				if (playerCurrentWeapon == 0)
				{
					break;
				}
				else
				{
					--playerCurrentWeapon;

					if (weaponsToChoose[playerCurrentWeapon].isTwoHanded
						|| !(weaponsToChoose[playerCurrentWeapon].isMelee))
					{
						weaponWithShield = false;
					}
					else
					{
						weaponWithShield = true;
					}
				}
				break;

				// Armour
			case 6:
				if (playerCurrentArmour == 0)
				{
					break;
				}
				else
				{
					--playerCurrentArmour;
				}
				break;

				// Shield
			case 7:
				if (weaponWithShield)
				{

					if (playerCurrentShield == 0)
					{
						break;
					}
					else
					{
						--playerCurrentShield;
					}
				}
				break;
			}
		}
		break;

		case KEY_RIGHT:
		{
			switch (currentOption)
			{
				// ST
			case 1:
				if (player->ModifyAttribute(1, 'S'))
				{
					++playerST;
					outOfPoints = false;
				}
				else
				{
					outOfPoints = true;
				}
				break;

				// DX
			case 2:
				if (player->ModifyAttribute(1, 'D'))
				{
					++playerDX;
					outOfPoints = false;
				}
				else
				{
					outOfPoints = true;
				}
				break;

				// HT
			case 3:
				if (player->ModifyAttribute(1, 'H'))
				{
					++playerHT;
					outOfPoints = false;
				}
				else
				{
					outOfPoints = true;
				}
				break;

				// Veterancy
			case 4:
				if (player->ModifyAttribute(1, 'V'))
				{
					++playerVet;
					outOfPoints = false;
				}
				else
				{
					outOfPoints = true;
				}
				break;

				// Weapon
			case 5:
				if (playerCurrentWeapon == weaponsToChoose.size() - 1)
				{
					break;
				}
				else
				{
					++playerCurrentWeapon;

					if (weaponsToChoose[playerCurrentWeapon].isTwoHanded
						|| !(weaponsToChoose[playerCurrentWeapon].isMelee))
					{
						weaponWithShield = false;
					}
					else
					{
						weaponWithShield = true;
					}
				}
				break;

				// Armour
			case 6:
				if (playerCurrentArmour == armoursToChoose.size() - 1)
				{
					break;
				}
				else
				{
					++playerCurrentArmour;
				}
				break;

				// Shield
			case 7:
				if (weaponWithShield)
				{

					if (playerCurrentShield == shieldsToChoose.size() - 1)
					{
						break;
					}
					else
					{
						++playerCurrentShield;
					}
				}
				break;
			}
		}
		break;

		case 10:
		{
			switch (currentOption)
			{
				// Name
			case 0:
			{
				if (isPlayerNameSet)
				{
					wmove(menu, optionPos[0],
						menuOptionXPos + playerCreationMenuOptions[0].size() - player->name.length());
					
					std::string emptyLine(20, ' ');

					wprintw(menu, emptyLine.c_str());

					wmove(menu, optionPos[0],
						menuOptionXPos + playerCreationMenuOptions[0].size() - player->name.length());
				}
				else
				{
					wmove(menu, optionPos[0], menuOptionXPos + playerCreationMenuOptions[0].size());
				}
				curs_set(1);
				echo();

				char nameBuf[20];

				nameBuf[19] = '\0';

				wgetnstr(menu, nameBuf, 19);

				std::string newName = nameBuf;
				if (newName.empty())
				{
					noecho();
					curs_set(0);

					isPlayerNameSet = false;
					break;
				}

				player->name = newName;
				isPlayerNameSet = true;

				noecho();
				curs_set(0);
				break;
			}
				// Done
			case 9:
				// Update character.
				if (isPlayerNameSet)
				{
					player->currentWeapon = weaponsToChoose[playerCurrentWeapon];
					player->currentArmour = armoursToChoose[playerCurrentArmour];

					if (weaponWithShield)
					{
						player->currentShield = shieldsToChoose[playerCurrentShield];
					}
					else
					{
						player->currentShield = shieldsToChoose[0];
					}
					player->CalculateExtraAttributes();
					player->CalculateSkillsDefaults();

					isPlayerInit = true;

					wclear(menu);
					return;
				}
				else
				{
					isPlayerInit = false;
					playerCreationMenuOptions[9] = "[Name is not set!]";
				}
				break;

				// Go back
			case 10:
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

		wclear(menu);
	}
}

void MenuUIHelper::PrepareTeamMenu()
{
	wclear(menu);

	std::vector<std::string> battleSizeOptions = 
	{
		"Choose battle size: ",
		"1 vs 1",
		"2 vs 2",
		"Go back"
	};

	int currentOption = 1;

	int optionPos[] = { 2, 5, 7, 9, 12};

	int previousPos = 0;
	int currentPos = optionPos[1];

	// Choosing battle size first.
	while (isGameRunning)
	{
		int j = 0;
		for (auto& i : battleSizeOptions)
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

		int chosenOption = getch();
		switch (chosenOption)
		{

		case KEY_UP:
			if (currentOption != 1)
			{
				--currentOption;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			else
			{
				currentOption = 3;
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
			else
			{
				currentOption = 1;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			break;
		case 10:
		{
			switch (currentOption)
			{				
				// 1 vs 1
			case 1:
				teamSize = 1;
				SelectFightersMenu();

				if (enteredBattleMode)
				{
					return;
				}
				break;

				// 2 vs 2 
			case 2:
				teamSize = 2;
				SelectFightersMenu();

				if (enteredBattleMode)
				{
					return;
				}
				break;

				// Go back
			case 3:
				teamSize = 0;
				return;
			default:
				break;

			}
		}
			break;
		default:
			break;
		}
		wclear(menu);

	}

}

void MenuUIHelper::SelectFightersMenu()
{	
	std::vector<Character> allCharacters = gm.GetCharacters();

	// Currently shown character
	int currentChar = 0;
	int maxChar = allCharacters.size() - 1;


	//
	// FOR 1VS1
	//
	if (teamSize == 1)
	{
		std::vector<std::string> showOptions =
		{
			// 15
			"Previous",
			// 25
			"Go back",
			// 35
			"Select",
			// 45
			"Next"
		};

		// Menu Positions

		// Horizontal line for options
		int menuOptionsPos = 2;
		// Positions for all options
		int optionPos[] = { 36, 46, 56, 66 };
		// Current option indicated by an arrow
		int currentOption = 0;
		// Holds previous arrow position
		int previousPos = 0;
		// Current arrow position
		int currentPos = optionPos[0] - 1;

		// Whether player has chosen his opponent.
		bool isEnemySelected = false;
		// What character has been chosen.
		int selectedChar = 0;

		while (true)
		{
			wclear(menu);

			if (isEnemySelected)
			{
				mvwprintw(menu, menuOptionsPos - 1, menuOptionXPos - 6, "Duel ready[Press 'y' to proceed]");
				mvwprintw(menu, menuOptionsPos + 2, menuOptionXPos, ("Your enemy: " + allCharacters[selectedChar].name).c_str());
			}
			else
			{
				mvwprintw(menu, menuOptionsPos - 1, menuOptionXPos, "Choose Team 2 Fighter");
			}

			std::vector<std::string> charPrinted = allCharacters[currentChar].PrintCharacter();

			int i = menuOptionsPos + 4;
			// Print Character
			for (auto& it : charPrinted)
			{
				mvwprintw(menu, i++, menuOptionXPos, it.c_str());
			}

			int j = 0;
			for (auto &it : showOptions)
			{
				if ((optionPos[j] - 1) == currentPos)
				{
					mvwaddch(menu, menuOptionsPos, previousPos, ' ');
					mvwaddch(menu, menuOptionsPos, currentPos, '>');
				}

				mvwprintw(menu, menuOptionsPos, optionPos[j], it.c_str());

				j++;
			}


			wrefresh(menu);

			int chosenOption = getch();
			switch (chosenOption)
			{
			case KEY_LEFT:
				if (currentOption != 0)
				{
					--currentOption;
					previousPos = currentPos;
					currentPos = optionPos[currentOption] - 1;
				}
				else
				{
					currentOption = showOptions.size() - 1;
					previousPos = currentPos;
					currentPos = optionPos[currentOption] - 1;
				}
				break;
			case KEY_RIGHT:
				if (currentOption != showOptions.size() - 1)
				{
					++currentOption;
					previousPos = currentPos;
					currentPos = optionPos[currentOption] - 1;
				}
				else
				{
					currentOption = 0;
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

					// Select
				case 2:
					isEnemySelected = true;
					selectedChar = currentChar;
					break;

					// Next
				case 3:
					if (currentChar != maxChar)
						++currentChar;
					break;

				default:
					break;
				}
			}
			break;
			case 'y':
				if (isEnemySelected)
				{
					gm.AddCharacterToMainVector(*player, 1);
					gm.AddCharacterToMainVector(allCharacters[selectedChar], 2);
					BattleMenu();
					return;
				}
				break;
			default:
				break;
			}
		}
	}
	// 
	//	FOR 2VS2 OR 3VS3
	// 
	else
	{
		std::vector<std::string> showOptions =
		{
			// 13
			"Previous",
			// 23
			"Go back",
			// 33
			"Select",
			// 43
			"Reset",
			// 53
			"Change Team",
			// 68
			"Next"
		};

		// Whether user wanted to reset team composition.
		bool displayTeamReset = false;
		// Whether user tried to reset empty teams.
		bool displayTeamEmpty = false;
		// Whether user tried to add character to full team.
		bool displayTeamFull = false;
		// Show extra info if all teams are full.
		bool isAllDone = false;

		// Menu Positions

		// Horizontal line for options
		int menuOptionsPos = 2;
		// Positions for all options
		int optionPos[] = { 23, 33, 43, 53, 63, 78 };
		// Current option indicated by an arrow
		int currentOption = 0;
		// Holds previous arrow position
		int previousPos = 0;
		// Current arrow position
		int currentPos = optionPos[0] - 1;

		// Keep record for Team 1 Characters [Player is team1]
		int team1Members = 1;
		// Keep record for Team 2 Characters
		int team2Members = 0;

		// 1 for Team 1, 2 for Team 2
		int currentTeamBeingChosen = 1;

		std::vector<Character> team1Chars;
		std::vector<Character> team2Chars;
		
		// For 2vs2/3vs3
		while (true)
		{
			if ((team1Members == teamSize) && (team2Members == teamSize))
				isAllDone = true;

			wclear(menu);

			// Displaying what team is being currently created.
			switch (currentTeamBeingChosen)
			{
				// Team 1
			case 1:
				if (isAllDone)
				{
					mvwprintw(menu, menuOptionsPos - 1, menuOptionXPos - 15, "All Teams are ready [Press 'y' to accept changes]");
				}
				else
				{
					mvwprintw(menu, menuOptionsPos - 1, menuOptionXPos, "Choose Team 1 Fighter");
				}
				mvwprintw(menu, menuOptionsPos + 1, menuOptionXPos,
					("Team members: " + std::to_string(team1Members)).c_str());
				break;
				// Team 2
			case 2:
				if (isAllDone)
				{
					mvwprintw(menu, menuOptionsPos - 1, menuOptionXPos - 15, "All Teams are ready [Press 'y' to accept changes]");
				}
				else
				{
					mvwprintw(menu, menuOptionsPos - 1, menuOptionXPos, "Choose Team 2 Fighter");
				}
				mvwprintw(menu, menuOptionsPos + 1, menuOptionXPos,
					("Team members: " + std::to_string(team2Members)).c_str());
				break;
				// At this point the app can as well end. [Shouldn't happen]
			default:
				return;
			}

			// Displaying info about reset, team being full/empty.

			if (displayTeamReset)
			{
				mvwprintw(menu, menuOptionsPos + 2, menuOptionXPos, "Team has been reset!");
				displayTeamReset = false;
			}
			if (displayTeamFull)
			{
				mvwprintw(menu, menuOptionsPos + 2, menuOptionXPos, "Team is full!");
				displayTeamFull = false;
			}
			if (displayTeamEmpty)
			{
				mvwprintw(menu, menuOptionsPos + 2, menuOptionXPos, "Nothing to reset!");
				displayTeamEmpty = false;
			}


			std::vector<std::string> charPrinted = allCharacters[currentChar].PrintCharacter();

			int i = menuOptionsPos + 4;
			// Print Character
			for (auto& it : charPrinted)
			{
				mvwprintw(menu, i++, menuOptionXPos, it.c_str());
			}

			int j = 0;
			for (auto &it : showOptions)
			{
				if ((optionPos[j] - 1) == currentPos)
				{
					mvwaddch(menu, menuOptionsPos, previousPos, ' ');
					mvwaddch(menu, menuOptionsPos, currentPos, '>');
				}

				mvwprintw(menu, menuOptionsPos, optionPos[j], it.c_str());

				j++;
			}


			wrefresh(menu);

			int chosenOption = getch();
			switch (chosenOption)
			{
			case KEY_LEFT:
				if (currentOption != 0)
				{
					--currentOption;
					previousPos = currentPos;
					currentPos = optionPos[currentOption] - 1;
				}
				else
				{
					currentOption = showOptions.size() - 1;
					previousPos = currentPos;
					currentPos = optionPos[currentOption] - 1;
				}
				break;
			case KEY_RIGHT:
				if (currentOption != showOptions.size() - 1)
				{
					++currentOption;
					previousPos = currentPos;
					currentPos = optionPos[currentOption] - 1;
				}
				else
				{
					currentOption = 0;
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

					// Select
				case 2:
				{
					switch (currentTeamBeingChosen)
					{
					case 1:
						if (team1Members == teamSize)
						{
							displayTeamFull = true;
							break;
						}
						++team1Members;
						// Push a copy of character.
						team1Chars.push_back(Character(allCharacters[currentChar], true));
						break;
					case 2:
						if (team2Members == teamSize)
						{
							displayTeamFull = true;
							break;
						}
						++team2Members;
						// Push a copy of character.
						team2Chars.push_back(Character(allCharacters[currentChar], true));
						break;
					default:
						return;
					}
				}
					break;

					// Reset
				case 3:
				{
					switch (currentTeamBeingChosen)
					{
					case 1:
						if (team1Members == 1)
						{
							displayTeamEmpty = true;
							break;
						}
						else
						{
							team1Members = 1;
							team1Chars.clear();
							displayTeamReset = true;
							isAllDone = false;
						}
						break;

					case 2:
						if (team2Members == 0)
						{
							displayTeamEmpty = true;
							break;
						}
						else
						{
							team2Members = 0;
							team2Chars.clear();
							displayTeamReset = true;
							isAllDone = false;
						}
						break;

					default:
						break;
					}
				}
					break;

					// Change Team
				case 4:
					if (currentTeamBeingChosen == 1)
					{
						currentTeamBeingChosen = 2;
					}
					else
					{
						currentTeamBeingChosen = 1;
					}
					break;

					// Next
				case 5:
					if (currentChar != maxChar)
						++currentChar;
					break;

				default:
					break;
				}
			}
			break;
			case 'y':
				if (isAllDone)
				{
					team1Chars.push_back(*player);
					gm.PrepareTeams(team1Chars, team2Chars);
					BattleMenu();
					return;
				}
				break;
			default:
				break;
			}
		}
	}
}
void MenuUIHelper::BattleMenu()
{
	// Get rid of old menu
	wclear(menu);
	wclear(logo);
	delwin(menu);
	delwin(logo);

	refresh();


	// And create something better

	int defaultMenuWidth = (appCOLS / 2) + (appCOLS / 4);
	int defaultMenuHeight = (appLINES / 4) + (appLINES / 4);


	WINDOW* battleWindow = newwin(defaultMenuHeight, defaultMenuWidth, 0, 0);
	WINDOW* actionsWindow = newwin(defaultMenuHeight, 0, 0, appCOLS - (appCOLS - defaultMenuWidth));
	WINDOW* logWindow = newwin(0, defaultMenuWidth, appLINES - (appLINES - defaultMenuHeight), 0);
	WINDOW* smallLogoWindow = newwin(0, 0,
		(appLINES + 4) - (appLINES - defaultMenuHeight),
		(appCOLS + 4) - (appCOLS - defaultMenuWidth));
	
	box(battleWindow, 0, 0);
	box(actionsWindow, 0, 0);
	mvwprintw(battleWindow, 0, 0, "Battle Screen");
	mvwprintw(actionsWindow, 0, 0, "Actions");
	mvwprintw(logWindow, 0, 0, "Logs");
	wprintw(smallLogoWindow, smallLogo.c_str());


	wrefresh(battleWindow);
	wrefresh(actionsWindow);
	wrefresh(logWindow);
	wrefresh(smallLogoWindow);

	std::vector<std::string> actionsOptions =
	{
		"Attack",
		"Skip Turn",
		"Surrender"
	};

	// Action menu options positions.
	int actionsYPos[] = { 2, 3, 5 };

	// Positions y for both teams.
	constexpr int teamsYPos[] = { 3, 5, 7 };
	// Positions on X for both teams.
	constexpr int teamsXPos[] = { 5, 50 };

	
	int currentPos = actionsYPos[0];
	int previousPos = 2;

	int currentOption = 0;

	// For writing into log.
	int logHeight = (LINES - 1) - (LINES - defaultMenuHeight);

	LogWriter logWriter(logHeight);

	// What character has it's move now.
	int currentCharacterTurn = 0;
	// Keeps record of turns played.
	int currentTurn = 0;
	// Keeps record of fallen characters.
	int howManyDied = 0;

	bool isBattleFinished = false;
	bool allEnemiesDied = false;
	// Keeps track who died.
	int enemiesDead[] = { -1, -1, -1 };
	// So dead counter is increment once for each dead character.
	bool whoIsDead[] = { false, false, false, false };

	// Let GM sort it's data.
	gm.CalculateInitiative();

	std::vector<Character> charactersInPlay = gm.GetCharactersInPlay();


	// Print Action menu
	{
		int i = 0;
		for (auto& it : actionsOptions)
		{
			mvwprintw(actionsWindow, actionsYPos[i], 2, it.c_str());
			++i;
		}
	}
	wrefresh(actionsWindow);

	// To keep indexes of enemies.
	int team2Characters[3] = { -1, -1, -1 };

	// Print BattleScreen
	{
		int t1 = 0;
		int t2 = 0;

		int index = 0;
		for (auto& it : charactersInPlay)
		{
			switch (it.GetTeam())
			{
			case 1:
				switch (teamSize)
				{
				case 1:
					mvwprintw(battleWindow, teamsYPos[1], teamsXPos[0], it.name.c_str());
					break;
				case 2:
					mvwprintw(battleWindow, teamsYPos[t1], teamsXPos[0], it.name.c_str());
					t1 = 2;
					break;
				case 3:
					mvwprintw(battleWindow, teamsYPos[t1], teamsXPos[0], it.name.c_str());
					++t1;
					break;
				default:
					break;
				}
				break;
			case 2:
				switch (teamSize)
				{
				case 1:
					mvwprintw(battleWindow, teamsYPos[1], teamsXPos[1], it.name.c_str());
					team2Characters[0] = index;
					break;
				case 2:
					mvwprintw(battleWindow, teamsYPos[t2], teamsXPos[1], it.name.c_str());
					if (t2 == 0)
					{
						team2Characters[t2] = index;
						enemiesDead[t2] = 0;
					}
					else
					{
						team2Characters[t2 - 1] = index;
						enemiesDead[t2 - 1] = 0;
					}
					t2 = 2;
					break;
				case 3:
					mvwprintw(battleWindow, teamsYPos[t2], teamsXPos[1], it.name.c_str());
					team2Characters[t2] = index;
					enemiesDead[t2] = 0;
					++t2;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}

			++index;
		}
	}
	wrefresh(battleWindow);

	// Result of attacking.
	std::string messageToLog = "";

	logWriter.WriteToLog(logWindow, false, "Battle Started! Press 'Enter' to move game forward.");
	getch();

	while (true)
	{
		if (isBattleFinished)
		{
			bool newLine = true;
			if (allEnemiesDied)
			{
				logWriter.WriteToLog(logWindow, true, "All your foes have fallen dead!");
				newLine = false;
			}
			logWriter.WriteToLog(logWindow, newLine, "Battle is finished, press ENTER to finish and go back to menu.");
			logWriter.WriteToLog(logWindow, false, (std::to_string(howManyDied) + " brave fighters died today.").c_str());
			getch();
			break;
		}
		else
		{
			currentCharacterTurn = 0;
			logWriter.WriteToLog(logWindow, false, ("Turn " + std::to_string(currentTurn + 1)).c_str());
			for (auto& it : charactersInPlay)
			{
				// Check if dead.
				if (it.isDead)
				{
					// Check if character is player's enemy.
					if (it.GetTeam() == 2)
					{
						int team2Died = 0;
						
						int i = 0;
						// See how many enemies died
						for (; i != 2; ++i)
						{
							if (enemiesDead[i] == 1)
								++team2Died;
						}
						enemiesDead[i - 1] = 1;
						++team2Died;

						// If all enemies died, battle is over.
						if (team2Died == teamSize)
							allEnemiesDied = true;

					}

					if (!whoIsDead[currentCharacterTurn])
					{
						// Increment dead characters counter.
						++howManyDied;
						whoIsDead[currentCharacterTurn] = true;
					}

					if (allEnemiesDied)
					{
						isBattleFinished = true;
						logWriter.WriteToLog(logWindow, true, "All your enemies perished! Press 'enter' to proceed.");
					}
					if (isBattleFinished)
						break;

					continue;
				}

				if (it.isKnockedDown)
				{
					logWriter.WriteToLog(logWindow, false,
						(it.name + " still knocked down for: " + std::to_string(it.knockDownTimer) + " Turns.").c_str());

					continue;
				}

				if (allEnemiesDied)
				{
					isBattleFinished = true;
					logWriter.WriteToLog(logWindow, true, "All your enemies perished! Press 'enter' to proceed.");
				}

				if (isBattleFinished)
					break;

				// Player actions
				if (it.isPlayer)
				{
					logWriter.WriteToLog(logWindow, false, "Your Turn.");

					bool playerFinished = false;
					while (!playerFinished)
					{
						{
							int j = 0;
							for (auto& i : actionsOptions)
							{
								if (actionsYPos[j] == currentPos)
								{
									mvwaddch(actionsWindow, previousPos, 1, ' ');
									mvwaddch(actionsWindow, currentPos, 1, '>');
								}

								mvwprintw(actionsWindow, actionsYPos[j], 2, i.c_str());

								++j;
							}
						}

						wrefresh(actionsWindow);

						int chosenOption = getch();
						switch (chosenOption)
						{
						case KEY_UP:
							if (currentOption != 0)
							{
								--currentOption;
								previousPos = currentPos;
								currentPos = actionsYPos[currentOption];
							}
							else
							{
								currentOption = 2;
								previousPos = currentPos;
								currentPos = actionsYPos[currentOption];
							}
							break;
						case KEY_DOWN:
							if (currentOption != 2)
							{
								++currentOption;
								previousPos = currentPos;
								currentPos = actionsYPos[currentOption];
							}
							else
							{
								currentOption = 0;
								previousPos = currentPos;
								currentPos = actionsYPos[currentOption];
							}
							break;
						case 10:
						{
							switch (currentPos)
							{
								// Attack
							case 2:
							{
								int currentPos = 1;
								int previousPos = 1;
								while (!playerFinished)
								{
									// Player choice.
									int option = 0;

									switch (teamSize)
									{
									case 1:
										mvwaddch(battleWindow, teamsYPos[1], teamsXPos[1] - 1, '>');
										wrefresh(battleWindow);
										option = getch();
										mvwaddch(battleWindow, teamsYPos[1], teamsXPos[1] - 1, ' ');
										wrefresh(battleWindow);
										// ENTER
										if (option == 10)
										{
											if (charactersInPlay[team2Characters[0]].isDead)
												logWriter.WriteToLog(logWindow, false, "You're trying to attack dead man");
											else
											{
												messageToLog = it.Attack(charactersInPlay[team2Characters[0]]);
												logWriter.WriteToLog(logWindow, false, messageToLog.c_str());
												playerFinished = true;
											}
										}
										break;
									case 2:
									{
										int selectedCharacter = team2Characters[0];
										// Current cursor position
										int currentTarget = teamsYPos[0];
										// Previous cursor position
										int previousTarget = 1;
										while (!playerFinished)
										{
											for (int i = 0; i < 3; ++i)
											{
												if (teamsYPos[i] == currentTarget)
												{
													mvwaddch(battleWindow, previousTarget, teamsXPos[1] - 1, ' ');
													mvwaddch(battleWindow, teamsYPos[i], teamsXPos[1] - 1, '>');
												}
											}

											wrefresh(battleWindow);

											int selectedTarget = getch();
											switch (selectedTarget)
											{
											case KEY_UP:
											case KEY_DOWN:
												if (currentTarget == teamsYPos[2])
												{
													previousTarget = currentTarget;
													currentTarget = teamsYPos[0];
													selectedCharacter = team2Characters[0];
												}
												else
												{
													previousTarget = currentTarget;
													currentTarget = teamsYPos[2];
													selectedCharacter = team2Characters[1];
												}
												break;
											case 10:
											{
												if (charactersInPlay[selectedCharacter].isDead)
													logWriter.WriteToLog(logWindow, false, "You're trying to attack dead man");
												else
												{
													messageToLog = it.Attack(charactersInPlay[selectedCharacter]);
													logWriter.WriteToLog(logWindow, false, messageToLog.c_str());
													playerFinished = true;
												}
											}
												break;
											default:
												break;
											}
										}

										mvwaddch(battleWindow, currentTarget, teamsXPos[1] - 1, ' ');
										wrefresh(battleWindow);

									}
										break;
									case 3:
										break;
									default:
										break;
									}
								}
							}
							break;
							
							// Skip Turn
							case 3:
								logWriter.WriteToLog(logWindow, false, "You've decided to do nothing. Press 'enter' to continue.");
								playerFinished = true;
								break;

								// Surrender
							case 5:
								logWriter.WriteToLog(logWindow, true, "You've surrendered. You lost, press 'enter' to continue.");
								gm.ClearBattleData();
								getch();
								delete player;
								player = gm.InitBasePlayer();
								playerST = 10;
								playerDX = 10;
								playerHT = 10;
								isPlayerInit = false;
								playerFinished = true;
								enteredBattleMode = true;
								isBattleFinished = true;
								wclear(battleWindow);
								wclear(actionsWindow);
								wclear(logWindow);
								wclear(smallLogoWindow);
								wrefresh(battleWindow);
								wrefresh(actionsWindow);
								wrefresh(logWindow);
								wrefresh(smallLogoWindow);
								delwin(battleWindow);
								delwin(actionsWindow);
								delwin(logWindow);
								delwin(smallLogoWindow);
								return;

							default:
								break;
							}
						}
						break;
						default:
							break;
						}
						wrefresh(actionsWindow);
					}

					mvwaddch(actionsWindow, currentPos, 1, ' ');
					wrefresh(actionsWindow);				
					
				}
				// NPC actions
				else
				{
					messageToLog = it.NPCAssessSituation(charactersInPlay);
					if (messageToLog == "")
						continue;
					logWriter.WriteToLog(logWindow, false, messageToLog.c_str());

					gm.UpdatePlayer(player, charactersInPlay);
					
					if (player->isDead)
					{
						logWriter.WriteToLog(logWindow, false, "...YOU DIED...\nPress 'enter' to proceed.");
						isBattleFinished = true;
						++howManyDied;
						getch();
						break;
					}

					messageToLog.clear();
				}

				getch();

				++currentCharacterTurn;
			}
			if (!isBattleFinished)
			{
				gm.NextTurn(charactersInPlay);
				gm.UpdatePlayer(player, charactersInPlay);
			}
			++currentTurn;
		}
	}

	// I'm not proud of this here \/

	gm.ClearBattleData();
	delete player;
	player = gm.InitBasePlayer();
	playerST = 10;
	playerDX = 10;
	playerHT = 10;
	isPlayerInit = false;
	enteredBattleMode = true;
	isBattleFinished = true;
	wclear(battleWindow);
	wclear(actionsWindow);
	wclear(logWindow);
	wclear(smallLogoWindow);
	wrefresh(battleWindow);
	wrefresh(actionsWindow);
	wrefresh(logWindow);
	wrefresh(smallLogoWindow);
	delwin(battleWindow);
	delwin(actionsWindow);
	delwin(logWindow);
	delwin(smallLogoWindow);
}

void MenuUIHelper::ShowItemsMenu()
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
	int currentPos = optionPos[0];

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

		int chosenOption = getch();
		switch (chosenOption)
		{

		case KEY_UP:
			if (currentOption != 0)
			{
				--currentOption;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			else
			{
				currentOption = 5;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			break;
		case KEY_DOWN:
			if (currentOption != 5)
			{
				++currentOption;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			else
			{
				currentOption = 0;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			break;
		case 10:
		{
			switch (currentPos)
			{
			case 2:
				ShowCharacters();
				break;
			case 4:
				ShowSkills();
				break;
			case 6:
				ShowArmours();
				break;
			case 8:
				ShowWeapons();
				break;
			case 10:
				ShowShields();
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


void MenuUIHelper::ShowCharacters()
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


	std::vector<Character> allCharacters = gm.GetCharacters();

	// Currently shown character
	int currentChar = 0;
	int maxChar = allCharacters.size() - 1;
	
	// Menu Positions

	// Horizontal line for options
	int menuOptionsPos = 2;
	// Positions for all 3 options
	int optionPos[] = { 39, 49, 59 };
	// Current option indicated by an arrow
	int currentOption = 0;
	// Holds previous arrow position
	int previousPos = 0;
	// Current arrow position
	int currentPos = optionPos[0] - 1;

	while (true)
	{
		wclear(menu);

		std::vector<std::string> charPrinted = allCharacters[currentChar].PrintCharacter();

		int i = menuOptionsPos + 4;
		// Print Character
		for (auto& it : charPrinted)
		{
			mvwprintw(menu, i++, menuOptionXPos, it.c_str());
		}

		int j = 0;
		for (auto &it : showOptions)
		{
			if ((optionPos[j] - 1) == currentPos)
			{
				mvwaddch(menu, menuOptionsPos, previousPos, ' ');
				mvwaddch(menu, menuOptionsPos, currentPos, '>');
			}

			mvwprintw(menu, menuOptionsPos, optionPos[j], it.c_str());

			j++;
		}


		wrefresh(menu);
		
		int chosenOption = getch();
		switch (chosenOption)
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
void MenuUIHelper::ShowSkills()
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


	std::vector<Skill> allSkills = gm.GetSkills();

	// Currently shown skill
	int currentSkill = 0;
	int maxSkill = allSkills.size() - 1;

	// Menu Positions

	// Horizontal line for options
	int menuOptionsPos = 2;
	// Positions for all 3 options
	int optionPos[] = { 39, 49, 59 };
	// Current option indicated by an arrow
	int currentOption = 0;
	// Holds previous arrow position
	int previousPos = 0;
	// Current arrow position
	int currentPos = optionPos[0] - 1;

	while (true)
	{
		wclear(menu);


		std::vector<std::string> skillPrinted = allSkills[currentSkill].PrintSkill();

		int i = menuOptionsPos + 4;
		// Print Character
		for (auto& it : skillPrinted)
		{
			mvwprintw(menu, i++, menuOptionXPos, it.c_str());
		}

		int j = 0;
		for (auto &it : showOptions)
		{
			if ((optionPos[j] - 1) == currentPos)
			{
				mvwaddch(menu, menuOptionsPos, previousPos, ' ');
				mvwaddch(menu, menuOptionsPos, currentPos, '>');
			}

			mvwprintw(menu, menuOptionsPos, optionPos[j], it.c_str());

			j++;
		}


		wrefresh(menu);

		int chosenOption = getch();
		switch (chosenOption)
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
				if (currentSkill != 0)
					--currentSkill;
				break;

				// Go back
			case 1:
				return;

				// Next
			case 2:
				if (currentSkill != maxSkill)
					++currentSkill;
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
void MenuUIHelper::ShowArmours()
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


	std::vector<Armour> allArmours = gm.GetArmours();

	// Currently shown armour
	int currentArmour = 0;
	int maxArmour = allArmours.size() - 1;

	// Menu Positions

	// Horizontal line for options
	int menuOptionsPos = 2;
	// Positions for all 3 options
	int optionPos[] = { 39, 49, 59 };
	// Current option indicated by an arrow
	int currentOption = 0;
	// Holds previous arrow position
	int previousPos = 0;
	// Current arrow position
	int currentPos = optionPos[0] - 1;

	while (true)
	{
		wclear(menu);


		std::vector<std::string> armourPrinted = allArmours[currentArmour].PrintArmour();

		int i = menuOptionsPos + 4;
		// Print Character
		for (auto& it : armourPrinted)
		{
			mvwprintw(menu, i++, menuOptionXPos, it.c_str());
		}

		int j = 0;
		for (auto &it : showOptions)
		{
			if ((optionPos[j] - 1) == currentPos)
			{
				mvwaddch(menu, menuOptionsPos, previousPos, ' ');
				mvwaddch(menu, menuOptionsPos, currentPos, '>');
			}

			mvwprintw(menu, menuOptionsPos, optionPos[j], it.c_str());

			j++;
		}


		wrefresh(menu);

		int chosenOption = getch();
		switch (chosenOption)
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
				if (currentArmour != 0)
					--currentArmour;
				break;

				// Go back
			case 1:
				return;

				// Next
			case 2:
				if (currentArmour != maxArmour)
					++currentArmour;
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
void MenuUIHelper::ShowWeapons()
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


	std::vector<Weapon> allWeapons = gm.GetWeapons();

	// Currently shown weapon
	int currentWeapon = 0;
	int maxWeapon = allWeapons.size() - 1;

	// Menu Positions

	// Horizontal line for options
	int menuOptionsPos = 2;
	// Positions for all 3 options
	int optionPos[] = { 39, 49, 59 };
	// Current option indicated by an arrow
	int currentOption = 0;
	// Holds previous arrow position
	int previousPos = 0;
	// Current arrow position
	int currentPos = optionPos[0] - 1;

	while (true)
	{
		wclear(menu);


		std::vector<std::string> weaponPrinted = allWeapons[currentWeapon].PrintWeapon();

		int i = menuOptionsPos + 4;
		// Print Character
		for (auto& it : weaponPrinted)
		{
			mvwprintw(menu, i++, menuOptionXPos, it.c_str());
		}

		int j = 0;
		for (auto &it : showOptions)
		{
			if ((optionPos[j] - 1) == currentPos)
			{
				mvwaddch(menu, menuOptionsPos, previousPos, ' ');
				mvwaddch(menu, menuOptionsPos, currentPos, '>');
			}

			mvwprintw(menu, menuOptionsPos, optionPos[j], it.c_str());

			j++;
		}


		wrefresh(menu);

		int chosenOption = getch();
		switch (chosenOption)
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
				if (currentWeapon != 0)
					--currentWeapon;
				break;

				// Go back
			case 1:
				return;

				// Next
			case 2:
				if (currentWeapon != maxWeapon)
					++currentWeapon;
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
void MenuUIHelper::ShowShields()
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


	std::vector<Shield> allShields = gm.GetShields();

	// Currently shown shield
	int currentShield = 0;
	int maxShield = allShields.size() - 1;

	// Menu Positions

	// Horizontal line for options
	int menuOptionsPos = 2;
	// Positions for all 3 options
	int optionPos[] = { 39, 49, 59 };
	// Current option indicated by an arrow
	int currentOption = 0;
	// Holds previous arrow position
	int previousPos = 0;
	// Current arrow position
	int currentPos = optionPos[0] - 1;

	while (true)
	{
		wclear(menu);

		std::vector<std::string> shieldPrinted = allShields[currentShield].PrintShield();

		int i = menuOptionsPos + 4;
		// Print Character
		for (auto& it : shieldPrinted)
		{
			mvwprintw(menu, i++, menuOptionXPos, it.c_str());
		}

		int j = 0;
		for (auto &it : showOptions)
		{
			if ((optionPos[j] - 1) == currentPos)
			{
				mvwaddch(menu, menuOptionsPos, previousPos, ' ');
				mvwaddch(menu, menuOptionsPos, currentPos, '>');
			}

			mvwprintw(menu, menuOptionsPos, optionPos[j], it.c_str());

			j++;
		}


		wrefresh(menu);

		int chosenOption = getch();
		switch (chosenOption)
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
				if (currentShield != 0)
					--currentShield;
				break;

				// Go back
			case 1:
				return;

				// Next
			case 2:
				if (currentShield != maxShield)
					++currentShield;
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




MenuUIHelper::MenuUIHelper()
{
	// Init PDCurses.
	initscr();
	// Start colors.
	start_color();
	// Clear screen.
	clear();
	// For reading arrow-keys.
	keypad(stdscr, true);

	// Refresh screen.
	refresh();

	// Turn off cursor.
	curs_set(0);

	// Default menu options start column.
	menuOptionXPos = 43;

	isPlayerInit = false;
	isGameRunning = true;
	enteredBattleMode = false;

	playerCurrentWeapon = 0;
	playerCurrentArmour = 0;
	playerCurrentShield = 0;

	playerST = 10;
	playerDX = 10;
	playerHT = 10;

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
{
	delete player;
}


void LogWriter::WriteToLog(WINDOW* logScreen, bool clearScreen, const char* text)
{
	if (currentLogPosition >= logHeight || clearScreen)
	{
		currentLogPosition = 1;
		wclear(logScreen);
		mvwprintw(logScreen, 0, 0, "Logs");
	}

	mvwprintw(logScreen, currentLogPosition, 1, text);

	std::string tmp = text;
	if (tmp.size() >= (unsigned)logScreen->_maxx)
	{
		currentLogPosition += 2;
	}
	else
	{
		currentLogPosition += 1;
	}


	

	wrefresh(logScreen);

}
LogWriter::LogWriter(int lH) : currentLogPosition(1), logHeight(lH) {}
LogWriter::~LogWriter() {}
