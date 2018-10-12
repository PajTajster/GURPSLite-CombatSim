#include "SimLogic.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>



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

Skill::Skill() : name("Undefined") { }

Skill::Skill(std::string nm, std::string dftAt, std::string dftOptAt,
			int dB, bool noDef) :
			name(nm), defaultAttribute(dftAt), defaultOptionalAttribute(dftOptAt),
			defaultBonus(dB), noDefaults(noDef) { }			

std::string Character::Attack(Character target, DiceRoller dr)
{
	// If target happens to be on the same team, abort.
	if (target.team == team)
		return "You can't attack your allies!";

	Position attackerPos = position;
	Position defenderPos = target.position;

	// If attacker's using melee weapon, check for range.
	if (currentWeapon.isMelee)
	{
		if ((defenderPos.x + 1 == attackerPos.x
				&& defenderPos.y == attackerPos.y)
			|| (defenderPos.x - 1 == attackerPos.x
				&& defenderPos.y == attackerPos.y)
			|| (defenderPos.x == attackerPos.x
				&& defenderPos.y + 1 == attackerPos.y)
			|| (defenderPos.x == attackerPos.x
				&& defenderPos.y - 1 == attackerPos.y))
		{
			std::string message = "Attempting to attack: ";
			message.append(name + " ...");

			// Roll for attack.
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
				damageApplied *= 2;
				target.ReceiveDamage(damageApplied);

				message.append("Critically hit for ");
				message.append(damageApplied + "!");
			}
			// Normal hit, needs to roll for defend 
			else if (roll < currentWeapon.skill.proficiency)
			{
				// If yes, apply damage to target
				if (DidGetHit(target, dr))
				{
					int damageApplied = dr.RollDice(currentWeapon.damage.dices + baseMeleeDamage.dices,
						currentWeapon.damage.bonus + baseMeleeDamage.bonus);
					target.ReceiveDamage(damageApplied);

					message.append("Target hit for ");
					message.append(damageApplied + "!");
				}
				// If no, then attack is blocked and round goes on.
				else
				{
					message.append("Attack blocked!");
				}
			}
			else
				message.append("Miss!");
				
			// Since character attacked already then we set bool to false.
			hasAttackedThisTurn = false;
			return message;
		}
		else
		{
			return "You're out of range!";
		}
	}

	// TODO: Ranged
	

	return "";
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
	else if (diceRoll < 17 && diceRoll > 4)
		return false;

	// Shouldn't happen
	return true;
}

void Character::ReceiveDamage(int damage)
{
	int finalDamage = damage - damageResistance;

	if (finalDamage < 0)
		return;

	// If character got damaged for 1/2 of it's HP, they get knocked down.
	if (health / 2 == finalDamage)
	{
		isKnockedDown = true;
		knockDownTimer++;
	}


	// R.I.P.
	if (health <= 0)
		isDead = true;
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
	if (currentWeapon.name == "Bare Hands")
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
	move = static_cast<int>(basicSpeed);
	dodge = move;
	// For keeping a record how many times character moved their turn.
	movingActions = move;
	
	// If there is armour on character's body, then he "inherits" it's bonuses
	// if not, it's 0.
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
}

float Character::GetInitiative() { return basicSpeed; }

Character::Character() : isWieldingShield(false), isDead(false),
						isKnockedDown(false), hasAttackedThisTurn(false),
						knockDownTimer(0), strength(10), dexterity(10),
						health(10), movingActions(2)
{
	ID = ++nextID;
	skills =
	{
		// Shield
		Skill("Shield", "DX", "None", -6, true),
		// Sword
		Skill("Sword", "DX", "None", -6, true),
		// Axe/Mace
		Skill("Axe/Mace", "DX", "None", -5, true),
		// Polearm
		Skill("Polearm", "DX", "None", -6, true),
		// Bow
		Skill("Bow", "DX", "None", -6, true),
		// Crossbow
		Skill("Crossbow", "DX", "None", -4, true),
		// Pistol
		Skill("Pistol", "DX", "None", -4, true),
		// Rifle
		Skill("Rifle", "DX", "None", -4, true),
	};
}

int Character::nextID = 0;

void Character::SetTeam(int teamToSet)
{
	if (teamToSet == 1 || teamToSet == 2)
		team = teamToSet;
}

int Character::GetTeam() { return team; }

Character& Character::operator=(const Character& original)
{
	ID = original.ID;
	return (*this);
}

Character::Character(const Character& original)
{
	ID = original.ID;
}

Character::~Character()
{
	skills.clear();
}

void Player::ModifyAttribute(int value, char attribute)
{
	if (value * 15 > characterPoints)
		return;

	switch (attribute)
	{
	case 'S':
		strength += value;
		characterPoints -= value * 10;
		break;
	case 'D':
		dexterity += value;
		characterPoints -= value * 10;
		break;
	case 'H':
		health += value;
		characterPoints -= value * 10;
		break;
	default:
		break;
	}

	return;
}

void NPC::AssessSituation()
{
	// TODO
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
	// Don't kill charater that's not dead! :((
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
			i.movingActions = i.move;
		}

	}
}

std::string GameMaster::MoveCharacter(Character c, DIRECTION dir)
{
	if (c.isKnockedDown)
		return "Character knocked down!";


	switch (dir)
	{
		case DIR_UP:
			if (c.position.y + 1 <= 1)
				return "You can't go there!";
			else
			{
				++c.position.y;
				return "You've moved up 1 meter.";
			}
		case DIR_LEFT:
			if(c.position.x - 1 <= 1)
				return "You can't go there!";
			else
			{
				--c.position.x;
				return "You've moved left 1 meter.";
			}
		case DIR_RIGHT:
			if (c.position.x + 1 >= 10)
				return "You can't go there!";
			else
			{
				++c.position.x;
				return "You've moved right 1 meter.";
			}
		case DIR_DOWN:
			if (c.position.y - 1 >= 10)
				return "You can't go there!";
			else
			{
				--c.position.y;
				return "You've moved down 1 meter.";
			}
		// Shouldn't happen
		default:
			break;
	}

	return "";
}

void GameMaster::AddCharacterToMainVector(Character character)
{
	int IDToFind = character.ID;

	// Check whether character is placed in such vector, if not, add them.
	if (std::find_if(charactersInPlay.cbegin(), charactersInPlay.cend(),
		[IDToFind](const auto &c) -> bool {return c.ID == IDToFind; }) == charactersInPlay.cend())
		return;
	else
		charactersInPlay.push_back(character);
}

bool GameMaster::InitializeGameMaster()
{
	// TODO
	return true;
}

bool GameMaster::LoadCharacters()
{
	// TODO
	return true;
}

bool GameMaster::LoadSkills()
{
	// TODO
	return true;
}

bool GameMaster::LoadArmours()
{
	// TODO
	return true;
}

bool GameMaster::LoadWeapons()
{
	// TODO
	return true;
}

GameMaster::GameMaster() { }

GameMaster::~GameMaster()
{
	delete map;
	allCharacters.clear();
	allSkills.clear();
	allArmours.clear();
	allWeapons.clear();

	charactersInPlay.clear();
	team1.clear();
	team2.clear();
}