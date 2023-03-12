#include <iostream>
#include <string>
#include "hero_products.h"
#include "living.h"
#include "product.h"

using namespace std;


HeroProducts::HeroProducts(Hero *H)
{
    this->H=H;
}

HeroProducts::~HeroProducts(){}

HeroWeapons::HeroWeapons(Hero *H):HeroProducts(H)
{
    int i;
    
    lhand=NULL;
    rhand=NULL;
    
    for (i=0;i<5; i++)
    {
        weapons[i]=NULL;
    }
}

HeroWeapons::~HeroWeapons()
{
    int i;
    
    for (i=0; i<5; i++)
    {
        if (weapons[i]!=NULL)
        {
            delete weapons[i];
        }
    }
}


Weapon *HeroWeapons::GetWeaponLHand() const
{
    return lhand;
}

Weapon *HeroWeapons::GetWeaponRHand() const
{
    return rhand;
}

Weapon *HeroWeapons::GetAvailableWeapon(int pos) const
{
    if (pos<0 || pos>4)
        return NULL;
    return weapons[pos];
}

void HeroWeapons::AddWeaponLHand(int pos)
{
    lhand=weapons[pos];
    return;
}

void HeroWeapons::AddWeaponRHand(int pos)
{
    rhand=weapons[pos];
    return;
}

bool HeroWeapons::ChangeWeapons(bool& boolW)
{
    boolW=false;
    char answer[4],hand[6];
    Weapon *lhw,*rhw;
    int w_pos, w_pos2;

    lhw=GetWeaponLHand();
    rhw=GetWeaponRHand();

    if (lhw==NULL && rhw==NULL)//if hero does not hold any weapon
    {
        cout<<"Hero "<<H->GetName()<<" holds no weapon with his hands."<<endl;
        cout<<"Do you want to add one or two? (Y/N)"<<endl;
    }
    else if (lhw==NULL)//if hero holds a weapon only with his right hand
    {
        cout<<"Hero "<<H->GetName()<<" holds with his right hand"<<endl;
        cout<<"weapon "<<rhw->GetName()<< " with features:"<<endl;
        rhw->DisplayInfo();
        cout<<"Do you want to add or change weapons? (Y/N)"<<endl;
    }
    else if (rhw==NULL)//if hero holds a weapon only with his left hand
    {
        cout<<"Hero "<<H->GetName()<<" holds with his left hand"<<endl;
        cout<<"weapon "<<lhw->GetName()<< " with features:"<<endl;
        lhw->DisplayInfo();
        cout<<"Do you want to add or change weapons? (Y/N)"<<endl;
    }
    else if (rhw==lhw)//if hero holds a weapon with both his hands
    {
        cout<<"Hero "<<H->GetName()<<" holds with both hands ";
        cout<<"weapon "<<lhw->GetName()<< " with features:"<<endl;
        lhw->DisplayInfo();
        cout<<"Do you want to change weapons? (Y/N)"<<endl;
    }
    else//if hero holds a weapon in each hand
    {
        cout<<"Hero "<<H->GetName()<<" holds a weapon in each hand."<<endl;
        cout<<"Do you want to change weapons? (Y/N)"<<endl;
    }

    cout<<"Choice: ";
    cin>>answer;
    cout<<endl;
    if (answer[0]=='Y' || answer[0]=='y')
    {
        cout<<"Choose weapon(number):"<<endl;
        if (DisplayProducts()==false)
        {
            return false;
        }
        cout<<"Choice: ";
        cin>>w_pos;
        cout<<endl;
        w_pos--;
        if (w_pos<0 || w_pos>4 ||
            ((GetAvailableWeapon(w_pos))==NULL))//if weapon is not valid
        {
            cout<<"Invalid weapon, try again."<<endl;
            return false;
        }
        else if ((H->GetLevel())<GetAvailableWeapon(w_pos)->GetLvlReq())
            //if hero's level does not meet weapon's requirement
        {
            cout<<"This hero does not meet level requirement for this weapon."<<
            " Try again."<<endl;
            return false;
        }
        else
        {
            if (GetAvailableWeapon(w_pos)->GetNumberOfHands()==Weapon::both_hands)
            {//if player chose a weapon that needs both hands
                AddWeaponLHand(w_pos);
                AddWeaponRHand(w_pos);
            }
            else
            {
                cout<<"Choose hand (L/R):"<<endl;
                cout<<"Choice: ";
                cin>>hand;
                cout<<endl;
                if (hand[0]=='L' || hand[0]=='l')
                {//if left hand is chosen
                    AddWeaponLHand(w_pos);
                    if (rhand!=NULL && rhand->GetNumberOfHands()==Weapon::both_hands)
                    {//If weapon in right hand needs both hands
                        rhand=NULL; //remove it
                    }
                    else if (rhand!=NULL && rhand==lhand)
                    {//If weapon in both hands is the same
                        cout<<"Hero simply changed with which hand holds his weapon."
                        <<endl;
                        rhand=NULL; //remove the weapon from right hand
                    }
                    cout<<"Do you also want to add/change weapon in right hand? (Y/N)"<<
                    endl;
                    cout<<"Choice: ";
                    cin>>answer;
                    cout<<endl;
                    if (answer[0]=='Y' || answer[0]=='y')
                    {//change right hand too
                        cout<<"Choose weapon(number):"<<endl;
                        DisplayProducts();
                        cout<<"Choice: ";
                        cin>>w_pos2;
                        cout<<endl;
                        w_pos2--;
                        if (w_pos2<0 || w_pos2>4 ||
                            ((GetAvailableWeapon(w_pos2))==NULL))//if weapon is not valid
                        {
                            cout<<"Invalid weapon, try again."<<endl;
                            return false;
                        }
                        else if ((H->GetLevel()) <
                            GetAvailableWeapon(w_pos)->GetLvlReq())
                            //if hero's level does not meet weapon's requirement
                        {
                            cout<<"This hero does not meet level requirement for this weapon."<<
                            "No change in right hand"<<endl;
                            return false;
                        }
                        else if (w_pos2==w_pos)
                        {
                            cout<<"Same one-hand weapon in both hands is not allowed"<<
                            endl;
                            cout<<"No change in right hand"<<endl;
                        }
                        else if (GetAvailableWeapon(w_pos2)->GetNumberOfHands()
                                 == Weapon::both_hands)
                        {
                            cout<<"This weapon needs both hands"<<endl;
                            cout<<"No change in right hand"<<endl;
                        }
                        else
                        {
                            AddWeaponRHand(w_pos2);
                        }
                    }
                    else if (answer[0]!='N' && answer[0]!='n')
                    {
                        cout<<"Invalid answer, no change in left hand"<<endl;
                    }
                }
                else if (hand[0]=='R' || hand[0]=='r')
                {//if right hand is chosen
                    AddWeaponRHand(w_pos);
                    if (lhand!=NULL && lhand->GetNumberOfHands()==Weapon::both_hands)
                    { //If weapon in left hand needs both hands
                        lhand=NULL; //remove it
                    }
                    else if (lhand!=NULL && lhand==rhand)
                    {//If weapon in both hands is the same
                        cout<<"Hero simply changed with which hand holds his weapon."
                        <<endl;
                        lhand=NULL; //remove the weapon from left hand
                    }
                    cout<<"Do you also want to add/change weapon in left hand? (Y/N)"<<
                    endl;
                    cout<<"Choice: ";
                    cin>>answer;
                    cout<<endl;
                    if (answer[0]=='Y' || answer[0]=='y')
                    {//change left hand too
                        cout<<"Choose weapon(number):"<<endl;
                        DisplayProducts();
                        cout<<"Choice: ";
                        cin>>w_pos2;
                        w_pos2--;
                        if (w_pos<0 || w_pos>4 ||
                            ((GetAvailableWeapon(w_pos))==NULL))//if weapon is not valid
                        {
                            cout<<"Invalid weapon, try again."<<endl;
                            return false;
                        }
                        else if ((H->GetLevel()) <
                            GetAvailableWeapon(w_pos)->GetLvlReq())
                            //if hero's level does not meet weapon's requirement
                        {
                            cout<<"This hero does not meet level requirement for this weapon."<<
                            "No change in left hand"<<endl;
                            return false;
                        }
                        else if (w_pos2==w_pos)
                        {
                            cout<<"Same one-hand weapon in both hands is not allowed"<<
                            endl;
                            cout<<"No change in left hand!"<<endl;
                        }
                        else if (GetAvailableWeapon(w_pos2)->GetNumberOfHands()
                                 == Weapon::both_hands)
                        {
                            cout<<"This weapon needs both hands"<<endl;
                            cout<<"No change in left hand"<<endl;
                        }
                        else
                        {
                            AddWeaponLHand(w_pos2);
                        }
                    }
                    else if (answer[0]!='N' && answer[0]!='n')
                    {
                        cout<<"Invalid answer, no change in left hand"<<endl;
                    }
                }
                else
                    return false;
            }
            boolW=true;
        }
    }
    else if (answer[0]=='N' || answer[0]=='n')
    {
        boolW=false;
    }
    else
    {
        cout<<"Invalid answer, try again"<<endl;
        return false;
    }
    return true;
}

bool HeroWeapons::DisplayProducts() const
{
    int i;
    
    if (GetAvailableWeapon(0)==NULL)
    {
        cout<<"No available weapons"<<endl;
        return false;
    }
    
    for (i=0; i<=4; i++)
    {
        if (GetAvailableWeapon(i)!=NULL && GetAvailableWeapon(i)==lhand && lhand==rhand)
        {
            cout<<i+1<<" :"<<GetAvailableWeapon(i)->GetName()<<
            "(currently used with both hands) with features:"<<endl;
            GetAvailableWeapon(i)->DisplayInfo();
        }
        else if (GetAvailableWeapon(i)!=NULL && GetAvailableWeapon(i)==lhand)
        {
            cout<<i+1<<" :"<<GetAvailableWeapon(i)->GetName()<<
            "(currently used with left hand) with features:"<<endl;
            GetAvailableWeapon(i)->DisplayInfo();
        }
        else if (GetAvailableWeapon(i)!=NULL && GetAvailableWeapon(i)==rhand)
        {
            cout<<i+1<<" :"<<GetAvailableWeapon(i)->GetName()<<
            "(currently used with right hand) with features:"<<endl;
            GetAvailableWeapon(i)->DisplayInfo();
        }
        else if (GetAvailableWeapon(i)!=NULL)
        {
            cout<<i+1<<" :"<<GetAvailableWeapon(i)->GetName()<<" with features:"<<endl;
            GetAvailableWeapon(i)->DisplayInfo();
        }
    
    }
    return true;
}

void HeroWeapons::SellWeapon(int pos)
{
    int i=pos;
    Weapon *W;
    
    if ((W=GetAvailableWeapon(pos))==NULL)
        cout<<"Invalid number, try again!"<<endl;
    else if (lhand==W || rhand==W)
    {
        cout<<"This weapon is currently used and cannot be selled!"<<endl;
        cout<<"Remove this weapon and try again!"<<endl;
    }
    else
    {
        H->GainMoney(W->GetPrice()/2);//Sell weapon

        while (i<4 && weapons[i+1]!=NULL)//Find last weapon
        {
            i++;
        }
        weapons[pos]=weapons[i];//and swap it with the empty spot
        weapons[i]=NULL;
        
        delete W;
    }
    
    return;
}

void HeroWeapons::BuyWeapon(Weapon *W)
{
    int i=0;
    
    if (W->GetPrice()>H->GetMoney())
        cout<<"Not enough money to buy this weapon!"<<endl;
    else if (GetAvailableWeapon(4)!=NULL)
        cout<<"Hero already has 5 weapons! Not enough space for more!"<<endl;
    else
    {
        while (weapons[i]!=NULL)//Find an empty spot
        {
            if (!(weapons[i]->GetName().compare(W->GetName())))
            {
                cout<<"Hero already has this weapon!"<<endl;
                return;
            }
            i++;
        }
        H->GainMoney(-(W->GetPrice()));//Buy weapon
        weapons[i]=new Weapon(*W); //Add a copy of the weapon
    }
    return;
}

HeroArmors::HeroArmors(Hero *H):HeroProducts(H)
{
    int i;
    
    armor=NULL;
    
    for (i=0;i<5; i++)
    {
        armors[i]=NULL;
    }
}

HeroArmors::~HeroArmors()
{
    int i;
    
    for (i=0; i<5; i++)
    {
        if (armors[i]!=NULL)
        {
            delete armors[i];
        }
    }
}

Armor *HeroArmors::GetArmor() const
{
    return armor;
}

int HeroArmors::GetArmorProtection() const
{
    if (armor==NULL)
        return -1;
    return armor->GetDamageReduction();
}

Armor *HeroArmors::GetAvailableArmor(int pos) const
{
    if (pos<0 || pos>4)
        return NULL;
    return armors[pos];
}

bool HeroArmors::ChangeArmorAfterWeapons(bool boolW)
{
    bool boolA=false;
    char answer[4];
    Armor *arm;
    int arm_pos;
    
    arm=GetArmor();
    
    if (arm==NULL)
    {
        cout<<"Hero "<<H->GetName()<<" wears no armor"<<endl;
        cout<<"Do you want him to wear an armor? (Y/N)"<<endl;
    }
    else
    {
        cout<<"Hero "<<H->GetName()<<" wears armor "<<arm->GetName()<<
        " with features:"<<endl;
        DisplayProducts();
        cout<<"Do you want to change armor? (Y/N)"<<endl;
    }
    
    cout<<"Choice: ";
    cin>>answer;
    cout<<endl;
    if (answer[0]=='Y' || answer[0]=='y')
    {
        cout<<"Choose armor(number):"<<endl;
        if (DisplayProducts()==true)
        {
            cout<<"Choice: ";
            cin>>arm_pos;
            arm_pos--;
            if (arm_pos<0 || arm_pos>4 ||
                ((GetAvailableArmor(arm_pos))==NULL))//if armor is not valid
            {
                if (boolW)
                    cout<<"Invalid answer, hero will not wear/change armor!"<<endl;
                else
                {
                    cout<<"Invalid answer, try again"<<endl;
                    return false;
                }
            }
            else if ((H->GetLevel()) <
                     GetAvailableArmor(arm_pos)->GetLvlReq())
                     //if hero's level does not meet armor's requirement
                 {
                     cout<<"This hero does not meet level requirement for this armor."<<endl;
                     if (boolW)
                         cout<<"Hero will not wear/change armor!"<<endl;
                     else
                     {
                         cout<<"Try again!"<<endl;
                         return false;
                     }
                 }
            else
            {
                WearArmor(arm_pos);
                boolA=true;
            }
        }
        else
        {
            if (boolW)
                cout<<"Hero cannot wear armor!"<<endl;
            else
                return false;
            boolA=false;
        }
    
    }
    else if (answer[0]=='N' || answer[0]=='n')
    {
        boolA=false;
    }
    else
    {
        if (boolW)
            cout<<"Invalid answer, hero will not wear/change armor!"<<endl;
        else
        {
            cout<<"Invalid answer, try again"<<endl;
            return false;
        }
        boolA=false;
    }
    
    return (boolW || boolA);
}

void HeroArmors::WearArmor(int pos)
{
    armor=armors[pos];
    return;
}

bool HeroArmors::DisplayProducts() const
{
    int i;
    
    if (GetAvailableArmor(0)==NULL)
    {
        cout<<"No available armors"<<endl;
        return false;
    }
    
    for (i=0; i<=4; i++)//Display armors
    {
        if ((GetAvailableArmor(i))!=NULL && GetAvailableArmor(i)==armor)
        {
            cout<<i+1<<" :"<<GetAvailableArmor(i)->GetName()<<
            "(currently used) with features:"<<endl;
            GetAvailableArmor(i)->DisplayInfo();
        }
        else if ((GetAvailableArmor(i))!=NULL)
        {
            cout<<i+1<<" :"<<GetAvailableArmor(i)->GetName()<<" with features:"<<endl;
            GetAvailableArmor(i)->DisplayInfo();
        }
    }
    return true;
}

void HeroArmors::SellArmor(int pos)
{
    int i=pos;
    Armor *A;
    
    if ((A=GetAvailableArmor(pos))==NULL)
        cout<<"Invalid number, try again!"<<endl;
    else if (armor==A)
    {
        cout<<"This armor is currently used and cannot be selled!"<<endl;
        cout<<"Remove this armor and try again!"<<endl;
    }
    else
    {
        H->GainMoney(A->GetPrice()/2);//Sell armor

        while (i<4 && armors[i+1]!=NULL)//Find last armor
        {
            i++;
        }
        armors[pos]=armors[i];//and swap it with the empty spot
        armors[i]=NULL;
        
        delete A;
    }
    
    return;
}

void HeroArmors::BuyArmor(Armor *A)
{
    int i=0;
    
    if (A->GetPrice()>H->GetMoney())
        cout<<"Not enough money to buy this armor!"<<endl;
    else if (GetAvailableArmor(4)!=NULL)
        cout<<"Hero already has 5 armors! Not enough space for more!"<<endl;
    else
    {
        while (armors[i]!=NULL)//Find an empty spot
        {
            if (!(armors[i]->GetName().compare(A->GetName())))
            {
                cout<<"Hero already has this armor!"<<endl;
                return;
            }
            i++;
        }
        H->GainMoney(-(A->GetPrice()));//Buy armor
        armors[i]=new Armor(*A);//Add a copy of the armor
    }
    return;
}

HeroPotions::HeroPotions(Hero *H):HeroProducts(H)
{
    int i;
    
    for (i=0;i<100; i++)
    {
        UsedPotions[i]=NULL;
    }
    
    for (i=0;i<5; i++)
    {
        potions[i]=NULL;
    }
}

HeroPotions::~HeroPotions()
{
    int i;
    
    for (i=0; i<5; i++)
    {
        if (potions[i]!=NULL)
        {
            delete potions[i];
        }
    }
    
    for (i=0; i<100; i++)
    {
        if (UsedPotions[i]!=NULL)
        {
            delete UsedPotions[i];
        }
    }
}

Potion *HeroPotions::GetAvailablePotion(int pos) const
{
    if (pos<0 || pos>4)
        return NULL;
    return potions[pos];
}

Potion *HeroPotions::GetUsedPotion(int pos) const
{
    if (pos<0 || pos>99)
        return NULL;
    return UsedPotions[pos];
}

void HeroPotions::MoveUsedPotion(int pos)
{
    int i=0;
    
    while (UsedPotions[i]!=NULL)//find an empty spot
    {
        i++;
    }
    
    UsedPotions[i]=potions[pos]; //Copy potion there
    
    i=pos;
    while (i<4 && potions[i+1]!=NULL) //find last potion
    {
        i++;
    }
    potions[pos]=potions[i];
    potions[i]=NULL;
    
    return;
}

void HeroPotions::ApplyPotions(double& strength, double& dexterity, double& agility)
{
    int inc,pos=0;
    Potion *P;
    
    while ((P=GetUsedPotion(pos))!=NULL)//Calculate increases due to potions
    {
        inc=P->GetIncrease();
        switch (P->GetStatIncreased())
        {
            case Potion::strength:
                strength+=inc;
                break;
            case Potion::dexterity:
                dexterity+=inc;
                break;
            case Potion::agility:
                agility+=inc;
                break;
        }
        pos++;
    }
    
    return;
}

bool HeroPotions::UsePotion()

{
    int pot;
    
    if (GetUsedPotion(99)!=NULL)//If 100 potions have been used
    {
        cout<<"This hero has used 100 potions and cannot use more!"<<endl;
        return false;
    }
    
    cout<<"Choose potion(number) for hero "<<H->GetName()<<":"<<endl;
    if (DisplayProducts()==false)
        return false;
    cout<<"Choice: ";
    cin>>pot;
    pot--;
    if (pot<0 || pot>4 || ((GetAvailablePotion(pot))==NULL))//if potion is not valid
    {
        cout<<"Invalid potion, choose again."<<endl;
        return false;
    }
    
    if ((H->GetLevel())<GetAvailablePotion(pot)->GetLvlReq())
        //if hero's level does not meet potion's requirement
    {
        cout<<"This hero does not meet level requirement for this potion."<<
        " Choose again."<<endl;
        return false;
    }
 
    MoveUsedPotion(pot); //Move potion in used potions list of hero
    H->CalculateStats();//Recalculate hero's stats
    
    return true;
}

bool HeroPotions::DisplayProducts() const
{
    int i;
    
    if (GetAvailablePotion(0)==NULL)
    {
        cout<<"No available potions"<<endl;
        return false;
    }
    
    for (i=0; i<=4; i++)//Display potions
    {
        if ((GetAvailablePotion(i))!=NULL)
        {
            cout<<i+1<<" :"<<GetAvailablePotion(i)->GetName()<<" with features:"<<endl;
            GetAvailablePotion(i)->DisplayInfo();
        }
    }
    return true;
}

void HeroPotions::SellPotion(int pos)
{
    int i=pos;
    Potion *P;
    
    if ((P=GetAvailablePotion(pos))==NULL)
        cout<<"Invalid number, try again!"<<endl;
    else
    {
        H->GainMoney(P->GetPrice()/2);//Sell potion

        while (i<4 && potions[i+1]!=NULL)//Find last potion
        {
            i++;
        }
        potions[pos]=potions[i];//and swap it with the empty spot
        potions[i]=NULL;
        
        delete P;
    }
    
    return;
}

void HeroPotions::BuyPotion(Potion *P)
{
    int i=0;
    
    if (P->GetPrice()>H->GetMoney())
        cout<<"Not enough money to buy this potion!"<<endl;
    else if (GetAvailablePotion(4)!=NULL)
        cout<<"Hero already has 5 potions! Not enough space for more!"<<endl;
    else
    {
        while (potions[i]!=NULL)//Find an empty spot
        {
            i++;
        }
        H->GainMoney(-(P->GetPrice()));//Buy potion
        potions[i]=new Potion(*P);//Add a copy of the potion
    }
    return;
}

HeroSpells::HeroSpells(Hero *H):HeroProducts(H)
{
    int i;
    
    for (i=0;i<5; i++)
    {
        spells[i]=NULL;
    }
}

HeroSpells::~HeroSpells()
{
    int i;
    
    for (i=0; i<5; i++)
    {
        if (spells[i]!=NULL)
        {
            delete spells[i];
        }
    }
}

Spell *HeroSpells::GetAvailableSpell(int pos) const
{
    if (pos<0 || pos>4)
        return NULL;
    return spells[pos];
}

bool HeroSpells::DisplayProducts() const
{
    int i;
    
    if (GetAvailableSpell(0)==NULL)
    {
        cout<<"No available spells"<<endl;
        return false;
    }
    
    for (i=0; i<=4; i++)//Display spells
    {
        if ((GetAvailableSpell(i))!=NULL)
        {
            cout<<i+1<<" :"<<GetAvailableSpell(i)->GetName()<<" with features:"<<endl;
            GetAvailableSpell(i)->DisplayInfo();
        }
    }
    return true;
}

void HeroSpells::SellSpell(int pos)
{
    int i=pos;
    Spell *S;
    
    if ((S=GetAvailableSpell(pos))==NULL)
        cout<<"Invalid number, try again!"<<endl;
    else
    {
        H->GainMoney(S->GetPrice()/2);//Sell spell

        while (i<4 && spells[i+1]!=NULL)//Find last spell
        {
            i++;
        }
        spells[pos]=spells[i];//and swap it with the empty spot
        spells[i]=NULL;
        
        delete S;
    }
    return;
}

void HeroSpells::BuySpell(Spell *S)
{
    int i=0;
    
    if (S->GetPrice()>H->GetMoney())
        cout<<"Not enough money to buy this spell!"<<endl;
    else if (GetAvailableSpell(4)!=NULL)
        cout<<"Hero already has 5 spells! Not enough space for more!"<<endl;
    else
    {
        while (spells[i]!=NULL)//Find an empty spot
        {
            if (!(spells[i]->GetName().compare(S->GetName())))
            {
                cout<<"Hero already has this spell!"<<endl;
                return;
            }
            i++;
        }
        H->GainMoney(-(S->GetPrice()));//Buy spell
        spells[i]=S->Clone();//Create a copy of spell S and add it in "spells"
    }
    return;
}

