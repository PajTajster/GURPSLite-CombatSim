#pragma once

#include <vector>
#include <queue>

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

class GameMaster
{
public:

	// "Rolls" 6-sided dice 'dices' times and adds 'bonus'[if it exists].
	int RollDice(int dices, int bonus);

	GameMaster();
	~GameMaster();

};

class Skill
{
public:
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

	// Set true if skill doesn't have default value.
	bool noDefaults;

	Skill();
	Skill(std::string nm, std::string dftAt, std::string dftOptAt,
		int dB, bool noDef);
};

class Armour
{
public:
	// Armour's name.
	std::string name;
	// How much bonus to defence roll this armor gives.
	int passiveDefence;
	// How much damage it blocks.
	int damageResistance;
};

class Weapon
{
public:
	// Weapon's name.
	std::string name;
	// Weapon's damage.
	Damage damage;
	// Skill proficiency used by weapon.
	Skill skill;
	// Decides whether weapon has to use ST-related bonus damage. [Ranged or not]
	bool isMelee;
	// How many "bullets" weapon shoots, if it's melee then it's value = 1.
	int rateOfFire;
	// Whether character uses both hands or not[if yes, then no shield possible].
	bool isTwoHanded;
};

typedef struct
{
	int x;
	int y;
}Position;


class Character
{
protected:
	// *** Base Attributes ***

	// IQ not needed, magic spells not planned as for now.

	// Correlated to melee attacks damage.
	int strength;
	// Correlated to movement and chances for dodging an attack.
	int dexterity;
	// If you get 0 or less, you're done. 
	int health;

	// ** Extra Attributes ***


	// Attribute deciding how far a character can go.
	int move;
	
	// ** Active Defenses **

	// Attribute deciding character succeed in doding an attack.
	int dodge;
	// Attribute deciding if character manages to parry an attack
	// related to the skill used by currentWeapon.
	int parry;
	// Attribute deciding character's chance in blocking.
	// Is active only if character has shield.
	int block;

	// ** Passive Defence **

	// A sum of defences given by armour and shield used by a character.
	int passiveDefence;
	// A value which decreases the amount of damage taken.
	int damageResistance;
	
	// Base damage using melee weapon, related to ST.
	Damage baseMeleeDamage;

public:

	// Amount of actions character can performs, which is 2 per turn.
	int actions;

	// Name character is represented with.
	std::string name;

	// An array of skills character's using.
	std::vector<Skill> skills;

	// Where is character located in 2D space.
	Position position;

	// ** Status Effects **

	// Tells whether character skips it's turn due to knockdown.
	bool isKnockedDown;
	// Tells for how many turns characters are supposed to be knocked down
	// when reaches 0, character gets up(if they didn't died already).
	int knockDownTimer;
	// Whether the character is not alive.
	bool isDead;

	// Used for calculating initiative in combat.
	float basicSpeed;

	// Currently used weapon of mass destruction.
	Weapon currentWeapon;
	// Says whether character has shield ready;
	bool isWieldingShield;
	// If Character has a shield, it gives this bonus to "Shield" skill.
	int equippedShieldBonus;
	// Says whethe character is wearing any armour or goes berserk-like nude.
	bool isWearingArmour;
	// Currently worn armour.
	Armour currentArmour;

	// Taken the character, try to attack him/her/whatever-the-hell-it-is
	//	returns string message with adequate message.
	std::string Attack(Character target, GameMaster gm);

	// Called usually when there's Attack method called on receiving character
	// calculate attackers skills vs defenders speed, dodging and defence
	bool DidGetHit(Character attacker, GameMaster gm);

	// If character happens not to defend himself, he'll get reduced HT.
	void ReceiveDamage(int);

	// Tries to move character in 'dir' direction by 'meters' m,
	// returns string with adequate message.
	std::string Move(DIRECTION dir, int meters);

	// Check the values of extra attributes depending on base Attributes
	// calculate values like movement, passive/active defenses
	// strength attribute is taken for calculating baseMeleeDamage.
	void CalculateExtraAttributes();

	// Initiative getter.
	float getInitiative();

	Character();
	virtual ~Character();
};

class Player : public Character 
{
private:
	
	// Used for upgrading skills and attributes.
	int characterPoints;
public:

	// During character creation, for modifying base attributes.
	void ModifyAttribute(int value, char attribute);
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

// Class taking care of turns and match movement.
class TurnLogic
{
private:
	// Characters that fight in present match.
	std::vector<Character> charactersInPlay;

	// Match sides. Set by player before match begin. 

	std::vector<Character> team1;
	std::vector<Character> team2;
public:

	// Takes a vector of characters and sorts it by initiative.
	void CalculateInitiative();

	void KillCharacter(Character);

	void NextTurn();

	TurnLogic();
	~TurnLogic();
};