#ifndef FILE_LIVING_H_INCLUDED
#define FILE_LIVING_H_INCLUDED

#include <string>
#include "product.h"


class HeroWeapons;
class HeroArmors;
class HeroPotions;
class HeroSpells;

class Living{
protected:
    const std::string name;
    int level;
    const int healthPower; //healthPower when healthy(full)
    int curHP;//current healthPower
public:
    Living(std::string name,int level,int healthPower);
    virtual ~Living()=0; //Makes living class abstract
    
    std::string GetName() const;
    int GetLevel() const;
    int GetHP() const; //Get current healthPower
    bool IsAlive() const; //Check if a living has fainted
    void InflictedDamage(int N); //Reduce HP
    void RegainHP(int N); //Regain N% of max health power
    virtual void CalculateStats()=0; //Calculate current stats
    virtual void PrintStats() const=0;
};

class Hero:public Living{
protected:
    const int magicPower;//full magicPower
    /*All base stats must have values 0-10*/
    const int BaseStrength; //initial strength at level 1 without items or increases
    const int BaseDexterity;//initial dexterity at level 1 without items or increases
    const int BaseAgility;  //initial agility at level 1 without items or increases

    int curMP; //current magicPower
    /*Stats must have values 0-1000*/
    int strength;//current strength
    int dexterity;//current dexterity
    int agility; //current agility.
    
    int money;
    int experience;

    HeroWeapons *HWeapons; //Weapons of the hero
    HeroArmors *HArmors; //Armors of the hero
    HeroPotions *HPotions; //Potions of the hero
    HeroSpells *HSpells; //Spells of the hero
public:
    Hero(std::string name,int level,int healthPower, int magicPower,
         int BaseStrength, int BaseDexterity, int BaseAgility,
         int money=0, int experience=0);
    virtual ~Hero()=0; //Makes hero class abstract
    
    int GetMP() const; //Get current magic power
    int GetStrength() const;
    int GetDexterity() const;
    int GetAgility() const;
    int GetMoney() const;
    int GetExperience() const;
    void RegainMP(int N); //Regain N% of max magic power
    void ReduceMP(int N); //Lose N points of magic power
    void GainMoney(int N); //Gain N money or lose -N money, if N<0
    void LoseMoneyPercent(int N); //Lose N percent of money
    void GainExperience(int N); //Gain N experience points
    void levelUp();
    HeroWeapons *GetHWeaponsP() const; //Get HWeapons pointer
    HeroArmors *GetHArmorsP() const; //Get HArmors pointer
    HeroPotions *GetHPotionsP() const; //Get HPotions pointer
    HeroSpells *GetHSpellsP() const; //Get HSpells pointer
    virtual void CalculateStats()=0; //Calculate current stats
    virtual void PrintStats() const;
};

class Warrior:public Hero{
private:
    static int StrengthPercentIncrease; /*"StrengthPercentIncrease" percent is the
                                       increase in strength. Default value is 10.*/
    static int AgilityPercentIncrease; /*"AgilityPercentIncrease" percent is the
                                       increase in agility. Default value is 10.*/
public:
    Warrior(std::string name,int level,int healthPower, int magicPower,
            int BaseStrength, int BaseDexterity, int BaseAgility,
            int money=0, int experience=0);
    
    static void SetStrAndAgiPercentIncrease(int StrPerInc, int AgiPerInc);
    
    virtual void CalculateStats();
    virtual void PrintStats() const;
};


class Sorcerer:public Hero{
private:
    static int DexterityPercentIncrease;/*"DexterityPercentIncrease" percent is the
                                        increase in dexterity. Default value is 10.*/
    static int AgilityPercentIncrease;/*"AgilityPercentIncrease" percent is the
                                      increase in agility. Default value is 10.*/
public:
    Sorcerer(std::string name,int level,int healthPower, int magicPower,
         int BaseStrength, int BaseDexterity, int BaseAgility,
         int money=0, int experience=0);
    
    static void SetDexAndAgiPercentIncrease(int DexPerInc, int AgiPerInc);
    
    virtual void CalculateStats();
    virtual void PrintStats() const;
};

class Paladin:public Hero{
private:
    static int StrengthPercentIncrease;/*"StrengthPercentIncrease" percent is the
                                       increase in strength. Default value is 10.*/
    static int DexterityPercentIncrease;/*"DexterityPercentIncrease" percent is the
                                        increase in dexterity. Default value is 10.*/
public:
    Paladin(std::string name,int level,int healthPower, int magicPower,
            int BaseStrength, int BaseDexterity, int BaseAgility,
            int money=0, int experience=0);
    
    static void SetStrAndDexPercentIncrease(int StrPerInc, int DexPerInc);
    
    virtual void CalculateStats();
    virtual void PrintStats() const;
};

class Monster:public Living{
protected:
    /*All base stats must have values 0-10*/
    const int BaseMinDamage;//initial min_damage at level 1 without increases/decreases
    const int BaseMaxDamage;//initial max_damage at level 1 without increases/decreases
    const int BaseDefence;  //initial defence at level 1 without increases/decreases
    const int BaseAvoidance;//initial prob_missing at level 1 without increases/decreases
    /*Stats must have values 0-1000*/
    int min_damage; //Minimum amount of damage that can inflict
    int max_damage; //Maximum amount of damage that can inflict
    int defence;
    int prob_missing; //"prob_missing"/2000 is the probability to avoid attack
public:
    Monster(std::string name,int level,int healthPower, int BaseMinDamage,
            int BaseMaxDamage,int BaseDefence, int BaseAvoidance);
    virtual ~Monster()=0;//Makes monster class abstract
    
    int GetMinDamage() const;
    int GetMaxDamage() const;
    int GetDefence() const;
    int GetProbMissing() const;
    void ReduceDamageRangePercent(int N);//Reduce damage by N percent range
    void ReduceDefencePercent(int N); //Reduce defence by N percent
    void ReduceProbMissingPercent(int N); //Reduce prob_missing by N percent

    virtual void CalculateStats()=0;
    virtual void PrintStats() const;
};

class Dragon:public Monster{
private:
    static int DamagePercentIncrease;/*"DamagePercentIncrease" percent is the increase
                                of the damage range that inflicts. Default value is 10.*/
public:
    Dragon(std::string name,int level,int healthPower, int BaseMinDamage,
           int BaseMaxDamage,int BaseDefence, int BaseAvoidance);
    
    static void SetDamagePercentIncrease(int);
    
    void CalculateStats();
    void PrintStats() const;
};

class Exoskeleton:public Monster{
private:
    static int DefencePercentIncrease;/*"DamagePercentIncrease" percent is the
                                       increase of the defence. Default value is 10.*/
public:
    Exoskeleton(std::string name,int level,int healthPower,int BaseMinDamage,
                int BaseMaxDamage,int BaseDefence, int BaseAvoidance);
    
    static void SetDefencePercentIncrease(int);
    
    void CalculateStats();
    void PrintStats() const;
};

class Spirit:public Monster{
private:
    static int AvoidancePercentIncrease;/*"AvoidancePercentIncrease" percent is the
                                increase of the prob_missing. Default value is 10.*/
public:
    Spirit(std::string name,int level,int healthPower, int BaseMinDamage,
           int BaseMaxDamage,int BaseDefence, int BaseAvoidance);
    
    static void SetAvoidancePercentIncrease(int);
    
    void CalculateStats();
    void PrintStats() const;
};

#endif
