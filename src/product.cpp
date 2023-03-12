#include <iostream>
#include "product.h"
#include "living.h"

using namespace std;

Product::Product(string name, int price, int min_lvl)
:name(name), price(price), min_lvl(min_lvl)
{}

Product::~Product(){}

string Product::GetName() const
{
    return name;
}

int Product::GetPrice() const
{
    return price;
}

int Product::GetLvlReq() const
{
    return min_lvl;
}

void Product::DisplayInfo() const
{
    cout<<"Level requirement for hero in order to use it:"<<min_lvl<<endl;
    cout<<"Price:"<<price<<endl;
    return;
}

Item::Item(string name, int price, int min_lvl)
:Product(name, price, min_lvl)
{}

Item::~Item(){}

Weapon::Weapon(string name, int price, int min_lvl, int damage, Hands n)
:Item(name, price, min_lvl), damage(damage), hands(n)
{}

int Weapon::GetDamage() const
{
    return damage;
}

Weapon::Hands Weapon::GetNumberOfHands() const
{
    return hands;
}

void Weapon::DisplayInfo() const
{
    Product::DisplayInfo();
    cout<<"It requires "<<
    ((Weapon::hands==one_hand) ? "one hand " : "both hands ")<<
    "to use it."<<endl;
    cout<<"It inflicts "<<damage<<" damage."<<endl;
    return;
}

Armor::Armor(string name, int price, int min_lvl, int damage_reduction)
:Item(name, price, min_lvl), damage_reduction(damage_reduction)
{}

int Armor::GetDamageReduction() const
{
    return damage_reduction;
}

void Armor::DisplayInfo() const
{
    Product::DisplayInfo();
    cout<<"It reduces the damage received by "<<damage_reduction<<"%."<<endl;
    return;
}

Potion::Potion(string name, int price, int min_lvl, Stat stat, int increase)
:Item(name,price,min_lvl),stat_increased(stat),increase(increase)
{}

Potion::Stat Potion::GetStatIncreased() const
{
    return stat_increased;
}

int Potion::GetIncrease() const
{
    return increase;
}

void Potion::DisplayInfo() const
{
    Product::DisplayInfo();
    cout<<"It increaces the ";
    switch (stat_increased){
        case strength:
            cout<<"strength";
            break;
        case dexterity:
            cout<<"dexterity";
            break;
        case agility:
            cout<<"agility";
            break;
    }
    cout<<" of hero by "<<increase<<"."<<endl;
    return;
}

Spell::Spell(string name, int price, int min_lvl,
             int min_damage,int max_damage,int energy_req)
:Product(name, price, min_lvl),
min_damage(min_damage),max_damage(max_damage), energy_req(energy_req)
{}

Spell::~Spell(){}

int Spell::GetEnergyReq() const
{
    return energy_req;
}

int Spell::GetMinDamage() const
{
    return min_damage;
}

int Spell::GetMaxDamage() const
{
    return max_damage;
}

void Spell::DisplayInfo() const
{
    Product::DisplayInfo();
    cout<<"The damage range that inflicts is "<<min_damage<<"-"<<
    max_damage<<"."<<endl;
    cout<<"It requires "<<energy_req<<" Magic Power from hero to use it."<<endl;
    return;
}

IceSpell::IceSpell(string name, int price, int min_lvl,
                   int min_damage,int max_damage,int energy_req,
                   int damage_reduction)
:Spell(name, price, min_lvl,min_damage,max_damage,energy_req),
damage_reduction(damage_reduction)
{}

Spell *IceSpell::Clone() const
{
    Spell *S=new IceSpell(*this);
    return S;
}

void IceSpell::ApplyEffect(Monster *M)
{
    M->ReduceDamageRangePercent(damage_reduction);
    return;
}

void IceSpell::DisplayInfo() const
{
    Spell::DisplayInfo();
    cout<<"It reduces the range of damage of the inflicted monster by "<<
    damage_reduction<<"%."<<endl;
    return;
}

FireSpell::FireSpell(string name, int price, int min_lvl,
                   int min_damage,int max_damage,int energy_req,
                   int defence_reduction)
:Spell(name, price, min_lvl,min_damage,max_damage,energy_req),
defence_reduction(defence_reduction)
{}

Spell *FireSpell::Clone() const
{
    Spell *S=new FireSpell(*this);
    return S;
}

void FireSpell::ApplyEffect(Monster *M)
{
    M->ReduceDefencePercent(defence_reduction);
    return;
}

void FireSpell::DisplayInfo() const
{
    Spell::DisplayInfo();
    cout<<"It reduces the defence of the inflicted monster by "<<
    defence_reduction<<"%."<<endl;
    return;
}

LightingSpell::LightingSpell(string name, int price, int min_lvl,
                   int min_damage,int max_damage,int energy_req,
                   int prob_reduction)
:Spell(name, price, min_lvl,min_damage,max_damage,energy_req),
prob_reduction(prob_reduction)
{}

Spell *LightingSpell::Clone() const
{
    Spell *S=new LightingSpell(*this);
    return S;
}

void LightingSpell::ApplyEffect(Monster *M) 
{
    M->ReduceProbMissingPercent(prob_reduction);
    return;
}

void LightingSpell::DisplayInfo() const
{
    Spell::DisplayInfo();
    cout<<"It reduces the probability of the inflicted monster to avoid attack by "<<
    prob_reduction<<"%."<<endl;
    return;
}
