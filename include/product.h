#ifndef FILE_PRODUCT_H_INCLUDED
#define FILE_PRODUCT_H_INCLUDED

#include <string>

class Monster;


class Product{//Things that players can buy(inluding spells, items)
protected:
    const std::string name;
    int price;
    int min_lvl;
public:
    Product(std::string name, int price, int min_lvl);
    virtual ~Product()=0;//make Product class abstract
    
    virtual void DisplayInfo() const=0; //Display stats of product
    std::string GetName() const;
    int GetPrice() const;
    int GetLvlReq() const;//Get level requirement
};

class Item:public Product{
public:
    Item(std::string name, int price, int min_lvl);
    virtual ~Item()=0; //make Item class abstract

    virtual void DisplayInfo() const =0;
};

class Weapon:public Item{
public:
    enum Hands{one_hand=1, both_hands=2};
private:
    int damage; //amount of damage the weapon inflicts
    Hands hands; //number of hands needed to hold it
public:
    Weapon(std::string name, int price, int min_lvl, int damage, Hands n);
    
    int GetDamage() const;
    Hands GetNumberOfHands() const;
    void DisplayInfo() const;
};

class Armor:public Item{
private:
    int damage_reduction; //when a hero wears it, received damage is reduced
                          //by "damage_reduction" percent(values 0-100)
public:
    Armor(std::string name, int price, int min_lvl, int damage_reduction);
    
    int GetDamageReduction() const;
    void DisplayInfo() const;
};

class Potion:public Item{
public:
    enum Stat{strength=1, dexterity=2, agility=3};
private:
    Stat stat_increased;
    int increase; //when a hero uses it, "stat_increased" is increased
                  //by "increase"
public:
    Potion(std::string name, int price, int min_lvl, Stat stat, int increase);
    
    Stat GetStatIncreased() const;//Get the stat that will be increased
    int GetIncrease() const;
    void DisplayInfo() const;
};

class Spell:public Product{
protected:
    int min_damage; //minimum damage that a spell can inflict
    int max_damage; //maximum damage that a spell can inflict
    int energy_req; //magic energy required to execute
public:
    Spell(std::string name, int price, int min_lvl,
          int min_damage,int max_damage,int energy_req);
    virtual ~Spell()=0; //make Spell class abstract

    int GetEnergyReq() const;
    int GetMinDamage() const;
    int GetMaxDamage() const;
    
    virtual Spell *Clone() const=0; /*Copy(clone) the spell("this" spell) and
                               return a ponter to the copy one*/
    virtual void ApplyEffect(Monster *M)=0;//Apply effect of this spell
                                           //on monster M
    virtual void DisplayInfo() const=0;
};

class IceSpell:public Spell{
private:
    int damage_reduction;// reduces range of damage of inflicted monster by "damage_reduction"%
public:
    IceSpell(std::string name, int price, int min_lvl,
             int min_damage,int max_damage,int energy_req,
             int damage_reduction);
    
    Spell *Clone() const; /*Copy(clone) the spell("this" spell) and
                               return a ponter to the copy one*/
    void ApplyEffect(Monster *M) ;
    void DisplayInfo() const;
};

class FireSpell:public Spell{
private:
    int defence_reduction;// reduces defence of inflicted monster by "defence_reduction"%
public:
    FireSpell(std::string name, int price, int min_lvl,
             int min_damage,int max_damage,int energy_req,
             int defence_reduction);
    
    Spell *Clone() const; /*Copy(clone) the spell("this" spell) and
                               return a ponter to copy one*/
    void ApplyEffect(Monster *M) ;
    void DisplayInfo() const;
};

class LightingSpell:public Spell{
private:
    int prob_reduction;// reduces probability of oponent monster to avoid the monster by "prob_reduction"%
public:
    LightingSpell(std::string name, int price, int min_lvl,
             int min_damage,int max_damage,int energy_req,
             int prob_reduction);
    
    Spell *Clone() const; /*Copy(clone) the spell("this" spell) and
                               return a ponter to copy one*/
    void ApplyEffect(Monster *M) ;
    void DisplayInfo() const;
};

#endif
