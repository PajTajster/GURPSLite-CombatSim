#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <memory>

#include "SimLogic.h"
#include "json.hpp"
using json = nlohmann::json;



int DiceRoller::RollDice(int dices, int bonus)
{

	int diceResult = 0;
	for (int i = 0; i < dices; ++i)
		diceResult += rand() % 6 + 1;

	return diceResult + bonus;
}

DiceRoller::DiceRoller() {}

DiceRoller::~DiceRoller() {}


std::vector<std::string> Skill::PrintSkill()
{
	std::vector<std::string> message;

	message.push_back("Name: " + name);

	if (noDefaults)
	{
		message.push_back("Skill has no base proficiency");
	}
	else
	{
		message.push_back("Bonus: " + std::to_string(defaultBonus));
		message.push_back("Default Attribute: " + defaultAttribute);
		message.push_back("Optional Attribute: " + defaultOptionalAttribute);
	}

	return message;
}

Skill::Skill() : name("Undefined") { }

Skill::Skill(std::string nm, std::string dftAt, std::string dftOptAt,
			int dB, bool noDef) :
			name(nm), defaultAttribute(dftAt), defaultOptionalAttribute(dftOptAt),
			defaultBonus(dB), noDefaults(noDef) { }			


std::vector<std::string> Shield::PrintShield()
{
	std::vector<std::string> message;

	message.push_back("Name: " + name);
	message.push_back("Armor bonus: " + std::to_string(bonus));

	return message;
}


Shield::Shield() : name("Undefined") { }

Shield::Shield(std::string n, int b) : name(n), bonus(b) { }


std::vector<std::string> Armour::PrintArmour()
{
	std::vector<std::string> message;

	message.push_back("Name: " + name);
	message.push_back("Passive Defence: " + std::to_string(passiveDefence));
	message.push_back("Damage Resistance: " + std::to_string(damageResistance));

	return message;
}

Armour::Armour() : name("Undefined") { }

Armour::Armour(std::string n, int pD, int dR) :
	name(n), passiveDefence(pD), damageResistance(dR) { }


std::vector<std::string> Weapon::PrintWeapon()
{
	std::vector<std::string> message;

	message.push_back("Name: " + name);
	message.push_back("Damage: " + std::to_string(damage.dices) + "(Dices), "
		+ std::to_string(damage.bonus) + "(Bonus)");
	message.push_back("Skill used: " + skill);

	if (isMelee)
	{
		if (isTwoHanded)
		{
			message.push_back("This is Two-Handed weapon.");
		}
		else
		{
			message.push_back("This is One-Handed weapon.");
		}
	}
	else
	{
		message.push_back("This is Ranged weapon");
		message.push_back("Rate of Fire: " + std::to_string(rateOfFire));
	}

	return message;
}

Weapon::Weapon() : name("Undefined") { }

Weapon::Weapon(std::string n, Damage d, std::string s, bool isM,
				int rOF, bool isTH) :
	name(n), damage(d), skill(s), isMelee(isM), rateOfFire(rOF), isTwoHanded(isTH) { }




std::string Character::Attack(Character& target)
{
	// If target happens to be on the same team, abort.
	if (target.team == team)
		return "You can't attack your allies!";

	// Before calculating damage, find the skill the weapon uses in attacker's skills vector.
	std::string skillToFind = currentWeapon.skill;

	auto weaponSkill = std::find_if(skills.cbegin(), skills.cend(),
		[skillToFind](const Skill& s)->bool {return s.name == skillToFind; });

	if (weaponSkill == skills.cend())
		return name + " wants to use some abnormal technology!";

	std::string message = "";
	// A little speed up for concatenating. 
	message.reserve(120);
	if (isPlayer)
		message = "Attempting to attack: ";
	else
		message = this->name + " tries to attack: ";

	message.append(target.name + " ...");

	// If attacker's using melee weapon:
	if (currentWeapon.isMelee)
	{
		// Roll for an attack.
		int roll = diceRoller.RollDice(3, 0);

		// Critical miss, applies knockdown effect for 1 turn.
		if (roll >= 17)
		{
			message.append("Critically missed! Lost balance, knockdown for 1 turn.");
			isKnockedDown = true;
			knockDownTimer = 1;
		}
		// Critial hit, ignores whole defending step and applies x2 damage.
		else
			if (roll <= 3)
			{
				int damageApplied = diceRoller.RollDice(currentWeapon.damage.dices + baseMeleeDamage.dices,
					currentWeapon.damage.bonus + baseMeleeDamage.bonus);

				int finalDamage = target.ReceiveDamage(damageApplied * 2);

				if (finalDamage == 0)
				{
					message.append("Your " + currentWeapon.name +
						" bounced off the " + target.name + " armour!");
				}
				else
				{
					message.append("Critically hit for ");
					message.append(std::to_string(damageApplied) + " hp!");
				}
				if (target.isKnockedDown)
					message.append("(KNOCKED DOWN!)");
			}
		// Normal hit, needs to roll for defend 
			else if (roll <= weaponSkill->proficiency)
			{
				// If yes, apply damage to target
				if (target.DidGetHit())
				{
					int damageApplied = diceRoller.RollDice(currentWeapon.damage.dices + baseMeleeDamage.dices,
						currentWeapon.damage.bonus + baseMeleeDamage.bonus);

					int finalDamage = target.ReceiveDamage(damageApplied);

					if (finalDamage == 0)
					{
						message.append("Your " + currentWeapon.name +
							" bounced off the " + target.name + " armour!");
					}
					else
					{
						message.append("Target hit for ");
						message.append(std::to_string(damageApplied) + " hp!");
					}
					if (target.isKnockedDown)
						message.append("(KNOCKED DOWN!)");

				}
				// If no, then attack is blocked and round goes on.
				else
				{
					message.append("Attack blocked!");
				}
			}
			else
				message.append("Miss!");

		// Since character attacked already then we set bool to true.
		hasAttackedThisTurn = true;
	}
	// If weapon is a ranged one.
	else
	{
		message.append("Target hit for: ");
		// Because ranged weapons have Rate Of Fire value, a loop is used
		// because every projectile is independent to each other.
		for (int i = 0; i < currentWeapon.rateOfFire; ++i)
		{
			// Roll for an attack.
			int roll = diceRoller.RollDice(3, 0);

			// Critical miss, applies knockdown effect for 1 turn.
			if (roll >= 17)
			{
				message.append("Critically missed! Lost balance, knockdown for 1 turn.");
				isKnockedDown = true;
				knockDownTimer = 1;
				// Since character landed on the floor, they can't shoot any more bullets.
				break;
			}
			// Critial hit, ignores whole defending step and applies x2 damage.
			else if (roll <= 3)
				{
					int damageApplied = diceRoller.RollDice(currentWeapon.damage.dices,
													currentWeapon.damage.bonus);

					int finalDamage = target.ReceiveDamage(damageApplied * 2);

					if (finalDamage == 0)
					{
						message.append("0!");
					}
					else
					{
						message.append(std::to_string(damageApplied) + " hp! ");
					}
					if (target.isKnockedDown)
						message.append("(KNOCKED DOWN!)");
				}
			// Normal hit, needs to roll for defend 
			else if (roll <= weaponSkill->proficiency)
				{
					// If yes, apply damage to target
					if (target.DidGetHit())
					{
						int damageApplied = diceRoller.RollDice(currentWeapon.damage.dices,
														currentWeapon.damage.bonus);
						int finalDamage = target.ReceiveDamage(damageApplied);

						if (finalDamage == 0)
						{
							message.append("0 hp!");
						}
						else
						{
							message.append(std::to_string(damageApplied) + " hp!");
						}

						if (target.isKnockedDown)
							message.append("(KNOCKED DOWN!)");
					}
					// If no, then attack is blocked and round goes on.
					else
					{
						message.append("Projectile bounced off the " + target.name + " armour!");
					}
				}
				else
					message.append("Miss!");
		}
		// Since character attacked already then we set bool to true.
		// in case of ranged weapons, attack is done after all the
		// projectiles(Rate of Fire) were shot.
		hasAttackedThisTurn = true;
	}	
	if (target.isDead)
		message.append("\n" + target.name + " lifeless body landed on the floor.");

	return message;
}

bool Character::DidGetHit()
{
	// Firstly calculate our total defence, which is sum of all
	// defences the character has. [if knockdown then only passive]
	int totalDefense = 0;

	if (isKnockedDown)
		totalDefense = passiveDefence;
	// Choose what type of active defence will be considered.
	else
	{
		int whatDef = rand() % 3;
		switch (whatDef)
		{
		case 0:
			totalDefense = block + passiveDefence;
			break;
		case 1:
			totalDefense = dodge + passiveDefence;
			break;
		case 2:
			totalDefense = parry + passiveDefence;
			break;
		default:
			return true;
		}
	}

	// Now we roll 3d6 against it.
	int diceRoll = diceRoller.RollDice(3, 0);

	// Roll of 4 or less is always a success.
	if (diceRoll <= 4)
		return false;
	// Roll of 17 or 18 is always a failure.
	else if (diceRoll >= 17)
		return true;
	// If roll is less than or equal to totalDefense, then it's a success.
	else if (diceRoll < totalDefense)
		return false;

	// If still nothing, then it's a hit.
	return true;
}

int Character::ReceiveDamage(int damage)
{
	int finalDamage = damage - damageResistance;

	// This could mean that the attack just bounced off the armour.
	if (finalDamage < 0)
		return 0;

	// If character got damaged for more or equal 3/4 of it's HP, they get knocked down.
	if (((3 * health) / 4) <= finalDamage)
	{
		isKnockedDown = true;
		knockDownTimer++;
	}

	health -= finalDamage;

	// R.I.P.
	if (health <= 0)
		isDead = true;

	return finalDamage;
}

void Character::InitializeCharacter(int initST, int initDX, int initHT, int initVet,
	std::string initName, std::vector<Skill> initSkills, Weapon initWeapon,
	Armour initArmour, Shield initShield)
{
	strength = initST;
	dexterity = initDX;
	health = initHT;
	veterancy = initVet;

	name = initName;

	skills = initSkills;
	currentWeapon = initWeapon;
	currentArmour = initArmour;
	if (currentArmour.name != "None")
		isWearingArmour = true;
	currentShield = initShield;
	if (currentShield.name != "None")
		isWieldingShield = true;
	// If somehow character passed in is using twohanded weapon the shield won't be added.
	if (currentWeapon.isTwoHanded)
		isWieldingShield = false;

	skills = initSkills;

	CalculateSkillsDefaults();
	CalculateExtraAttributes();

	return;
}

void Character::CalculateExtraAttributes()
{
	// Player has his own intelligence, no need for extra ones.
	if (isPlayer)
		usedAI = AI_NULL;

	// If no shield, then block is 0
	// if there's one, then it's half of "Shield" skill proficiency.
	if (!isWieldingShield)
		block = 0;
	else
	{
		std::string skillToFind = "Shield";

		auto shieldSkill = std::find_if(skills.cbegin(), skills.cend(),
			[skillToFind](const Skill& s)->bool {return s.name == skillToFind; });

		if (shieldSkill == skills.cend())
			block = 0;
		else
			block = shieldSkill->proficiency;
	}

	// If no weapon is used, then parry is 0
	// but if it is, then parry is half of weapon's skill's proficiency.
	if (currentWeapon.name == "Bare Fists")
		parry = 0;

	else
	{
		std::string skillToFind = currentWeapon.skill;

		auto weaponSkill = std::find_if(skills.cbegin(), skills.cend(),
			[skillToFind](const Skill& s)->bool {return s.name == skillToFind; });

		if (weaponSkill == skills.cend())
			parry = 0;
		else
			parry = weaponSkill->proficiency / 2;
	}

	basicSpeed = static_cast<float>((health + dexterity) / 4);
	dodge = static_cast<int>(basicSpeed);
		
	// If there is armour on character's body, then he "inherits" it's bonuses
	// if not, it's 0.
	if (currentArmour.name == "Ordinary Clothes")
		isWearingArmour = false;
	else
		isWearingArmour = true;

	if (isWearingArmour)
	{
		passiveDefence = currentArmour.passiveDefence;
		damageResistance = currentArmour.damageResistance;
	}
	else
	{
		passiveDefence = 0;
		damageResistance = 0;
	}

	if (strength <= 4)
	{
		baseMeleeDamage.dices = 1;
		baseMeleeDamage.bonus = -6;
	}
	else if (strength > 4 && strength <= 10)
	{
		baseMeleeDamage.dices = 1;
	}
	else if (strength > 10)
	{
		baseMeleeDamage.dices = 2;
	}
	baseMeleeDamage.bonus = strength / 3;

	CalculateSkillsDefaults();
}

void Character::CalculateSkillsDefaults()
{
	for (auto& i : skills)
	{
		int defaultAttProf = 0;
		int defaultOptionalProf = 0;
		int finalProf = 0;

		// If noDefaults flag is set, then skill proficiency is 0.
		if (i.noDefaults)
		{
			i.proficiency = 0;
			continue;
		}
		else
		{
			// S - Strength, D - Dexterity, H - Health
			// Calculate default proficiency.
			switch (i.defaultAttribute[0])
			{
			case 'S':
				defaultAttProf = strength + i.defaultBonus;
				break;
			case 'D':
				defaultAttProf = dexterity + i.defaultBonus;
				break;
			case 'H':
				defaultAttProf = health + i.defaultBonus;
				break;
			default:
				defaultAttProf = 0;
				break;
			}

			// If there's a second optional default Attribute, we calculate it also.
			if (i.defaultOptionalAttribute != "None")
			{
				switch (i.defaultOptionalAttribute[0])
				{
				case 'S':
					defaultOptionalProf = strength + i.defaultBonus;
					break;
				case 'D':
					defaultOptionalProf = dexterity + i.defaultBonus;
					break;
				case 'H':
					defaultOptionalProf = health + i.defaultBonus;
					break;
				default:
					defaultOptionalProf = 0;
					break;
				}

				// We take better proficiency.
				finalProf = (defaultAttProf > defaultOptionalProf) ? defaultAttProf : defaultOptionalProf;
			}
			// If there's no optional default attribute, we take the original one.
			else
				finalProf = defaultAttProf;
		}
		// Veterancy raises skill proficiency.
		finalProf += veterancy;

		// Just for the sake of any success.
		if (finalProf < 3)
			i.proficiency = 3;
		else
			i.proficiency = finalProf;
	}
}

int Character::GetHealth() { return health; }

float Character::GetInitiative() { return basicSpeed; }

int Character::nextID = 0;

bool Character::ModifyAttribute(int value, char attribute)
{
	// Checks whether modification is possible
	switch (attribute)
	{
	case 'S':
	case 'D':
	case 'H':
		if (characterPoints < value * 10)
			return false;
		break;
	case 'V':
		if (characterPoints < value * 30)
			return false;
		break;
	default:
		return false;
	}


	switch (attribute)
	{
	case 'S':
		// Can't go lower than 1.
		if (value < 0 && strength == 1)
			return false;
		strength += value;
		characterPoints -= (value * 10);
		break;
	case 'D':
		// Can't go lower than 1.
		if (value < 0 && dexterity == 1)
			return false;
		dexterity += value;
		characterPoints -= (value * 10);
		break;
	case 'H':
		// Can't go lower than 1.
		if (value < 0 && health == 1)
			return false;
		health += value;
		characterPoints -= (value * 10);
		break;
	case 'V':
		// Can't go lower than 0.
		if (value < 0 && veterancy == 0)
			return false;
		// Can't go above 10.
		if (value > 0 && veterancy == 10)
			return false;
		veterancy += value;
		characterPoints -= (value * 30);
	default:
		break;
	}

	return true;
}

int Character::GetCharacterPoints() { return characterPoints; }

void Character::NPCSelectTarget(std::vector<Character>& charactersToChoose)
{
	// If NPC is dead or knocked down, he does nothing.
	if (isDead || isKnockedDown)
		return;

	if (doesNPCHaveTarget)
	{
		if (!charactersToChoose[currentTargetIndex].isDead)
			return;
	}
	bool foundTarget = false;
	int newTargetIndex = 0;

	switch (usedAI)
	{
		// NPC chooses opponent with most health.
	case AI_TARGET_STRONGEST:
	{
		int max = 0;
		int i = 0;
		int selectedTarget = 0;
		for (auto& it : charactersToChoose)
		{
			if (this->team == it.team || this->ID == it.ID || it.isDead)
			{
				++i;
				continue;
			}
			if (it.GetHealth() > max)
			{
				foundTarget = true;
				selectedTarget = i;
			}
			++i;
		}

		currentTargetIndex = selectedTarget;
		break;
	}
	// NPC chooses opponent with least health.
	case AI_TARGET_WEAKEST:
	{
		int min = 999;
		int i = 0;
		int selectedTarget = 0;
		for (auto& it : charactersToChoose)
		{
			if (this->team == it.team || this->ID == it.ID || it.isDead)
			{
				++i;
				continue;
			}
			if (it.GetHealth() < min)
			{
				foundTarget = true;
				selectedTarget = i;
			}
			++i;
		}

		currentTargetIndex = selectedTarget;
		break;
	}
	// NPC chooses random opponent.
	case AI_TARGET_RANDOM:
	{
		int randTarget = 0;
		randTarget = rand() % charactersToChoose.size();

		while (charactersToChoose[randTarget].ID == this->ID ||
			charactersToChoose[randTarget].team == this->team ||
			charactersToChoose[randTarget].isDead)
		{
			randTarget = rand() % charactersToChoose.size();
			foundTarget = true;
		}

		

		currentTargetIndex = randTarget;
	}

	break;
	default:
		return;
	}

	if (!foundTarget)
		currentTargetIndex = -1;
	else
	{
		doesNPCHaveTarget = true;
	}
}

std::string Character::NPCAssessSituation(std::vector<Character>& charactersToChoose)
{
	std::string message = "";


	if (currentTargetIndex == -1)
	{
		NPCSelectTarget(charactersToChoose);
	}
	if (currentTargetIndex != -1)
	{

		if (charactersToChoose[currentTargetIndex].isDead)
		{
			NPCSelectTarget(charactersToChoose);
			if (currentTargetIndex == -1)
				return message;
		}

		// If NPC uses melee weapon resolve this branch.
		if (currentWeapon.isMelee)
		{
			message = Attack(charactersToChoose[currentTargetIndex]);
		}
		// If NPC uses ranged weapon instead, do that:
		else
		{
			// Well, shoot stuff!
			message = Attack(charactersToChoose[currentTargetIndex]);
		}


	}


	return message;
}

void Character::SetTeam(int teamToSet)
{
	if (teamToSet == 1 || teamToSet == 2)
		team = teamToSet;
}

int Character::GetTeam() { return team; }

std::vector<std::string> Character::PrintCharacter()
{
	std::vector<std::string> message;


	message.push_back("Name: " + name);
	message.push_back("Strength: " + std::to_string(strength));
	message.push_back("Dexterity: " + std::to_string(dexterity));
	message.push_back("Health: " + std::to_string(health));
	message.push_back("Equipped Weapon: " + currentWeapon.name);
	message.push_back("Equipped Shield: " + currentShield.name);
	message.push_back("Equipped Armour: " + currentArmour.name);

	return message;
}

Character::Character(const Character& original)
{
	*this = original;
}
Character::Character(const Character& original, bool deep)
{
	*this = original;
	if (deep)
	{
		this->ID = ++nextID;
	}
}
Character::Character() : isWieldingShield(false), isDead(false), isPlayer(false),
isKnockedDown(false), hasAttackedThisTurn(false), ID(++nextID), doesNPCHaveTarget(false),
knockDownTimer(0), strength(10), dexterity(10), health(10), veterancy(0), currentTargetIndex(-1)
{
	usedAI = static_cast<AI>(rand() % AI_NULL);

	skills =
	{
		Skill("Undefined", "None", "None", 0, false),
	};
}

Character::~Character()
{
	skills.clear();
}



void GameMaster::CalculateInitiative()
{
	std::sort(charactersInPlay.begin(), charactersInPlay.end(),
		[](const auto& x, const auto& y) -> bool
		{
			return x.basicSpeed > y.basicSpeed;
		});
}

void GameMaster::NextTurn(std::vector<Character>& updatedVector)
{
	for (auto& i : updatedVector)
	{
		if (i.isDead)
			continue;
		if (i.knockDownTimer)
		{
			if (--i.knockDownTimer);
			else
				i.isKnockedDown = false;
		}
		else
		{
			i.hasAttackedThisTurn = false;
		}

	}
}

void GameMaster::ClearBattleData()
{
	charactersInPlay.clear();
	team1.clear();
	team2.clear();
}
void GameMaster::PrepareTeams(std::vector<Character> t1, std::vector<Character> t2)
{
	for (auto& it : t1)
	{
		AddCharacterToMainVector(it, 1);
	}
	for (auto& it : t2)
	{
		AddCharacterToMainVector(it, 2);
	}
}
void GameMaster::AddCharacterToTeam(int id, int teamToSet)
{
	int IDToFind = id;

	auto characterToPush = std::find_if(charactersInPlay.begin(), charactersInPlay.end(),
		[IDToFind](const auto &c) -> bool {return c.ID == IDToFind; });

	if (teamToSet == 1 || teamToSet == 2)
	{
		switch (teamToSet)
		{
		case 1:
			team1.push_back(*characterToPush);
			break;
		case 2:
			team2.push_back(*characterToPush);
			break;
		}
		characterToPush->SetTeam(teamToSet);
	}
	return;
}
void GameMaster::AddCharacterToMainVector(Character c, int team)
{
	int IDToFind = c.ID;

	auto characterToPush = std::find_if(charactersInPlay.cbegin(), charactersInPlay.cend(),
		[IDToFind](const auto &c) -> bool {return c.ID == IDToFind; });
	
	// Check whether character is placed in such vector, if not, add them.
	if ( characterToPush != charactersInPlay.cend())
	{
		return;
	}
	else
	{
		if (!c.isPlayer)
		{
			RandomizeName(c);
		}
		charactersInPlay.push_back(c);
		AddCharacterToTeam(c.ID, team);
	}
}

void GameMaster::RandomizeName(Character& c)
{
	std::string oldName = c.name;
	std::string newName = "";

	// Take a random name from app data.
	newName = names[rand() % names.size()];

	newName.append(", the " + oldName);

	c.name = newName;
}

bool GameMaster::InitializeGameMaster()
{
	// Init data
	if (LoadNames() == -1)
		return false;
	if (LoadSkills() == -1)
		return false;
	if (LoadArmours() == -1)
		return false;
	if (LoadShields() == -1)
		return false;
	if (LoadWeapons() == -1)
		return false;
	if (LoadCharacters() == -1)
		return false;

	return true;
}

Character* GameMaster::InitBasePlayer()
{
	Character* player = new Character();

	player->name = "Nobody";
	player->currentArmour = allArmours[0];
	player->currentWeapon = allWeapons[0];
	player->skills = allSkills;
	player->CalculateExtraAttributes();
	player->CalculateSkillsDefaults();

	player->isPlayer = true;

	player->characterPoints = 100;


	return player;
}

void GameMaster::UpdatePlayer(Character* player, std::vector<Character>& updatedVector)
{
	int IDToFind = player->ID;

	auto characterToFind = std::find_if(updatedVector.cbegin(), updatedVector.cend(),
		[IDToFind](const auto &c) -> bool {return c.ID == IDToFind; });

	if (characterToFind == updatedVector.cend())
		return;
	
	*player = *characterToFind;
}

int GameMaster::LoadCharacters()
{
	std::ifstream ifs("characters.json");
	if (!ifs.good())
	{
		return -1;
	}
	json j = json::parse(ifs);
	
	json charactersArray = j["characters"];


	for (auto& it : charactersArray)
	{
		int newST = it["strength"];
		int newDX = it["dexterity"];
		int newHT = it["health"];
		int newVet = it["veterancy"];
		std::string newName = it["name"];
		std::string newWeap = it["weapon"];
		std::string newArm = it["armour"];
		std::string newShld = it["shield"];


		// Checks if character is using items loaded previously. If not, it won't be added.

		// Weapon
		std::string nameToSearch = newWeap;

		auto searchedWeapon = std::find_if(allWeapons.cbegin(), allWeapons.cend(),
			[nameToSearch](const Weapon& w) -> bool {return w.name == nameToSearch; });
		
		Weapon newWeapon;

		// Take default one if failed to load.
		if (searchedWeapon == allWeapons.cend())
			newWeapon = allWeapons[0];
		else
			newWeapon = *searchedWeapon;

		// Armour
		nameToSearch = newArm;

		auto searchedArmour = std::find_if(allArmours.cbegin(), allArmours.cend(),
			[nameToSearch](const Armour& a) -> bool {return a.name == nameToSearch; });

		Armour newArmour;

		// Take default one if failed to load.
		if (searchedArmour == allArmours.cend())
			newArmour = allArmours[0];
		else
			newArmour = *searchedArmour;
		
		// Shield
		nameToSearch = newShld;

		auto searchedShield = std::find_if(allShields.cbegin(), allShields.cend(),
			[nameToSearch](const Shield& s) -> bool {return s.name == nameToSearch; });

		Shield newShield;

		// Take default one if failed to load.
		if (searchedShield == allShields.cend())
			newShield = allShields[0];
		else
			newShield = *searchedShield;

		Character newCharacter;
		newCharacter.InitializeCharacter(newST, newDX, newHT, newVet,
			newName, allSkills, newWeapon, newArmour, newShield);

		allCharacters.push_back(newCharacter);
	}
	return 0;
}

int GameMaster::LoadSkills()
{
	std::ifstream ifs("skills.json");
	if (!ifs.good())
	{
		return -1;
	}
	json j = json::parse(ifs);

	json skillsArray = j["skills"];

	for (auto& it : skillsArray)
	{
		std::string newName = it["name"];
		std::string newDefAtt = it["defaultAtt"];
		std::string newDefOptAtt = it["defaultOptionalAtt"];
		int newDefBonus = it["defaultBonus"];

		bool newNoDefaults;
		if (newDefAtt == "None")
			newNoDefaults = true;
		else
			newNoDefaults = false;

		Skill newSkill(newName, newDefAtt,
			newDefOptAtt, newDefBonus, newNoDefaults);

		allSkills.push_back(newSkill);
	}

	return 0;
}

int GameMaster::LoadArmours()
{
	std::ifstream ifs("armours.json");
	if (!ifs.good())
	{
		return -1;
	}
	json j = json::parse(ifs);

	json armoursArray = j["armours"];

	for (auto& it: armoursArray)
	{
		std::string newName = it["name"].get<std::string>();
		int newPD = it["passiveDefence"];
		int newDR = it["damageResistance"];

		Armour newArmour(newName, newPD, newDR);
		
		allArmours.push_back(newArmour);
	}
	return 0;
}

int GameMaster::LoadWeapons()
{
	std::ifstream ifs("weapons.json");
	if (!ifs.good())
	{
		return -1;
	}
	json j = json::parse(ifs);

	json weaponsArray = j["weapons"];

	for (auto& it : weaponsArray)
	{
		std::string newName = it["name"];
		int newDD = it["damageDices"];
		int newDB = it["damageBonus"];
		std::string newSkillName = it["skillName"];
		bool newIsM = it["isMelee"];
		bool newIsTH = it["isTwoHanded"];
		int newRoF = it["rateOfFire"];

		// Check if the skill used by weapon exists in skills vector.
		auto foundSkill = std::find_if(allSkills.cbegin(), allSkills.cend(),
			[newSkillName](const Skill& s) -> bool {return s.name == newSkillName; });

		// If not, weapon won't be added.
		if (foundSkill != allSkills.cend())
		{
			Damage newDamage;
			newDamage.dices = newDD;
			newDamage.bonus = newDB;

			// Take skill from the iterator.
			std::string newSkill = foundSkill->name;

			Weapon newWeapon(newName, newDamage, newSkill, 
					newIsM, newRoF, newIsTH);

			allWeapons.push_back(newWeapon);
		}
	}
	return 0;
}

int GameMaster::LoadShields() 
{
	std::ifstream ifs("shields.json");
	if (!ifs.good())
	{
		return -1;
	}
	json j = json::parse(ifs);

	json shieldsArray = j["shields"];

	for (auto& it : shieldsArray)
	{
		std::string newName = it["name"];
		int newBonus = it["bonus"];

		Shield newShield(newName, newBonus);

		allShields.push_back(newShield);
	}
	return 0;
}

int GameMaster::LoadNames()
{
	std::ifstream ifs("names.json");

	if (!ifs.good())
	{
		return -1;
	}

	json j = json::parse(ifs);

	json namesArray = j["names"];

	for (auto& it : namesArray)
	{
		std::string newName = it;

		names.push_back(newName);
	}

	return 0;
}

std::vector<Character> GameMaster::GetCharacters()
{
	return allCharacters;
}
std::vector<Skill> GameMaster::GetSkills()
{
	return allSkills;
}
std::vector<Armour> GameMaster::GetArmours()
{
	return allArmours;
}
std::vector<Weapon> GameMaster::GetWeapons()
{
	return allWeapons;
}
std::vector<Shield> GameMaster::GetShields()
{
	return allShields;
}
std::vector<Character>& GameMaster::GetCharactersInPlay(){ return charactersInPlay; }

GameMaster::GameMaster() { }

GameMaster::~GameMaster()
{
	allCharacters.clear();
	allSkills.clear();
	allArmours.clear();
	allWeapons.clear();

	charactersInPlay.clear();
	team1.clear();
	team2.clear();
}
