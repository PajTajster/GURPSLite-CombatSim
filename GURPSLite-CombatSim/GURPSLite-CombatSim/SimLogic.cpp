#include "SimLogic.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>

int GameMaster::RollDice(int dices, int bonus)
{
	srand(time(NULL));
	int diceResult = 0;
	for (int i = 0; i < dices; ++i)
		diceResult += rand() % 6 + 1;

	return diceResult + bonus;
}

GameMaster::GameMaster() {}
GameMaster::~GameMaster() {}



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
	
}

void Character::ReceiveDamage(int damage)
{
	
}

std::string Character::Move(DIRECTION dir, int meters)
{

	return "";
}

void Character::CalculateExtraAttributes()
{
	// If no shield, then block is 0
	// if there's one, then it's half of "Shield" skill proficiency.
	if (!isWieldingShield)
		block = 0;
	else
	{
		auto shieldSkill = find(skills.cbegin(), skills.cend(), "Shield");

		if (shieldSkill->name == "Shield")
			block = shieldSkill->proficiency / 2;
	}

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

Character::Character() : actions(2), isWieldingShield(false),
						isDead(false), isKnockedDown(false),
						knockDownTimer(0) { }

Character::~Character()
{
	skills.clear();
}


void Player::ModifyAttribute(int value, char attribute)
{
	
}

void NPC::AssessSituation()
{

}

void TurnLogic::CalculateInitiative()
{
	std::sort(charactersInPlay.cbegin, charactersInPlay.cend,
		[](const Character& x, const Character& y) -> bool
	{
		return 
	});
}

void TurnLogic::KillCharacter()
{

}

void TurnLogic::NextTurn()
{

}

TurnLogic::TurnLogic() { }

TurnLogic::~TurnLogic() { }