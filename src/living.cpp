#include <iostream>
#include <string>
#include "living.h"
#include "product.h"
#include "hero_products.h"

using namespace std;

Living::Living(string name,int level,int healthPower)
:name(name),level(level),healthPower(healthPower)
{
    curHP=healthPower;
}

Living::~Living(){}

string Living::GetName() const
{
    return name;
}

int Living::GetLevel() const
{
    return level;
}

int Living::GetHP() const
{
    return healthPower;
}

bool Living::IsAlive() const
{
    if (curHP>0)
        return true;
    return false;
}

void Living::InflictedDamage(int N)
{
    if (curHP>N)
        curHP-=N;
    else
        curHP=0;
    return;
}

void Living::RegainHP(int N)
{
    curHP+=N*healthPower/100.0;
    if (curHP>healthPower)
        curHP=healthPower;
    return;
}

Hero::Hero(string name,int level,int healthPower, int magicPower,
     int BaseStrength, int BaseDexterity, int BaseAgility,
     int money, int experience)
:Living(name,level,healthPower), magicPower(magicPower),
BaseStrength(BaseStrength),BaseDexterity(BaseDexterity),BaseAgility(BaseAgility),
money(money),experience(experience)
{
    curMP=magicPower;
    strength=level*BaseStrength;
    dexterity=level*BaseDexterity;
    agility=level*BaseAgility;

    HWeapons=new HeroWeapons(this);
    HArmors=new HeroArmors(this);
    HPotions=new HeroPotions(this);
    HSpells=new HeroSpells(this);
}

Hero::~Hero()
{
    delete HWeapons;
    delete HArmors;
    delete HPotions;
    delete HSpells;
}

int Hero::GetMP() const
{
    return curMP;
}

int Hero::GetStrength() const
{
    return strength;
}

int Hero::GetDexterity() const
{
    return dexterity;
}

int Hero::GetAgility() const
{
    return agility;
}

int Hero::GetMoney() const
{
    return money;
}

int Hero::GetExperience() const
{
    return experience;
}

void Hero::RegainMP(int N)
{
    curMP+=N*magicPower/100.0;
    if (curMP>magicPower)
        curMP=magicPower;
    return;
}

void Hero::ReduceMP(int N)
{
    if (curMP>N)
        curMP-=N;
    else
        curMP=0;
    return;
}

void Hero::GainMoney(int N)
{
    if (N>=0 || money>-N)
        money+=N;
    else
        money=0;
    return;
}

void Hero::LoseMoneyPercent(int N)
{
    money*=(1-N/100.0);
    return;
}

void Hero::GainExperience(int N)
{
    experience+=N;
    return;
}

void Hero::levelUp()
{
    level++;
    CalculateStats();
    return;
}

HeroWeapons *Hero::GetHWeaponsP() const
{
    return HWeapons;
}

HeroArmors *Hero::GetHArmorsP() const
{
    return HArmors;
}

HeroPotions *Hero::GetHPotionsP() const
{
    return HPotions;
}

HeroSpells *Hero::GetHSpellsP() const
{
    return HSpells;
}

void Hero::PrintStats() const
{
    cout<<"Level: "<<level<<endl;
    
    cout<<"Health Power: "<<curHP<<"/"<<healthPower<<endl;
    cout<<"Magic Power: "<<curMP<<"/"<<magicPower<<endl;

    cout<<"Strength: "<<strength<< ", Dexterity: "<<
    dexterity<< ", Agility: "<<agility<<endl;

    cout<<"Money: "<<money<<", Experience: "<<experience<<endl;

    return;
}



Warrior::Warrior(string name,int level,int healthPower, int magicPower,
                 int BaseStrength, int BaseDexterity, int BaseAgility,
                 int money, int experience):
Hero(name,level,healthPower,magicPower,
     BaseStrength, BaseDexterity, BaseAgility,
     money,experience)
{
    CalculateStats();
}

int Warrior::StrengthPercentIncrease=10;
int Warrior::AgilityPercentIncrease=10;

void Warrior::PrintStats() const
{
    cout<<"Hero "<<name<<" is a Warrior and his stats are:"<<endl;
    Hero::PrintStats();
    return;
}

void Warrior::CalculateStats()
{
    double s,d,a;
    
    s=level*BaseStrength*(1+StrengthPercentIncrease/100.0);
    d=level*BaseDexterity;
    a=level*BaseAgility*(1+AgilityPercentIncrease/100.0);
    
    HPotions->ApplyPotions(s,d,a);
    
    //Round stats and keep in mind that max is 1000
    strength=(int) (s+0.5);
    strength=((strength>1000) ? 1000 : strength);
    dexterity=(int) (d+0.5);
    dexterity=((dexterity>1000) ? 1000 : dexterity);
    agility=(int) (a+0.5);
    agility=((agility>1000) ? 1000 : agility);
    return;
}

void Warrior::SetStrAndAgiPercentIncrease(int StrPerInc, int AgiPerInc)
{
    StrengthPercentIncrease=StrPerInc;
    AgilityPercentIncrease=AgiPerInc;
}

Sorcerer::Sorcerer(string name,int level,int healthPower, int magicPower,
                   int BaseStrength, int BaseDexterity, int BaseAgility,
                   int money, int experience):
Hero(name,level,healthPower,magicPower,
     BaseStrength, BaseDexterity, BaseAgility,
     money,experience)
{
    CalculateStats();
}

int Sorcerer::DexterityPercentIncrease=10;
int Sorcerer::AgilityPercentIncrease=10;

void Sorcerer::SetDexAndAgiPercentIncrease(int DexPerInc, int AgiPerInc)
{
    DexterityPercentIncrease=DexPerInc;
    AgilityPercentIncrease=AgiPerInc;
}

void Sorcerer::CalculateStats()
{
    double s,d,a;
    
    s=level*BaseStrength;
    d=level*BaseDexterity*(1+DexterityPercentIncrease/100.0);
    a=level*BaseAgility*(1+AgilityPercentIncrease/100.0);
    
    HPotions->ApplyPotions(s,d,a);
    
    //Round stats and keep in mind that max is 1000
    strength=(int) (s+0.5);
    strength=((strength>1000) ? 1000 : strength);
    dexterity=(int) (d+0.5);
    dexterity=((dexterity>1000) ? 1000 : dexterity);
    agility=(int) (a+0.5);
    agility=((agility>1000) ? 1000 : agility);
    return;
}

void Sorcerer::PrintStats() const
{
    cout<<"Hero "<<name<<" is a Sorcerer and his stats are:"<<endl;
    Hero::PrintStats();
    return;
}

Paladin::Paladin(string name,int level,int healthPower, int magicPower,
                 int BaseStrength, int BaseDexterity, int BaseAgility,
                 int money, int experience):
Hero(name,level,healthPower,magicPower,
     BaseStrength, BaseDexterity, BaseAgility,
     money,experience)
{
    CalculateStats();
}

int Paladin::StrengthPercentIncrease=10;
int Paladin::DexterityPercentIncrease=10;

void Paladin::SetStrAndDexPercentIncrease(int StrPerInc, int DexPerInc)
{
    StrengthPercentIncrease=StrPerInc;
    DexterityPercentIncrease=DexPerInc;
}

void Paladin::CalculateStats()
{
    double s,d,a;
    
    s=level*BaseStrength*(1+StrengthPercentIncrease/100.0);
    d=level*BaseDexterity*(1+DexterityPercentIncrease/100.0);
    a=level*BaseAgility;
    
    HPotions->ApplyPotions(s,d,a);
    
    //Round stats and keep in mind that max is 1000
    strength=(int) (s+0.5);
    strength=((strength>1000) ? 1000 : strength);
    dexterity=(int) (d+0.5);
    dexterity=((dexterity>1000) ? 1000 : dexterity);
    agility=(int) (a+0.5);
    agility=((agility>1000) ? 1000 : agility);
    return;
}


void Paladin::PrintStats() const
{
    cout<<"Hero "<<name<<" is a Paladin and his stats are:"<<endl;
    Hero::PrintStats();
    return;
}

Monster::Monster(string name,int level,int healthPower, int BaseMinDamage,
                 int BaseMaxDamage,int BaseDefence, int BaseAvoidance)
:Living(name,level,healthPower), BaseMinDamage(BaseMinDamage),
BaseMaxDamage(BaseMaxDamage), BaseDefence(BaseDefence), BaseAvoidance(BaseAvoidance)
{
    min_damage=BaseMinDamage;
    max_damage=BaseMaxDamage;
    defence=BaseDefence;
    prob_missing=BaseAvoidance;
}

Monster::~Monster(){}

int Monster::GetMinDamage() const
{
    return min_damage;
}

int Monster::GetMaxDamage() const
{
    return max_damage;
}

int Monster::GetDefence() const
{
    return defence;
}

int Monster::GetProbMissing() const
{
    return prob_missing;
}

void Monster::ReduceDamageRangePercent(int N)
{
    min_damage*=1-N/100.0;
    max_damage*=1-N/100.0;
    return;
}

void Monster::ReduceDefencePercent(int N)
{
    defence*=1-N/100.0;
    return;
}

void Monster::ReduceProbMissingPercent(int N)
{
    prob_missing*=1-N/100.0;
    return;
}

void Monster::PrintStats() const
{
    cout<<"Level: "<<level<<endl;
    cout<<"Health Power: "<<curHP<<"/"<<healthPower<<endl;

    cout<<"Damage Range: "<<min_damage<<"-"<<max_damage<<endl;
    cout<<"Defence: "<<defence<<endl;
    cout<<"Avoidance: "<<prob_missing<< ", so probability to avoid attack is: "<<prob_missing/20.0<<"%"<<endl;

    return;
}

Dragon::Dragon(string name,int level,int healthPower, int BaseMinDamage,
               int BaseMaxDamage,int BaseDefence, int BaseAvoidance)
:Monster(name,level,healthPower,BaseMinDamage,BaseMaxDamage,BaseDefence,BaseAvoidance)
{
    CalculateStats();
}

int Dragon::DamagePercentIncrease=10;

void Dragon::SetDamagePercentIncrease(int DamPerInc)
{
    DamagePercentIncrease=DamPerInc;
    return;
}

void Dragon::CalculateStats()
{
    min_damage=(int) (level*BaseMinDamage*(1+DamagePercentIncrease/100.0)+0.5);
    min_damage=((min_damage>1000) ? 1000 : min_damage);
    max_damage=(int) (level*BaseMaxDamage*(1+DamagePercentIncrease/100.0)+0.5);
    max_damage=((max_damage>1000) ? 1000 : max_damage);
    defence=level*BaseDefence;
    prob_missing=level*BaseAvoidance;
    return;
}

void Dragon::PrintStats() const
{
    cout<<"Monster "<<name<<" is a Dragon and its stats are:"<<endl;
    Monster::PrintStats();
    return;
}

Exoskeleton::Exoskeleton(string name,int level,int healthPower, int BaseMinDamage,
                         int BaseMaxDamage,int BaseDefence, int BaseAvoidance)
:Monster(name,level,healthPower,BaseMinDamage,BaseMaxDamage,BaseDefence,BaseAvoidance)
{
    CalculateStats();
}

int Exoskeleton::DefencePercentIncrease=10;

void Exoskeleton::SetDefencePercentIncrease(int DefPerInc)
{
    DefencePercentIncrease=DefPerInc;
    return;
}

void Exoskeleton::CalculateStats()
{
    min_damage=level*BaseMinDamage;
    max_damage=level*BaseMaxDamage;
    defence=(int) (level*BaseDefence*(1+DefencePercentIncrease/100.0)+0.5);
    defence=((defence>1000) ? 1000 : defence);
    prob_missing=level*BaseAvoidance;
    return;
}

void Exoskeleton::PrintStats() const
{
    cout<<"Monster "<<name<<" is a Exoskeleton and its stats are:"<<endl;
    Monster::PrintStats();
    return;
}

Spirit::Spirit(string name,int level,int healthPower, int BaseMinDamage,
               int BaseMaxDamage,int BaseDefence, int BaseAvoidance)
:Monster(name,level,healthPower,BaseMinDamage,BaseMaxDamage,BaseDefence,BaseAvoidance)
{
    CalculateStats();
}

int Spirit::AvoidancePercentIncrease=10;

void Spirit::SetAvoidancePercentIncrease(int AvdPerInc)
{
    AvoidancePercentIncrease=AvdPerInc;
    return;
}

void Spirit::CalculateStats()
{
    min_damage=level*BaseMinDamage;
    max_damage=level*BaseMaxDamage;
    defence=level*BaseDefence;
    prob_missing=(int) (level*BaseAvoidance*(1+AvoidancePercentIncrease/100.0)+0.5);
    prob_missing=((prob_missing>1000) ? 1000 : prob_missing);
    return;
}

void Spirit::PrintStats() const
{
    cout<<"Monster "<<name<<" is a Spirit and its stats are:"<<endl;
    Monster::PrintStats();
    return;
}
