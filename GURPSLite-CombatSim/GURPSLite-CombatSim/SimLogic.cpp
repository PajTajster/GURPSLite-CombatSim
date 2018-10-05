#include "SimLogic.h"



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
				if (DidGetHit(*this))
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

bool Character::DidGetHit(Character attacker)
{
	return true;
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

}

Character::Character() : actions(2)
{

}

Character::~Character()
{
	skills.clear();
}
