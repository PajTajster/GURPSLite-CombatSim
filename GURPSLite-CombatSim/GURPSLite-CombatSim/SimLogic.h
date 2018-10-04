#pragma once

#include <vector>
#include <queue>
#include <optional>


enum DIRECTION 
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};

class Weapon
{

};

class GameMaster 
{
private:
	std::vector<Character> charactersInPlay;
public:
	// Takes a vector of characters and sorts it by initiative.
	void CalculateInitiative();

	// "Rolls" 6-sided dice 'dices' times and adds 'bonus'.
	int RollDice(int dices, std::optional<int> bonus);

	GameMaster();
	~GameMaster();

};

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
	// calculate values like movement, passive/active defenses.
	void CalculateExtraAttributes();



	Character();
	virtual ~Character();
};