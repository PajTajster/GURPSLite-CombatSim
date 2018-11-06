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
	srand((unsigned)time(NULL));
	int diceResult = 0;
	for (int i = 0; i < dices; ++i)
		diceResult += rand() % 6 + 1;

	return diceResult + bonus;
}

DiceRoller::DiceRoller() {}

DiceRoller::~DiceRoller() {}


std::string Skill::PrintSkill()
{
	std::string message;

	message.append("Name: " + name + "\n");

	if (noDefaults)
	{
		message.append("Skill has no base proficiency");
	}
	else
	{
		message.append("Bonus: " + std::to_string(defaultBonus) + 
		"\nDefault Attribute: " + defaultAttribute +
		"\nOptional Attribute: " + defaultOptionalAttribute);
	}

	return message;
}

Skill::Skill() : name("Undefined") { }

Skill::Skill(std::string nm, std::string dftAt, std::string dftOptAt,
			int dB, bool noDef) :
			name(nm), defaultAttribute(dftAt), defaultOptionalAttribute(dftOptAt),
			defaultBonus(dB), noDefaults(noDef) { }			


std::string Shield::PrintShield()
{
	std::string message;

	message.append("Name: " + name +
		"\nArmor bonus: " + std::to_string(bonus));

	return message;
}


Shield::Shield() : name("Undefined") { }

Shield::Shield(std::string n, int b) : name(n), bonus(b) { }


std::string Armour::PrintArmour()
{
	std::string message;

	message.append("Name: " + name +
		"\nPassive Defence: " + std::to_string(passiveDefence) +
		"\nDamage Resistance: " + std::to_string(damageResistance));

	return message;
}

Armour::Armour() : name("Undefined") { }

Armour::Armour(std::string n, int pD, int dR) :
	name(n), passiveDefence(pD), damageResistance(dR) { }


std::string Weapon::PrintWeapon()
{
	std::string message;

	message.append("Name: " + name +
		"\nDamage: " + std::to_string(damage.dices) + "(Dices), " +
		std::to_string(damage.bonus) + "(Bonus)" +
		"\nSkill used: " + skill.name + "\n");

	if (isMelee)
	{
		if (isTwoHanded)
		{
			message.append("This is Two-Handed weapon.");
		}
		else
		{
			message.append("This is One-Handed weapon.");
		}
	}
	else
	{
		message.append("This is Ranged weapon" \
			"\nRate of Fire: " + std::to_string(rateOfFire));
	}

	return message;
}

Weapon::Weapon() : name("Undefined") { }

Weapon::Weapon(std::string n, Damage d, Skill s, bool isM,
				int rOF, bool isTH) :
	name(n), damage(d), skill(s), isMelee(isM), rateOfFire(rOF), isTwoHanded(isTH) { }




std::string Character::Attack(Character target, DiceRoller dr)
{
	// If target happens to be on the same team, abort.
	if (target.team == team)
		return "You can't attack your allies!";

	std::string message = "";
	// A little speed up for concatenating. 
	message.reserve(120);
	if (isPlayer)
		message = this->name + " tries to attack: ";
	else
		message = "Attempting to attack: ";
	message.append(name + " ...");

	// If attacker's using melee weapon:
	if (currentWeapon.isMelee)
	{
		if (true)
		{
			// Roll for an attack.
			int roll = dr.RollDice(3, 0);

			// Critical miss, applies knockdown effect for 1 turn.
			if (roll >= 17)
			{
				message.append("Critically missed! You lost balance, knockdown for 1 turn.");
				isKnockedDown = true;
				knockDownTimer = 1;
			}
			// Critial hit, ignores whole defending step and applies x2 damage.
			else
			if (roll <= 3)
			{
				int damageApplied = dr.RollDice(currentWeapon.damage.dices + baseMeleeDamage.dices,
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
					message.append(damageApplied + "ht!");
				}
			}
			// Normal hit, needs to roll for defend 
			else if (roll < currentWeapon.skill.proficiency)
			{
				// If yes, apply damage to target
				if (DidGetHit(target, dr))
				{
					int damageApplied = dr.RollDice(currentWeapon.damage.dices + baseMeleeDamage.dices,
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
						message.append(damageApplied + "ht!");
					}

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
		else
		{
			return "You're out of range!";
		}
	}
	// If weapon is a ranged one.
	else
	{
		// Because ranged weapons have Rate Of Fire value, a loop is used
		// because every projectile is independent to each other.
		for (int i = 0; i < currentWeapon.rateOfFire; ++i)
		{
			// If target is dead, no point in shooting a corpse and wasting time.
			if (target.isDead)
			{
				message.append("\nTarget is dead, you lower your " + currentWeapon.name);
			}
			message.append("[Bullet " + std::to_string(i + 1) + "]\n");
			// Roll for an attack.
			int roll = dr.RollDice(3, 0);

			// Critical miss, applies knockdown effect for 1 turn.
			if (roll >= 17)
			{
				message.append("Critically missed! You lost balance, knockdown for 1 turn.");
				isKnockedDown = true;
				knockDownTimer = 1;
				// Since character landed on the floor, they can't shoot any more bullets.
				break;
			}
			// Critial hit, ignores whole defending step and applies x2 damage.
			else if (roll <= 3)
				{
					int damageApplied = dr.RollDice(currentWeapon.damage.dices,
													currentWeapon.damage.bonus);

					int finalDamage = target.ReceiveDamage(damageApplied * 2);

					if (finalDamage == 0)
					{
						message.append("Projectile bounced off the " + target.name + " armour!");
					}
					else
					{
						message.append("Critically hit for ");
						message.append(damageApplied + "ht!");
					}
				}
			// Normal hit, needs to roll for defend 
			else if (roll < currentWeapon.skill.proficiency)
				{
					// If yes, apply damage to target
					if (DidGetHit(target, dr))
					{
						int damageApplied = dr.RollDice(currentWeapon.damage.dices,
														currentWeapon.damage.bonus);
						int finalDamage = target.ReceiveDamage(damageApplied);

						if (finalDamage == 0)
						{
							message.append("Projectile bounced off the " + target.name + " armour!");
						}
						else
						{
							message.append("Target hit for ");
							message.append(damageApplied + "ht!");
						}
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
	
	message.append("\n");

	return message;
}

bool Character::DidGetHit(Character attacker, DiceRoller dr)
{
	// Firstly calculate our total defence, which is sum of all
	// defences the character has. [if knockdown then only passive]
	int totalDefense = 0;

	if (isKnockedDown)
		totalDefense = passiveDefence;
	else
		totalDefense = (dodge + parry + block + passiveDefence);

	// Now we roll 3d6 against it.
	int diceRoll = dr.RollDice(3, 0);

	// Roll of 4 or less is always a success.
	if (diceRoll <= 4)
		return false;
	// Roll of 17 or 18 is always a failure.
	else if (diceRoll >= 17)
		return true;
	// If roll is less than or equal to totalDefense, then it's a success.
	else if (diceRoll < 17)
		return false;

	// Shouldn't happen
	return true;
}

int Character::ReceiveDamage(int damage)
{
	int finalDamage = damage - damageResistance;

	// This could mean that the attack just bounced off the armour.
	if (finalDamage < 0)
		return 0;

	// If character got damaged for 1/2 of it's HP, they get knocked down.
	if (health / 2 == finalDamage)
	{
		isKnockedDown = true;
		knockDownTimer++;
	}


	// R.I.P.
	if (health <= 0)
		isDead = true;

	return finalDamage;
}

void Character::InitializeCharacter(int initST, int initDX, int initHT,
	std::string initName, std::vector<Skill> initSkills,
	Weapon initWeapon, Armour initArmour, Shield initShield)
{
	strength = initST;
	dexterity = initDX;
	health = initHT;

	name = initName;

	skills = initSkills;
	currentWeapon = initWeapon;
	currentArmour = initArmour;
	if (currentArmour.name != "None")
		isWearingArmour = true;
	currentShield = initShield;
	// If somehow character passed in is using twohanded weapon the shield won't be added.
	if (currentShield.name != "None" || currentWeapon.isTwoHanded)
		isWieldingShield = true;

	skills = initSkills;

	CalculateExtraAttributes();

	return;
}

void Character::CalculateExtraAttributes()
{
	// If no shield, then block is 0
	// if there's one, then it's half of "Shield" skill proficiency.
	if (!isWieldingShield || currentWeapon.isTwoHanded)
		block = 0;
	else
		block = skills[0].proficiency;

	// If no weapon is used, then parry is 0
	// but if it is, then parry is half of weapon's skill's proficiency.
	if (currentWeapon.name == "Bare Fists")
		parry = 0;
	else
	{
		std::string skillToFind = currentWeapon.skill.name;

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

		i.proficiency = finalProf;
	}
}

int Character::GetHealth() { return health; }

float Character::GetInitiative() { return basicSpeed; }

Character::Character() : isWieldingShield(false), isDead(false),
						isKnockedDown(false), hasAttackedThisTurn(false),
						knockDownTimer(0), strength(10), dexterity(10),
						health(10)
{
	isPlayer = false;
	ID = ++nextID;

	skills =
	{
		Skill("Undefined", "None", "None", 0, false),
	};
}

int Character::nextID = 0;

void Character::SetTeam(int teamToSet)
{
	if (teamToSet == 1 || teamToSet == 2)
		team = teamToSet;
}

int Character::GetTeam() { return team; }

std::string Character::PrintCharacter()
{
	std::string message;


	message.append("Name: " + name +
		"\nStrength: " + std::to_string(strength) +
		"\nDexterity: " + std::to_string(dexterity) +
		"\nHealth: " + std::to_string(health) +
	"\nEquipped Weapon: " + currentWeapon.name +
	"\nEquipped Shield: " + currentShield.name +
	"\nEquipped Armour: " + currentArmour.name);

	return message;
}

Character& Character::operator=(const Character& original)
{
	ID = original.ID;
	return (*this);
}

Character::~Character()
{
	skills.clear();
}



bool Character::ModifyAttribute(int value, char attribute)
{
	if (characterPoints < value * 10)
		return false;

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
	default:
		break;
	}

	return true;
}

int Character::GetCharacterPoints() { return characterPoints; }



void Character::NPCSelectTarget(std::vector<Character> charactersToChoose)
{
	// If NPC is dead or knocked down, he does nothing.
	if (isDead || isKnockedDown)
		return;

	Character newTarget;	

	switch (usedAI)
	{
		// NPC chooses opponent with most health.
	case AI_TARGET_STRONGEST:
	{
		int max = 0;
		for (auto i : charactersToChoose)
		{
			if (i.GetHealth() > max)
				newTarget = i;
		}
		break;
	}
		// NPC chooses opponent with least health.
	case AI_TARGET_WEAKEST:
	{
		int min = 999;
		for (auto i : charactersToChoose)
		{
			if (i.GetHealth() < min)
				newTarget = i;
		}
		break;
	}
		// NPC chooses random opponent.
	case AI_TARGET_RANDOM:
	{
		srand((unsigned)time(NULL));
		int randTarget = 0;
		randTarget = rand() % charactersToChoose.size();
		newTarget = charactersToChoose[randTarget];
	}
		break;
	default:
		return;
	}

	*currentTarget = newTarget;
}

void Character::NPCAssessSituation()
{
	// If NPC uses melee weapon resolve this branch.
	if (currentWeapon.isMelee)
	{
		Attack(*currentTarget, diceRoller);
	}
	// If NPC uses ranged weapon instead, do that:
	else
	{
		// Well, shoot stuff!
		Attack(*currentTarget, diceRoller);
	}

	return;
}



void GameMaster::AddCharacterToTeam(Character c, int teamToSet)
{
	if (teamToSet == 1 || teamToSet == 2)
	{
		switch (teamToSet)
		{
		case 1:
			team1.push_back(c);
			break;
		case 2:
			team2.push_back(c);
			break;
		}
		c.SetTeam(teamToSet);
	}
	return;
}

void GameMaster::CalculateInitiative()
{
	std::sort(charactersInPlay.begin(), charactersInPlay.end(),
		[](const auto& x, const auto& y) -> bool
		{
			return x.basicSpeed > y.basicSpeed;
		});
}

void GameMaster::KillCharacter(Character character)
{
	// Don't kill character that's not dead! :((
	if (!character.isDead)
		return;

	int charID = character.ID;

	// Search charactersInPlay vector in order to delete character.
	auto characterToDelete = std::find_if(charactersInPlay.cbegin(), charactersInPlay.cend(),
							[charID](const auto &c) -> bool {return c.ID == charID; });
	if (characterToDelete == charactersInPlay.cend())
		return;

	charactersInPlay.erase(characterToDelete);

	// Search adequate team vector in order to delete character.
	switch (character.GetTeam())
	{
		// If team 1
	case 1:
		characterToDelete = std::find_if(team1.cbegin(), team1.cend(),
			[charID](const auto &c) -> bool {return c.ID == charID; });
		if (characterToDelete == team1.cend())
			return;
		team1.erase(characterToDelete);
		break;
		// If team 2
	case 2:
		characterToDelete = std::find_if(team2.cbegin(), team2.cend(),
			[charID](const auto &c) -> bool { return c.ID == charID; });
		if (characterToDelete == team2.cend())
			return;
		team2.erase(characterToDelete);
		break;
		// Shouldn't happen!
	default:
		break;
	}
}

void GameMaster::NextTurn()
{
	for (auto i : charactersInPlay)
	{
		if (i.isDead)
			KillCharacter(i);
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

void GameMaster::AddCharacterToMainVector(Character character)
{
	int IDToFind = character.ID;

	// Check whether character is placed in such vector, if not, add them.
	if (std::find_if(charactersInPlay.cbegin(), charactersInPlay.cend(),
		[IDToFind](const auto &c) -> bool {return c.ID == IDToFind; }) != charactersInPlay.cend())
		return;
	else
		charactersInPlay.push_back(character);
}

void GameMaster::InitializeGameMaster()
{
	// Init data

	LoadSkills();
	LoadArmours();
	LoadShields();
	LoadWeapons();
	LoadCharacters();
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

int GameMaster::LoadCharacters()
{
	std::ifstream ifs("characters.json");
	json j = json::parse(ifs);
	
	json charactersArray = j["characters"];


	for (auto& it : charactersArray)
	{
		int newST = it["strength"];
		int newDX = it["dexterity"];
		int newHT = it["health"];
		std::string newName = it["name"];
		std::string newWeap = it["weapon"];
		std::string newArm = it["armour"];
		std::string newShld = it["shield"];


		// Checks if character is using items loaded previously. If not, it won't be added.

		// Weapon
		std::string nameToSearch = newWeap;

		auto searchedWeapon = std::find_if(allWeapons.cbegin(), allWeapons.cend(),
			[nameToSearch](const Weapon& w) -> bool {return w.name == nameToSearch; });

		if (searchedWeapon == allWeapons.cend())
			continue;
		Weapon newWeapon = *searchedWeapon;

		// Armour
		nameToSearch = newArm;

		auto searchedArmour = std::find_if(allArmours.cbegin(), allArmours.cend(),
			[nameToSearch](const Armour& a) -> bool {return a.name == nameToSearch; });

		if (searchedArmour == allArmours.cend())
			continue;
		Armour newArmour = *searchedArmour;
		
		// Shield
		nameToSearch = newShld;

		auto searchedShield = std::find_if(allShields.cbegin(), allShields.cend(),
			[nameToSearch](const Shield& s) -> bool {return s.name == nameToSearch; });

		if (searchedShield == allShields.cend())
			continue;
		Shield newShield = *searchedShield;

		Character newCharacter;
		newCharacter.InitializeCharacter(newST, newDX, newHT, newName, allSkills, newWeapon, newArmour, newShield);

		allCharacters.push_back(newCharacter);
	}
	return 0;
}

int GameMaster::LoadSkills()
{
	std::ifstream ifs("skills.json");
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
			Skill newSkill = *foundSkill;

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

std::vector<Character> GameMaster::getCharacters()
{
	return allCharacters;
}
std::vector<Skill> GameMaster::getSkills()
{
	return allSkills;
}
std::vector<Armour> GameMaster::getArmours()
{
	return allArmours;
}
std::vector<Weapon> GameMaster::getWeapons()
{
	return allWeapons;
}
std::vector<Shield> GameMaster::getShields()
{
	return allShields;
}

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
