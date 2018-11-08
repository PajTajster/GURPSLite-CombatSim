#include "SimTUI.h"


void MenuUIHelper::Init()
{
	gm.InitializeGameMaster();

	player = gm.InitBasePlayer();
	
	MainMenu();
}

void MenuUIHelper::MainMenu()
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
				PlayerCreationMenu();
				break;
			case 1:
				if (isPlayerInit)
				{
					PrepareTeamMenu();
				}
				if (!isGameRunning)
				{
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
	int optionPos[] = { 1, 2, 3, 4, 6, 7, 8, 10, 12, 14 };

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
		playerCreationMenuOptions[4] = "Weapon: " + weaponsToChoose[playerCurrentWeapon].name;
		playerCreationMenuOptions[5] = "Armour: " + armoursToChoose[playerCurrentArmour].name;


		if (weaponWithShield)
		{
			playerCreationMenuOptions[6] = "Shield: " + shieldsToChoose[playerCurrentShield].name;
		}
		else
		{
			playerCreationMenuOptions[6] = "Shield: None [Ranged or Two-Handed Weapon Equipped]";
		}
		if (outOfPoints)
		{
			playerCreationMenuOptions[7] = "Character Points Left: " + std::to_string(player->characterPoints);
			playerCreationMenuOptions[7].append("(Stats at minimum/Not enough points!)");
		}
		else
		{
			playerCreationMenuOptions[7] = "Character Points Left: " + std::to_string(player->characterPoints);
		}

		if (isPlayerNameSet)
		{
			playerCreationMenuOptions[8] = "Done";
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
			break;
		case KEY_DOWN:
			if (currentOption != 9)
			{
				++currentOption;
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

				// Weapon
			case 4:
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
			case 5:
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
			case 6:
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

				// Weapon
			case 4:
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
			case 5:
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
			case 6:
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
			case 8:
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
					playerCreationMenuOptions[8] = "[Name is not set!]";
				}
				break;

				// Go back
			case 9:
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
		"3 vs 3",
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
			break;
		case KEY_DOWN:
			if (currentOption != 4)
			{
				++currentOption;
				previousPos = currentPos;
				currentPos = optionPos[currentOption];
			}
			break;
		case 10:
		{
			switch (currentPos)
			{				
				// 1 vs 1
			case 5:
				teamSize = 1;
				SelectFightersMenu();
				break;

				// 2 vs 2 
			case 7:
				teamSize = 2;
				SelectFightersMenu();
				break;

				// 3 vs 3
			case 10:
				teamSize = 3;
				SelectFightersMenu();
				break;

				// Go back
			case 12:
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
	std::vector<std::string> chooseFightersOptions =
	{
		"Choose Team 1 Fighters:",
		"Choose Team 2 Fighters:",
		"Back",
		"Next",
		"Go back"
	};

	// Keep record for Team 1 Characters [Player is team1]
	int team1Characters = 1;
	// Keep record for Team 2 Characters
	int team2Characters = 0;

	// 1 for Team 1, 2 for Team 2
	int currentTeamBeingChosen = 1;


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


	std::vector<Character> allCharacters = gm.GetCharacters();

	// Currently showed character
	int currentChar = 0;
	int maxChar = allCharacters.size() - 1;

	// Menu Positions

	// Horizontal line for options
	int menuOptionsPos = 2;
	// Positions for all 3 options
	int optionPos[] = { 15, 25, 35, 45 };
	// Current option indicated by an arrow
	int currentOption = 0;
	// Holds previous arrow position
	int previousPos = 0;
	// Current arrow position
	int currentPos = optionPos[0] - 1;

	while (true)
	{
		wclear(menu);

		mvwprintw(menu, menuOptionsPos - 1, menuOptionXPos, "Choose Team 2 Fighter");

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
			if (currentOption != 3)
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

				// Select
			case 2:
				return;

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
		default:
			break;
		}
	}
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
			break;
		case KEY_DOWN:
			if (currentOption != 5)
			{
				++currentOption;
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
	int optionPos[] = { 29, 39, 49 };
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
	int optionPos[] = { 29, 39, 49 };
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
	int optionPos[] = { 29, 39, 49 };
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
	int optionPos[] = { 29, 39, 49 };
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
	int optionPos[] = { 29, 39, 49 };
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
	// Clear screen.
	clear();
	// For reading arrow-keys.
	keypad(stdscr, true);

	// Refresh screen.
	refresh();

	// "Logo" window.
	logo = newwin(7, 30, 1, 30);
	// "Menu" window.
	menu = newwin(0, 0, 8, 0);

	// Refresh both new windows.

	wrefresh(logo);
	wrefresh(menu);

	// Turn off cursor.
	curs_set(0);

	// Default menu options start column.
	menuOptionXPos = 30;

	isPlayerInit = false;
	isGameRunning = true;

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
