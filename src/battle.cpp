#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include "battle.h"
#include "grid.h"
#include "hero_products.h"

using namespace std;

Battle::Battle(Hero *hero1,Hero *hero2,Hero *hero3)
{
    srand((unsigned int) time(NULL));
    int i, max_lvl, min_lvl;
    int randNM=rand()%10; //random number of monsters
    int randLVL; //random level of monsters
    
    heroes[0]=hero1;
    heroes[1]=hero2;
    heroes[2]=hero3;
    
    
    //count number of heroes in the team and find max and min level
    if (heroes[1]==NULL)
    {
        heroes_num=1;
        max_lvl=heroes[0]->GetLevel();
        min_lvl=heroes[0]->GetLevel();
    }
    else if (heroes[2]==NULL)
    {
        heroes_num=2;
        max_lvl=(((heroes[1]->GetLevel()) > (heroes[0]->GetLevel())) ?
                 (heroes[1]->GetLevel()) : heroes[0]->GetLevel());
        min_lvl=(((heroes[0]->GetLevel()) > (heroes[1]->GetLevel())) ?
                 (heroes[1]->GetLevel()) : heroes[0]->GetLevel());
                 
    }
    else
    {
        heroes_num=3;
        max_lvl=(((heroes[1]->GetLevel()) > (heroes[2]->GetLevel())) ?
                 (heroes[1]->GetLevel()) : heroes[2]->GetLevel());
        max_lvl=(((heroes[0]->GetLevel()) > max_lvl ) ?
                 (heroes[0]->GetLevel()) : max_lvl);
        min_lvl=(((heroes[1]->GetLevel()) > (heroes[2]->GetLevel())) ?
                 (heroes[2]->GetLevel()) : heroes[1]->GetLevel());
        min_lvl=(((heroes[0]->GetLevel()) > min_lvl ) ?
                 min_lvl : (heroes[0]->GetLevel()));
                 
    }
    
    
    // Choose number of monsters depending on the number of heroes
    if (heroes_num==1) //1 hero
    {
        if (0<=randNM  && randNM<=4) //50% 1 monster
            monsters_num=1;
        else if (5<=randNM  && randNM<=7)//30% 2 monsters
            monsters_num=2;
        else if (8<=randNM  && randNM<=9)//20% 3 monsters
            monsters_num=3;
    }
    else if (heroes_num==2) //2 heroes
    {
        if (0<=randNM  && randNM<=1) //20% 1 monster
            monsters_num=1;
        else if (2<=randNM  && randNM<=6)//50% 2 monsters
            monsters_num=2;
        else if (7<=randNM  && randNM<=8)//20% 3 monsters
            monsters_num=3;
        else if (randNM==9)//10% 4 monsters
            monsters_num=4;
    }
    else if (heroes_num==3) //3 heroes
    {
        if (randNM==0) //10% 1 monster
            monsters_num=1;
        else if (1<=randNM  && randNM<=2)//20% 2 monsters
            monsters_num=2;
        else if (3<=randNM  && randNM<=6)//40% 3 monsters
            monsters_num=3;
        else if (7<=randNM  && randNM<=8)//20% 4 monsters
            monsters_num=4;
        else if (randNM==9)//10% 5 monsters
            monsters_num=5;
    }
    
    switch (monsters_num) {
        case 1:
            cout<<"A Monster appeared suddenly! A battle will start now..."<<endl;
            break;
        default:
            cout<<"Monsters appeared suddenly! A battle will start now..."<<endl;
            break;
    }
    
    string line,Name,Type,Str;
    int hP,BMinD,BMaxD,BD,BA;
    ifstream infile("Names_Stats/Monsters.txt"); //Read names and stats from this file
    istringstream is;
    int count_lines=0;
    int line_num;
        
    while (getline(infile,line)) //Count lines in the file
    {
        count_lines++;
    }
    
    count_lines--; //Ignore first line
    
    for (i=0;i<monsters_num;i++)//Create monsters
    {
        //Each monster has level in the range [min_lvl-1,max_lvl+1]
        randLVL=min_lvl-1+(rand()%(max_lvl+3-min_lvl));
        if (randLVL<1)
            randLVL=1;
        
        infile.clear();
        infile.seekg(0,ios::beg);
        
        line_num=1+(rand()%count_lines); //Choose random line/monster
        
        while ((line_num--))//consume previous lines
        {
            getline(infile,line);
        }
        
        getline(infile,Name,'-'); //read name
        getline(infile,Type,'-'); //read kind of monster
        getline(infile,Str,'-'); //read health power
        is.str(Str);  //convert to number
        is.clear();
        is>>hP;
        
        getline(infile,Str,'-'); //read BaseMinDamage
        is.str(Str);  //convert to number
        is.clear();
        is>>BMinD;
        
        getline(infile,Str,'-'); //read BaseMaxDamage
        is.str(Str);  //convert to number
        is.clear();
        is>>BMaxD;
        
        getline(infile,Str,'-'); //read BaseDefence
        is.str(Str);  //convert to number
        is.clear();
        is>>BD;
        
        getline(infile,Str); //read BaseAvoidance
        is.str(Str);  //convert to number
        is.clear();
        is>>BA;
        
        //Create monsters
        if (!(Type.compare("Dragon")))
            monsters[i]=new Dragon(Name,randLVL, hP,BMinD,BMaxD,BD,BA);
        else if (!(Type.compare("Exoskeleton")))
            monsters[i]=new Exoskeleton(Name,randLVL, hP,BMinD,BMaxD,BD,BA);
        else if (!(Type.compare("Spirit")))
            monsters[i]=new Spirit(Name,randLVL, hP,BMinD,BMaxD,BD,BA);
        
    }
}

Battle::~Battle()
{
    int i;
    
    for (i=0;i<monsters_num;i++)//Destroy monsters
    {
        delete monsters[i];
    }
}

bool Battle::MonstersAndHeroesAreAlive() const
{
    int i;
    bool is_alive=false;
    for (i=0; i<heroes_num; i++) //Check if at least one hero is alive
    {
        if (heroes[i]->IsAlive())
        {
            is_alive=true;
        }
    }
    if (!is_alive) //If all heroes have fainted
        return false;
    
    for (i=0; i<monsters_num; i++) //Check if at least one monster is alive
    {
        if (monsters[i]->IsAlive())
            return true;
    }
    
    return false; //If all monsters have fainted
}

void Battle::SpellsEndOfRound()
{
    int i;
    
    if (SpellsInEffect.empty())
        return;
    
    vector <SpellEffect>::iterator iter;
    iter=SpellsInEffect.begin();
    while (iter!=SpellsInEffect.end()) //for every spell
    {
        if (--(iter->rounds_left)==0) //reduce a round
        {
            iter=SpellsInEffect.erase(iter); //and delete it, if its effect finshed
        }
        else
            iter++;
    }
    
    //Recalculate monsters' stats, since some spells stopped their effects
    for (i=0;i<monsters_num;i++)//Calculate initial stats
    {
        monsters[i]->CalculateStats();
    }
    for (iter=SpellsInEffect.begin();iter!=SpellsInEffect.end();iter++)
    {//Apply effects of the spells which are still effective
        iter->spell->ApplyEffect(iter->monster);
    }
    return;
}

bool Battle::attack(Hero *hero)
{
    int i,j=1,m,mon=0,damage,defence,avoidance;
    Weapon *lhw,*rhw;
    
    srand((unsigned int) time(NULL));
    cout<<"Choose monster(number) that hero "<<hero->GetName()<<
    " will target to attack:"<<endl;
    for (i=0; i<monsters_num;i++)//Display alive monsters
    {
        if (monsters[i]->IsAlive())
        {
            cout<<j<<" :"<<monsters[i]->GetName()<<endl;
            j++;
        }
    }
    cout<<"Choice: ";
    cin>>m;
    j=1;
    for (i=0; i<monsters_num;i++)//Find monster "m"
    {
        if (monsters[i]->IsAlive())
        {
            if (j==m)
            {
                mon=i;
            }
            j++;
        }
    }
    if (mon<0 || mon>monsters_num || !(monsters[mon]->IsAlive()))//if monster is not valid
    {
        cout<<"Invalid or fainted monster, choose again."<<endl;
        return false;
    }
    
    avoidance=monsters[mon]->GetProbMissing();
    
    if ((1+(rand()%2000))>avoidance)//Decide if hero hits monster
    {
        /*Inflict damage*/
        lhw=hero->GetHWeaponsP()->GetWeaponLHand();
        rhw=hero->GetHWeaponsP()->GetWeaponRHand();
        
        damage=hero->GetStrength();
        if (lhw!=NULL) //if hero holds weapon in left hand
        {
            damage+=lhw->GetDamage(); //add its damage
            if (lhw->GetNumberOfHands()==Weapon::one_hand) //if the above weapon
                                                           //needs one hand
            {
                if (rhw!=NULL) //check if he holds a weapon in his right hand
                    damage+=rhw->GetDamage(); //and add its damage
            }
        }
        else if (rhw!=NULL) //check if he holds a weapon in his right hand
            damage+=rhw->GetDamage(); //and add its damage
        
        defence=monsters[mon]->GetDefence();
        damage-=defence; //substract defence from damage
        damage=(damage>10 ? damage : 10); //damage is at least 10
        monsters[mon]->InflictedDamage(damage);//Inflict damage
        cout<<"Hero "<<hero->GetName()<<" hit monster "<<
        monsters[mon]->GetName()<<"."<<endl;
        if (monsters[mon]->IsAlive()==false)
            cout<<monsters[mon]->GetName()<<" fainted..."<<endl;
        
    }
    else //missed
    {
        cout<<"Hero's attack missed"<<endl;
    }

    return true;
}


bool Battle::castSpell(Hero *hero)
{
    int i,j=1,sp,m,mon=0,MP,dexterity,min,max,damage,defence,avoidance;
    
    srand((unsigned int) time(NULL));

    cout<<"Choose spell(number) for hero "<<hero->GetName()<<":"<<endl;
    if (hero->GetHSpellsP()->DisplayProducts()==false)
        return false;
    cout<<"Choice: ";
    cin>>sp;
    sp--;
    if (sp<0 || sp>4 || ((hero->GetHSpellsP()->GetAvailableSpell(sp))==NULL))//if spell is not valid
    {
        cout<<"Invalid spell, choose again."<<endl;
        return false;
    }

    if ((hero->GetMP())<hero->GetHSpellsP()->GetAvailableSpell(sp)->GetEnergyReq())//if MP is not enough
    {
        cout<<"This hero has not enough magic Power for this spell."<<
        " Choose again."<<endl;
        return false;
    }
    
    if ((hero->GetLevel())<hero->GetHSpellsP()->GetAvailableSpell(sp)->GetLvlReq())
        //if hero's level does not meet spell's requirement
    {
        cout<<"This hero does not meet level requirement for this spell."<<
        " Choose again."<<endl;
        return false;
    }
    
    cout<<"Choose monster(number) to attack:"<<endl;
    for (i=0; i<monsters_num;i++)//Display alive monsters
    {
        if (monsters[i]->IsAlive())
        {
            cout<<j<<" :"<<monsters[i]->GetName()<<endl;
            j++;
        }
    }
    cout<<"Choice: ";
    cin>>m;
    j=1;
    for (i=0; i<monsters_num;i++)//Find monster "m"
    {
        if (monsters[i]->IsAlive())
        {
            if (j==m)
            {
                mon=i;
            }
            j++;
        }
    }
    if (mon<0 || mon>monsters_num || !(monsters[mon]->IsAlive()))//if monster is not valid
    {
        cout<<"Invalid or fainted monster, choose again."<<endl;
        return false;
    }
    
    MP=hero->GetHSpellsP()->GetAvailableSpell(sp)->GetEnergyReq(); //Use magic power
    hero->ReduceMP(MP);
    
    avoidance=monsters[mon]->GetProbMissing();
    
    if ((1+(rand()%2000))>avoidance)//Decide if spell hits monster
    {
        /*Inflict damage of spell*/
        min=hero->GetHSpellsP()->GetAvailableSpell(sp)->GetMinDamage(); //Calculate inflicted damage
        max=hero->GetHSpellsP()->GetAvailableSpell(sp)->GetMaxDamage();
        dexterity=hero->GetDexterity();
        damage=(dexterity/1000.0)*max+(1-(dexterity/1000.0))*min;
        defence=monsters[mon]->GetDefence();
        damage-=defence; //substract defence from damage
        damage=(damage>10 ? damage : 10); //damage is at least 10
        monsters[mon]->InflictedDamage(damage);//Inflict damage
        cout<<"Hero "<<hero->GetName()<<" hit monster "<<
        monsters[mon]->GetName()<<" with his spell."<<endl;
        
        if (monsters[mon]->IsAlive()==false)
            cout<<monsters[mon]->GetName()<<" fainted..."<<endl;
        
        /*Apply effect of spell*/
        SpellEffect S;

        S.spell=hero->GetHSpellsP()->GetAvailableSpell(sp);
        S.rounds_left=3; //Each spell lasts for 3 rounds
        S.monster=monsters[mon];
        
        SpellsInEffect.push_back(S); //Save the information about the spell
        
        S.spell->ApplyEffect(S.monster); //Apply effect of the spell
    }
    else //missed
    {
        cout<<"Spell missed its target"<<endl;
    }

    return true;
}

bool Battle::use(Hero *hero)
{
    return hero->GetHPotionsP()->UsePotion();
}

bool Battle::change(Hero *hero)
{
    bool boolW=false;
    if (!(hero->GetHWeaponsP()->ChangeWeapons(boolW)))
        return false;
    
    return (hero->GetHArmorsP()->ChangeArmorAfterWeapons(boolW));
}

void Battle::displayStats() const
{
    int i;
    
    cout<<"Stats of heroes are:"<<endl;
    for (i=0; i<heroes_num; i++)
    {
        heroes[i]->PrintStats();
    }
    cout<<endl;
    cout<<"Stats of monsters are:"<<endl;
    for (i=0; i<monsters_num; i++)
    {
        monsters[i]->PrintStats();
    }
    cout<<endl;
    return;
}


bool Battle::startBattle(){
    int i,choice;
    int randH; //random hero
    int randA; //choose a random number to determine if hero will avoid attack
    int damage;
    bool MoveCompleted; //true if a move  completed
    
    srand((unsigned int) time(NULL));
    
    while (MonstersAndHeroesAreAlive())
    {
        for (i=0; i<heroes_num; i++) //heroes' turn
        {
            choice=0;
            MoveCompleted=false;
            while (heroes[i]->IsAlive() && MoveCompleted==false &&
                   MonstersAndHeroesAreAlive()) /*If hero is alive and battle can be
                                                 continued, choose a move*/
            {
                cout<<"Choose number for hero "<<heroes[i]->GetName()<<":"<<endl;
                cout<<"1: Attack"<<endl;
                cout<<"2: Cast spell"<<endl;
                cout<<"3: Use potion"<<endl;
                cout<<"4: Change weapon and armor"<<endl;
                cout<<"5: Display all stats"<<endl;
                cout<<"6: Quit battle and game"<<endl;
                cout<<"Choice: ";
                cin>>choice;
                cout<<endl;
                if (choice>6 || choice<1)
                    cout<<"Wrong number, try again!"<<endl;
                else if (choice==1)
                    MoveCompleted=attack(heroes[i]);
                else if (choice==2)
                    MoveCompleted=castSpell(heroes[i]);
                else if (choice==3)
                    MoveCompleted=use(heroes[i]);
                else if (choice==4)
                    MoveCompleted=change(heroes[i]);
                else if (choice==5)
                    displayStats();
                else
                {
                    cout<<"Quitting battle and game now..."<<endl;
                    Grid::ForceQuit();
                    return false;
                }
                cout<<endl;
            }
        }
        for (i=0; i<monsters_num; i++) //monsters attack
        {
            if (monsters[i]->IsAlive() && MonstersAndHeroesAreAlive()) //If monster is
            {//alive and battle can be continued, make monster attack*/
                randH=rand()%heroes_num; //choose a random hero to be attacked
                while (!(heroes[randH]->IsAlive())) //if chosen hero has fainted
                {
                    randH=rand()%heroes_num; //choose again
                }
                randA=1+(rand()%2000);
                if (randA>(heroes[randH]->GetAgility()))//Decide if monster hits hero
                {
                    cout<<"Monster "<<monsters[i]->GetName()<<" hit hero "
                    <<heroes[randH]->GetName()<<endl;
                    
                    int min_dam=monsters[i]->GetMinDamage();
                    int max_dam=monsters[i]->GetMaxDamage();
                    
                    if (min_dam==max_dam)
                        damage=min_dam;
                    else//choose randomly from the range of damage of the monster
                        damage=min_dam+(rand()%(max_dam-min_dam));
                    

                    if (heroes[randH]->GetHArmorsP()->GetArmorProtection()!=-1)
                        //reduce damage due to armor
                        damage*=1-(heroes[randH]->GetHArmorsP()->GetArmorProtection())/100.0;
                    
                    damage=(damage>5 ? damage : 5); //damage is at least 5
                    heroes[randH]->InflictedDamage(damage);
                    if (heroes[randH]->IsAlive()==false)
                        cout<<heroes[randH]->GetName()<<" fainted..."<<endl;
                }
                else
                {
                    cout<<"Monster "<<monsters[i]->GetName()<<" missed hero "
                    <<heroes[randH]->GetName()<<endl;
                }
            }
        }
        
        for (i=0; i<heroes_num; i++)
        {
            if (heroes[i]->IsAlive())
            {
                heroes[i]->RegainHP(5); //Hero regains 5% HP
                heroes[i]->RegainMP(10); //Hero regains 10% MP
            }
        }
        for (i=0; i<monsters_num; i++)
        {
            if (monsters[i]->IsAlive())
                monsters[i]->RegainHP(5); //Monster regains 5% HP
        }
        SpellsEndOfRound();
        cout<<endl;
    }
    
    //If heroes won
    if ((heroes[0]->IsAlive() && heroes_num==1) ||
        (heroes[1]->IsAlive() && heroes_num==2) ||
        heroes[2]->IsAlive() )
    {
        /*Each hero gains 1+(monster's level)*(Hero's level) money
         for every defeated monster*/
        int sum=0; //sum of monsters' levels
        
        cout<<"Heroes won!"<<endl;
        
        for (i=0; i<monsters_num; i++)
        {
            sum+=monsters[i]->GetLevel();
        }
        
        for (i=0; i<heroes_num; i++)
        {
            heroes[i]->GainMoney(1+(heroes[i]->GetLevel())*sum);
        }
        
        /*Each hero gains 50*(monster's level)/(Hero's level) experience
         points for every defeated monster*/
        for (i=0; i<heroes_num; i++)
        {
            heroes[i]->GainExperience((int) (sum*(50.0/heroes[i]->GetLevel())));
            while (heroes[i]->GetLevel()<=100 &&
                   (heroes[i]->GetExperience())>
                   30*(1+(heroes[i]->GetLevel())*(heroes[i]->GetLevel())))
            {//next level needs 30*(1+level^2) experience points
                heroes[i]->levelUp();
            }
        }
    }
    
    else //if heroes lost
    {
        cout<<"Heroes lost..."<<endl;

        for (i=0; i<heroes_num; i++)
        {
            heroes[i]->LoseMoneyPercent(50);
        }
    }
    for (i=0; i<heroes_num; i++)
    {
        if (!(heroes[i]->IsAlive()))
        {
            heroes[i]->RegainHP(50); //Hero regains 50% HP
        }
    }
    
    return true;
}

