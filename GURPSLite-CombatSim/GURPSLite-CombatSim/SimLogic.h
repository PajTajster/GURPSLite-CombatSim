#pragma once

#include <vector>
#include <queue>
#include <string>
#include <memory>

// Decision NPCs take during combat
enum AI
{
	// NPC will focus strongest enemies in fight.
	AI_TARGET_STRONGEST,
	// NPC will treat weakest opponent as primary target.
	AI_TARGET_WEAKEST,
	// NPC will randomly choose his target.
	AI_TARGET_RANDOM,
	// Nonexistant AI.
	AI_NULL
};

// GURPS system uses dices to apply damage, so it uses
// 'dices' amount of 6-sided dices + 'bonus'.
typedef struct
{
	int dices;
	int bonus;
}Damage;

class DiceRoller
{
public:

	// "Rolls" 6-sided dice 'dices' times and adds 'bonus'[if it exists].
	int RollDice(int dices, int bonus);

	DiceRoller();
	~DiceRoller();

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

	std::vector<std::string> PrintSkill();

	Skill();
	Skill(std::string nm, std::string dftAt, std::string dftOptAt,
		int dB, bool noDef);
};

class Shield {
public:
	std::string name;
	int bonus;

	std::vector<std::string> PrintShield();

	Shield();
	Shield(std::string n, int b);
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

	std::vector<std::string> PrintArmour();

	Armour();
	Armour(std::string n, int pD, int dR);
};

class Weapon
{
public:
	// Weapon's name.
	std::string name;
	// Weapon's damage.
	Damage damage;
	// Skill's name, of which proficiency is used by weapon.
	std::string skill;
	// Decides whether weapon has to use ST-related bonus damage. [Ranged or not]
	bool isMelee;
	// How many "bullets" weapon shoots, if it's melee then it's value = 1.
	int rateOfFire;
	// Whether character uses both hands or not[if yes, then no shield possible].
	bool isTwoHanded;

	std::vector<std::string> PrintWeapon();

	Weapon();
	Weapon(std::string n, Damage d, std::string s, bool isM, int rOF, bool isTH);
};

class Character
{
protected:
	DiceRoller diceRoller;

	// Value to keep record of last ID given, this way
	// IDs won't repeat.
	static int nextID;

	// *** Base Attributes ***

	// IQ not needed, magic spells not planned as for now.

	// Correlated to melee attacks damage.
	int strength;
	// Correlated to movement and chances for dodging an attack.
	int dexterity;
	// If you get 0 or less, you're done. 
	int health;
	// Bonus towards all skills proficiencies. [max: 10]
	int veterancy;
	
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

	// What team character belongs to
	// for now only 1 and 2.
	int team;

	// NPC's currently used AI.
	AI usedAI;
	// Checks whether NPC has selected it's prey.
	bool doesNPCHaveTarget;
	// Current character's ID that NPC wants to do something bad.
	int currentTargetIndex;
public:
	// Character's ID for distinguishability.
	int ID;

	// Tells whether character is player or not.
	bool isPlayer;
	// Used for upgrading skills and attributes.
	int characterPoints;

	// Tells whether character have attacked
	bool hasAttackedThisTurn;

	// Name character is represented with.
	std::string name;

	// An array of skills character's using.
	std::vector<Skill> skills;

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
	// Currently used shield.
	Shield currentShield;
	// Says whethe character is wearing any armour or goes berserk-like nude.
	bool isWearingArmour;
	// Currently worn armour.
	Armour currentArmour;


	// Modifies given attribute by value at the cost of characterPoints.
	// Returns true if attribute was succesfully modified, false if not.
	bool ModifyAttribute(int value, char attribute);
	// Returns Character characterPoints;
	int GetCharacterPoints();

	// Selects the target AI will try to kill depending on the 'usedAI'.
	// must be supplied all the possible targets.
	void NPCSelectTarget(std::vector<Character>& charactersToChoose);

	// NPC tries to decide it's next move depending on their situation.
	std::string NPCAssessSituation(std::vector<Character>& charactersToChoose);

	// Taken the character, try to attack him/her/whatever-the-hell-it-is
	//	returns string message with adequate message.
	std::string Attack(Character& targe);

	// Called usually when there's Attack method called on receiving character
	bool DidGetHit();

	// If character happens not to defend himself, he'll get reduced HT.
	// returns damage taken.
	int ReceiveDamage(int);

	// Initialize character[mainly used for loading from file].
	void InitializeCharacter(int initST, int initDX, int initHT, int initVet,
		std::string initName, std::vector<Skill> initSkills, Weapon initWeapon,
		Armour initArmour, Shield initShield);

	// Check the values of extra attributes depending on base Attributes
	// calculate values like movement, passive/active defenses
	// strength attribute is taken for calculating baseMeleeDamage.
	void CalculateExtraAttributes();

	// Update all the character's skills proficiencies.
	void CalculateSkillsDefaults();

	// Get current HT.
	int GetHealth();

	// Initiative getter.
	float GetInitiative();

	void SetTeam(int teamToSet);
	int GetTeam();

	std::vector<std::string> PrintCharacter();


	//Character& operator=(const Character& original);

	Character();
	Character(const Character&);
	Character(const Character&, bool);

	~Character();
};

class GameMaster
{
private:
	static DiceRoller diceRoller;

	// Names that will be appended to characters.
	// [just to skip fighting against Bandit, and Bandit and Bandit]
	std::vector<std::string> names;

	// Vectors containing all the data app needs.

	// All the characters available to choose.
	std::vector<Character> allCharacters;
	// All the skills that characters can learn.
	std::vector<Skill> allSkills;
	// All the armours that characters can use.
	std::vector<Armour> allArmours;
	// All the weapons that characters can make a mess with.
	std::vector<Weapon> allWeapons;
	// All the shields characters can use.
	std::vector<Shield> allShields;
	// Characters that fight in present match.
	std::vector<Character> charactersInPlay;

	// Match sides. Set by player before match begin. 

	std::vector<Character> team1;
	std::vector<Character> team2;

	int LoadCharacters();
	int LoadSkills();
	int LoadArmours();
	int LoadWeapons();
	int LoadShields();
	void LoadNames();
public:


	// Takes a vector of 'charactersInPlay' and sorts it by initiative.
	void CalculateInitiative();

	// Run through all the present characters and refresh their
	// actions and decrement knockdowntimer (if they have one active).
	void NextTurn(std::vector<Character>& updatedVector);

	// Clears team and charactersInPlay vector.
	void ClearBattleData();
	// Taken 2 vectors of teams, gamemaster applies those character to his data and sets correct teams.
	void PrepareTeams(std::vector<Character> t1, std::vector<Character> t2);
	// Adds given character to adequate team vector (either 1 or 2).
	void AddCharacterToTeam(int id, int teamToSet);
	// Adds character into 'charactersInPlay' vector, then add them to team.
	void AddCharacterToMainVector(Character c, int team);

	// Gives character a random name, result is {name}, the {oldName}.
	void RandomizeName(Character& c);

	// Load all the data in JSON files.
	void InitializeGameMaster();
	// Init base player (returns Character which is Player).
	Character* InitBasePlayer();
	void UpdatePlayer(Character* player, std::vector<Character>& updatedVector);

	std::vector<Character> GetCharacters();
	std::vector<Skill> GetSkills();
	std::vector<Armour> GetArmours();
	std::vector<Weapon> GetWeapons();
	std::vector<Shield> GetShields();
	std::vector<Character>& GetCharactersInPlay();

	GameMaster();
	~GameMaster();
};
