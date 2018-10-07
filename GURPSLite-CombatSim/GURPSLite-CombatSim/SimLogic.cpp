#include "SimLogic.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>

int GameMaster::RollDice(int dices, int bonus)
{
	srand((unsigned)time(NULL));
	int diceResult = 0;
	for (int i = 0; i < dices; ++i)
		diceResult += rand() % 6 + 1;

	return diceResult + bonus;
}

GameMaster::GameMaster() {}
GameMaster::~GameMaster() {}

Skill::Skill() { }

Skill::Skill(std::string nm, std::string dftAt, std::string dftOptAt,
			int dB, bool noDef) :
			name(nm), defaultAttribute(dftAt), defaultOptionalAttribute(dftOptAt),
			defaultBonus(dB), noDefaults(noDef) { }			

std::string Character::Attack(Character target, GameMaster gm)
{
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
			message.append(Name + " ...");

			// Roll for attack.
			int roll = gm.RollDice(3, 0);

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
				int damageApplied = gm.RollDice(currentWeapon.damage.dices + baseMeleeDamage.dices,
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
				if (DidGetHit(target, gm))
				{
					int damageApplied = gm.RollDice(currentWeapon.damage.dices + baseMeleeDamage.dices,
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
				
			// Attack is an action, thus we reduce possible actions for this turn.
			--actions;
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

bool Character::DidGetHit(Character attacker, GameMaster gm)
{
	// Firstly calculate our total defence, which is sum of all
	// defences the character has. [if knockdown then only passive]
	int totalDefense = 0;

	if (isKnockedDown)
		totalDefense = passiveDefence;
	else
		totalDefense = (dodge + parry + block + passiveDefence);

	// Now we roll 3d6 against it.
	int diceRoll = gm.RollDice(3, 0);

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
	if (health / 2 == damage)
	{
		isKnockedDown = true;
		knockDownTimer++;
	}
	health -= damage;

	// R.I.P.
	if (health <= 0)
		isDead = true;
}

std::string Character::Move(DIRECTION dir, int meters)
{
	
	return "";
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
		auto weaponSkill = find(skills.cbegin(), skills.cend(), currentWeapon.skill.name);
		parry = weaponSkill->proficiency / 2;
	}

	basicSpeed = static_cast<float>((health + dexterity) / 4);
	move = static_cast<int>(basicSpeed);
	dodge = move;
	
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

float Character::getInitiative() { return basicSpeed; }

Character::Character() : actions(2), isWieldingShield(false),
						isDead(false), isKnockedDown(false),
						knockDownTimer(0)
{
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
		Skill("Rifle", "DX", "None", -4, true)
	};
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

}

void TurnLogic::CalculateInitiative()
{
	std::sort(charactersInPlay.cbegin(), charactersInPlay.cend(),
		[](const Character& x, const Character& y) -> bool
		{
			return x.basicSpeed > y.basicSpeed;
		});
}

void TurnLogic::KillCharacter(Character character)
{
	if (!character.isDead)
		return;

	// wip
	auto characterToDelete = find(charactersInPlay.cbegin(), charactersInPlay.cend(), character.Name);
}

void TurnLogic::NextTurn()
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

	}
}

TurnLogic::TurnLogic() { }

TurnLogic::~TurnLogic() { }