#pragma once

#include <vector>
#include <queue>


class Skill
{
	// Alters chance of succees of using skill.
	// Max: 18.
	int proficiency;
	// Skill's name.
	std::string name;

	// *** DEFAULTS ***
	// Some skills have their default value
	// depending on characters main attributes.
	
	// Value to be added/subtracted to/from proficency.
	int defaultBonus;
	// Attribute that will depend of default proficency.
	std::string defaultAttribute;
	// Optional attribute, if higher that 'defaultAttribute'
	// then this one will be treated as default one.
	std::string defaultOptionalAttribute;
	// *** DEFAULTS ***

	// Set true if skills doesn't have default value.
	bool noDefaults;
};

// The harder the skill, the more player has to pay to upgrade them.
enum SKILLDIFFICULTY
{
	SD_VERYHARD,
	SD_HARD,
	SD_MEDIUM,
	SD_EASY
};

// Directions character can move by on the map.
enum DIRECTION 
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};

// Decision NPCs take during combat
enum AI
{
	// NPC will focus strongest enemies in fight.
	AI_TARGET_STRONGEST,
	// NPC will treat weakest opponent as primary target.
	AI_TARGET_WEAKEST,
	// NPC will randomly choose his target.
	AI_TARGET_RANDOM
};

// GURPS system uses dices to apply damage, so it uses
// 'dices' amount of 6-sided dices + 'bonus'.
typedef struct
{
	int dices;
	int bonus;
}Damage;

class Weapon
{
	// Weapon's name.
	std::string name;
	// Weapon's damage.
	Damage damage;
	// Skill proficiency used by weapon.
	Skill skill;
	// Decides whether weapon has to use ST-related bonus damage.
	bool isMelee;
};

class Character
{
protected:

	// Name character is represented with.
	std::string Name;

	// An array of skills character's using.
	std::vector<Skill> skills;

	// *** Base Attributes ***

	// IQ not needed, magic spells not planned as for now.

	// Correlated to melee attacks damage.
	int strength;
	// Correlated to movement and chances for dodging an attack.
	int dexterity;
	// If you get 0 or less, you're done. 
	int health;

	// ** Extra Attributes ***

	// Used for calculating initiative in combat.
	float basicSpeed;
	// Attribute deciding how far a character can go.
	int move;
	
	// ** Active Defenses **

	// Attribute deciding character succeed in doding an attack.
	int dodge;
	// Attribute deciding if character manages to parry an attack.
	int parry;
	// Same as above, but for blocking.
	int block;

	// ** Passive Defence **

	// A sum of defences given by armour and shield used by a character.
	int armour;
	// A value which decreases the amount of damage taken.
	int damageResistance;

	// ** Status Effects **

	// Tells whether character skips it's turn due to knockdown.
	bool isKnockedDown;
	// Whether the character is not alive.
	bool isDead;
	
	// Base damage using melee weapon, related to ST.
	Damage baseMeleeDamage;

public:
	// Currently used weapon of mass destruction.
	Weapon currentWeapon;

	// Taken the character, try to attack him/her/whatever-the-hell-it-is
	void Attack(Character target);

	// Called usually when there's Attack method called on receiving character
	// calculate attackers skills vs defenders speed, dodging and defence
	void AnswerToAttack(Character attacker);

	// If character happens not to defend himself, he'll get reduced HT.
	void ReceiveDamage(int);

	// Tries to move character in 'dir' direction by 'meters' m,
	// returns string with adequate message.
	std::string Move(DIRECTION dir, int meters);

	// Check the values of extra attributes depending on base Attributes
	// calculate values like movement, passive/active defenses
	// strength attribute is taken for calculating baseMeleeDamage.
	void CalculateExtraAttributes();

	Character();
	virtual ~Character();
};

class Player : public Character 
{
private:
	
	// Used for upgrading skills and attributes.
	int characterPoints;
public:

};

class NPC : public Character
{
private:

	// Is set during NPC creation
	AI usedAI;

public:

	// NPC tries to decide it's next move depending on their situation.
	void AssessSituation();
};

class GameMaster
{
private:
	std::vector<Character> charactersInPlay;
public:
	// Takes a vector of characters and sorts it by initiative.
	void CalculateInitiative();

	// "Rolls" 6-sided dice 'dices' times and adds 'bonus'[if it exists].
	int RollDice(int dices, int bonus);

	GameMaster();
	~GameMaster();

};
