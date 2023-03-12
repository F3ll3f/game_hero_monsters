#ifndef FILE_HERO_PRODUCTS_H_INCLUDED
#define FILE_HERO_PRODUCTS_H_INCLUDED

#include "living.h"


class HeroProducts{ //Products that a hero has bought
protected:
    Hero *H; //The hero that has these products
public:
    HeroProducts(Hero *H);
    virtual ~HeroProducts();
    
    virtual bool DisplayProducts() const=0;
};

class HeroWeapons:public HeroProducts{
private:
    Weapon *lhand; //weapon that hero currently holds(uses) in his left hand
    Weapon *rhand; //weapon that hero currently holds(uses) in his right hand
    Weapon *weapons[5];//5 is the maximum number of weapons a hero can have(not in use)
                      //If hero has n weapons, weapons occupy the first n positions
                      //Remaining weapons are NULL
public:
    HeroWeapons(Hero *H);
    ~HeroWeapons();

    Weapon *GetWeaponLHand() const;
    Weapon *GetWeaponRHand() const;
    Weapon *GetAvailableWeapon(int pos) const; //Get weapon in position "pos" in
                                              //"weapons". Returns Null if weapon in
                                              //position "pos" does not exist
    void AddWeaponLHand(int pos);//Add weapon from potition "pos" in lhand
    void AddWeaponRHand(int pos);//Add weapon from potition "pos" in rhand
    bool ChangeWeapons(bool& boolW);/*Returns false only if an invalid entry was
                                     entered before a change. boolW keeps track if
                                     player changed weapon*/
    bool DisplayProducts() const;//Display counted available weapons
    void SellWeapon(int pos);//Sell weapon in position "pos"
    void BuyWeapon(Weapon *W);//Buy a copy of weapon W
};

class HeroArmors:public HeroProducts{
private:
    Armor *armor; //armor that hero currently wears
    Armor *armors[5];//5 is the maximum number of armors a hero can have(not in use)
                      //If hero has n armors, armors occupy the first n positions
                      //Remaining armors are NULL
public:
    HeroArmors(Hero *H);
    ~HeroArmors();
    
    Armor *GetArmor() const;
    int GetArmorProtection() const; //Returns -1 if there is no armor
    Armor *GetAvailableArmor(int pos) const;//Get armor in position "pos" in "armors"
                                      //Returns Null if armor in position "pos"
                                      //does not exist
    bool ChangeArmorAfterWeapons(bool boolW);/*Returns true only if player changed
                                             weapon or armor. boolW shows if
                                             player changed weapon immediately
                                             before calling this function*/
    void WearArmor(int pos);//Add Armor from potition "pos" in armor
    bool DisplayProducts() const;//Display counted available armors
    void SellArmor(int pos);//Sell armor in position "pos"
    void BuyArmor(Armor *A);//Buy a copy of armor A
};

class HeroPotions:public HeroProducts{
private:
    Potion *potions[5];//5 is the maximum number of potions a hero can hold
                      //If hero has n potions, potions occupy the first n positions
                      //Remaining positions are NULL
    Potion *UsedPotions[100];//100 is the maximum number of potions a hero can use
                             //If hero has used n spells, spells occupy the first n
                             //positions. Remaining positions are NULL
public:
    HeroPotions(Hero *H);
    ~HeroPotions();

    Potion *GetAvailablePotion(int pos) const; //Returns Null if potion in position "pos"
                              //does not exist
    Potion *GetUsedPotion(int pos) const; //Returns Null if potion in position "pos" in
                              //"UsedPotions" does not exist
    void MoveUsedPotion(int pos);//Move potion from potions from potition "pos"
                                 //to UsedPotions. Ensure this is possible before
                                 //calling it.
    void ApplyPotions(double& strength, double& dexterity, double& agility);/*Apply
                                                        potion increases to stats*/
    bool UsePotion();//Returns true only if he hero used a potion
    bool DisplayProducts() const;//Display counted available potions
    void SellPotion(int pos);//Sell potion in position "pos"
    void BuyPotion(Potion *P);//Buy a copy of potion P
};

class HeroSpells:public HeroProducts{
private:
    Spell *spells[5];//5 is the maximum number of spells a hero is able to know
                    //If hero has n spells, spells occupy the first n positions
                    //Remaining positions are NULL
public:
    HeroSpells(Hero *H);
    ~HeroSpells();

    Spell *GetAvailableSpell(int pos) const; //Returns Null if spell in position "pos"
                                       //does not exist
    bool DisplayProducts() const;//Display counted available spells
    void SellSpell(int pos);//Sell spell in position "pos"
    void BuySpell(Spell *S);//Buy a copy of spell S
};


#endif



