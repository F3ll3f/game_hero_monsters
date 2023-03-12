#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "grid.h"
#include "battle.h"
#include "hero_products.h"

using namespace std;


bool Grid::DoQuit=false;

Grid::Grid(bool CreateMap, char **Map, int rows, int columns)
{
    int i,j;
    bool DefaultMap=false;
    Team[0]=NULL; Team[1]=NULL; Team[2]=NULL;
    position[0]=-1; position[1]=-1;
    
    if (CreateMap)//Create a map
    {
        cout<<"Choose number of rows:"<<endl;
        cout<<"Choice: ";
        cin>>rows;
        cout<<"Choose number of columns:"<<endl;
        cout<<"Choice: ";
        cin>>columns;
        Map=new char*[rows];
        for (i=0;i<rows;i++)
        {
            Map[i]=new char[columns];
        }
        for (i=0;i<rows; i++)
        {
            for (j=0;j<columns; j++)
            {
                if (i==0 && j==0)
                {
                    Map[i][j]='C';
                }
                else
                {
                    cout<<"Choose type for square ("<<i<<","<<j<<")"<<endl;
                    cout<<"Enter N, C or M:"<<endl;
                    cout<<"Choice: ";
                    cin>>(Map[i][j]);
                    while (Map[i][j]!='N' && Map[i][j]!='M' && Map[i][j]!='C')
                    {
                        cout<<"Wrong format! "<<
                        "Choose type for square ("<<i<<","<<j<<")"<<endl;
                        cout<<"Enter N, C or M:"<<endl;
                        cout<<"Choice: ";
                        cin>>(Map[i][j]);
                    }
                }
            }
        }
    }
    else if (columns==0 || rows==0 || Map==NULL)//Create a default map
    {
        int N=20;
        
        DefaultMap=true;
        rows=N;
        columns=N;
        
        Map=new char*[rows];
        for (i=0;i<rows;i++)
        {
            Map[i]=new char[columns];
        }
        for (i=0;i<rows; i++)
        {
            for (j=0;j<columns; j++)
            {
                if (i<N/2 && j>(N/2+i) )
                    Map[i][j]='N';
                else if (i>=N/2 && j<(i-N/2) )
                    Map[i][j]='N';
                else if (i==j && (i%4)==1)
                    Map[i][j]='M';
                else
                    Map[i][j]='C';
            }
        }
    }
    
    size[0]=rows+2;
    size[1]=columns+2;
    
    squares=new Square**[size[0]];
    
    for (i=0;i<size[0];i++)
    {
        squares[i]=new Square*[size[1]];
    }
    
    
    for (j=0;j<size[1];j++) //Perimeter of Map is not accessible
    {
        squares[0][j]=new nonAccessible(this);
        squares[size[0]-1][j]=new nonAccessible(this);
    }
    
    for (i=0;i<size[0];i++)
    {
        squares[i][0]=new nonAccessible(this);
        squares[i][size[1]-1]=new nonAccessible(this);
    }
    
    for (i=1;i<size[0]-1; i++)
    {
        for (j=1;j<size[1]-1; j++)
        {
            if (i==1 && j==1)
            {
                position[0]=1;
                position[1]=1;
               
                squares[1][1]=new common(this,true); //team starts at (1,1)
            }
            else
            {
                switch (Map[i-1][j-1]){
                    case 'N':
                        squares[i][j]=new nonAccessible(this);
                        break;
                    case 'C':
                        squares[i][j]=new common(this);
                        break;
                    case 'M':
                        squares[i][j]=new market(this);
                        break;
                }
            }
        }
    }
    
    if (CreateMap || DefaultMap)//If a new map was created
    {
        for (i=0;i<rows;i++)
        {
            delete[] Map[i];
        }
        delete[] Map;
    }
}

Grid::~Grid()
{
    int i,j;
    
    for (i=0;i<size[0]; i++)
    {
        for (j=0;j<size[1]; j++)
        {
            delete squares[i][j];
        }
        delete[] squares[i];
    }
    delete[] squares;
}

void Grid::move(direction d)
{
    int i=position[0],j=position[1];
    int cp_i=i,cp_j=j;
    squares[i][j]->Exit();
    switch (d){
        case UP:
            i--;
            break;
            
        case RIGHT:
            j++;
            break;
            
        case DOWN:
            i++;
            break;
            
        case LEFT:
            j--;
            break;
    }
    
    
    if (!(squares[i][j]->Enter()))//If enter did not succeed
    {
        squares[i][j]->Exit(); //Exit nonAccessible square
        squares[cp_i][cp_j]->Enter(); //Enter previous square
    }
    else
    {
        position[0]=i;
        position[1]=j;
    }
    return;
}

void Grid::checkInventory(Hero *H) const
{
    cout<<"Hero "<<H->GetName()<<" has the following items and spells:"<<endl;
    cout<<"Weapons:"<<endl;
    H->GetHWeaponsP()->DisplayProducts();
    cout<<"Armors:"<<endl;
    H->GetHArmorsP()->DisplayProducts();
    cout<<"Potions:"<<endl;
    H->GetHPotionsP()->DisplayProducts();
    cout<<"Spells:"<<endl;
    H->GetHSpellsP()->DisplayProducts();
    return;
}

void Grid::equip(Hero *H)
{
    bool bool_weapon=false;
    if (!(H->GetHWeaponsP()->ChangeWeapons(bool_weapon)))
        return;
    H->GetHArmorsP()->ChangeArmorAfterWeapons(bool_weapon);
    return;
}

void Grid::use(Hero *H)
{
    H->GetHPotionsP()->UsePotion();
    return;
}

void Grid::displayInfo() const
{
    int i;
    for (i=0; i<heroes_num; i++)
    {
        Team[i]->PrintStats();
    }
    return;
}

void Grid::displayMap() const
{
    int i,j;
    cout<<"Grid. Team with 'T', Market with 'M', Non Accessible with 'NNN'"<<endl;
    
    for (j=0;j<=size[1]-1;j++)
    {
        cout<<"----";
    }
    cout<<endl;
    
    for (i=0;i<size[0];i++)
    {
        for (j=0;j<size[1];j++)
        {
            cout<<"|";
            squares[i][j]->Print();
        }
        cout<<"|";
        cout<<endl;

        for (j=0;j<=size[1]-1;j++)
        {
            cout<<"----";
        }
        
        cout<<endl;
    }
    return;
}

void Grid::quitGame()
{
    int i;
    
    for (i=0; i<heroes_num; i++)
    {
        delete Team[i];
    }
    
    cout<<"Quitting game now..."<<endl;
    return;
}

void Grid::ForceQuit()
{
    DoQuit=true;
    return;
}

void Grid::startGame()
{
    int i,j;
    int h_answ,menu_answer=0;
    char direct[6];
    
    srand((unsigned int) time(NULL));
    
    cout<<"Choose number of heroes(1-3):"<<endl;
    cout<<"Choice: ";
    cin>>heroes_num;
    while(heroes_num<1 || heroes_num>3)
    {
        cout<<"Wrong number, choose number of heroes(1-3):";
        cout<<"Choice: ";
        cin>>heroes_num;
    }
    cout<<endl;
    string line,Name,Type,Str;
    int mP,hP,BS,BD,BA;
    ifstream infile("Names_Stats/Heroes.txt");//Read hero names and stats from this file
    istringstream is;
    int count_lines=0;
    Hero **Heroes_creation_tables;
    
    while (getline(infile,line)) //Count lines in the file
    {
        count_lines++;
    }
    
    count_lines--; //Ignore first line
    
    for (i=0; i<heroes_num; i++)
    {
        cout<<"Choose ";
        switch (i){
            case 0:
                cout<<"1st";
                break;
            case 1:
                cout<<"2nd";
                break;
            case 2:
                cout<<"3rd";
                break;
        }
        cout<<" hero(number):"<<endl;
        
        Heroes_creation_tables=new Hero*[count_lines];
        
        infile.clear();
        infile.seekg(0,ios::beg);
        getline(infile,line);//Consume first line
        
        for (j=0; j<count_lines; j++) //Create Heroes
        {
            getline(infile,Name,'-'); //read name
            getline(infile,Type,'-'); //read kind of hero
            getline(infile,Str,'-'); //read health power
            is.str(Str);  //convert to number
            is.clear();
            is>>hP;
            
            getline(infile,Str,'-'); //read magic power
            is.str(Str);  //convert to number
            is.clear();
            is>>mP;
            
            getline(infile,Str,'-'); //read BaseStrength
            is.str(Str);  //convert to number
            is.clear();
            is>>BS;
            
            getline(infile,Str,'-'); //read BaseDexterity
            is.str(Str);  //convert to number
            is.clear();
            is>>BD;
            
            getline(infile,Str); //read BaseAgility
            is.str(Str);  //convert to number
            is.clear();
            is>>BA;
            
            if (!(Type.compare("Warrior")))
                Heroes_creation_tables[j]=new Warrior(Name,1,hP,mP,BS,BD,BA,5);
            else if (!(Type.compare("Sorcerer")))
                Heroes_creation_tables[j]=new Sorcerer(Name,1,hP,mP,BS,BD,BA,5);
            else if (!(Type.compare("Paladin")))
                Heroes_creation_tables[j]=new Paladin(Name,1,hP,mP,BS,BD,BA,5);
            
            cout<<j+1<<" :";
            Heroes_creation_tables[j]->PrintStats();
        }
        cout<<"Choice: ";
        cin>>h_answ;
        while (h_answ<1 || h_answ>count_lines)//if wrong input
        {
            cout<<"Invalid input. Choose a number between 1 and "
            <<count_lines<<":"<<endl;
            cout<<"Choice: ";
            cin>>h_answ;
        }
        cout<<endl;
        h_answ--;
        Team[i]=Heroes_creation_tables[h_answ];
        
        for (j=0; j<h_answ; j++)
        {
            delete Heroes_creation_tables[j];
        }
        for (j=h_answ+1; j<count_lines; j++)
        {
            delete Heroes_creation_tables[j];
        }
        
        delete[] Heroes_creation_tables;
    }
    
    cout<<endl;
    
    while (menu_answer!=7)//Menu
    {
        
        cout<<"Choose number from menu:"<<endl;
        cout<<"1 :Move"<<endl;
        cout<<"2 :Check Inventory"<<endl;
        cout<<"3 :Equip"<<endl;
        cout<<"4 :Use potion"<<endl;
        cout<<"5 :Display information for the heroes"<<endl;
        cout<<"6 :Display Map"<<endl;
        cout<<"7 :Quit Game"<<endl;

        cout<<"Choice: ";
        if (!( cin>>menu_answer))
        {
            cin.clear();
            cin.ignore(100,'\n');
            menu_answer=0;
        }
        cout<<endl;
        switch (menu_answer){
            case 1:
                cout<<"Choose direction (U/L/D/R):"<<endl;
                cout<<"Choice: ";
                cin>>direct;
                if (direct[0]=='U' || direct[0]=='u')
                    move(UP);
                else if (direct[0]=='L' || direct[0]=='l')
                    move(LEFT);
                else if (direct[0]=='D' || direct[0]=='d')
                    move(DOWN);
                else if (direct[0]=='R' || direct[0]=='r')
                    move(RIGHT);
                else
                    cout<<"Invalid answer, try again!"<<endl;
                if (DoQuit)//If hero quit during battle
                    menu_answer=7;
                cout<<endl;
                break;
                
            case 2:
                cout<<"Choose hero(number) to check Inventory:"<<endl;
                for (i=0; i<heroes_num; i++)
                {
                    cout<<i+1<<" :"<<Team[i]->GetName()<<endl;
                }
                cout<<"Choice: ";
                cin>>h_answ;
                h_answ--;
                if (h_answ>=0 && h_answ<heroes_num)
                    checkInventory(Team[h_answ]);
                cout<<endl;
                break;
                
            case 3:
                cout<<"Choose hero(number) to equip:"<<endl;
                for (i=0; i<heroes_num; i++)
                {
                    cout<<i+1<<" :"<<Team[i]->GetName()<<endl;
                }
                cout<<"Choice: ";
                cin>>h_answ;
                h_answ--;
                if (h_answ>=0 && h_answ<heroes_num)
                    equip(Team[h_answ]);
                cout<<endl;
                break;
                
            case 4:
                cout<<"Choose hero(number) to use potion:"<<endl;
                for (i=0; i<heroes_num; i++)
                {
                    cout<<i+1<<" :"<<Team[i]->GetName()<<endl;
                }
                cout<<"Choice: ";
                cin>>h_answ;
                h_answ--;
                if (h_answ>=0 && h_answ<heroes_num)
                    use(Team[h_answ]);
                cout<<endl;
                break;
                
            case 5:
                displayInfo();
                cout<<endl;
                break;
                
            case 6:
                displayMap();
                cout<<endl;
                break;
                    
            case 7:
                quitGame();
                cout<<endl;
                break;
            default:
                break;
        }
        cout<<endl;
    }
    
    return;
}

Grid::Square::Square(Grid *grid)
{
    this->grid=grid;
}

Grid::Square::~Square(){}

Grid::nonAccessible::nonAccessible(Grid *grid):Square(grid)
{}

bool Grid::nonAccessible::Enter()
{
    cout<<"Team tries entering this square, but it does not seem accessible!"<<endl;
    return false;
}

void Grid::nonAccessible::Exit()
{
    cout<<"Team couldn't enter the square and returns to the previous one"<<endl;
    return;
}

void Grid::nonAccessible::Print() const
{
    cout<<"NNN";
    return;
}

Grid::Accessible::Accessible(Grid *grid): Square(grid)
{
    TeamIsHere=false;
}

Grid::Accessible::~Accessible(){}

bool Grid::Accessible::Enter()
{
    TeamIsHere=true;
    return true;
}

void Grid::Accessible::Exit()
{
    TeamIsHere=false;
    return;
}

Grid::market::market(Grid *grid): Accessible(grid)
{}

void Grid::market::buy(Hero *H)
{
    int answer,answer2;
    string line,Name,Str;
    int Price,MLvl;
    int j;
    string file_name;
    
    cout<<"Choose product(number) that you want to buy:"<<endl;
    cout<<"1 :Weapon"<<endl;
    cout<<"2 :Armor"<<endl;
    cout<<"3 :Potion"<<endl;
    cout<<"4 :Spell"<<endl;
    cout<<"5 :Go back to the initial menu"<<endl;

    cout<<"Choice: ";
    cin>>answer;
    cout<<endl;
    switch (answer)//Choose file to read dependent on user's answer
    {
        case 1:
            cout<<"Choose weapon(number) to buy:"<<endl;
            file_name="Names_Stats/Weapons.txt";
            break;
        case 2:
            cout<<"Choose armor(number) to buy:"<<endl;
            file_name="Names_Stats/Armors.txt";
            break;
        case 3:
            cout<<"Choose potion(number) to buy:"<<endl;
            file_name="Names_Stats/Potions.txt";
            break;
        case 4:
            cout<<"Choose spell(number) to buy:"<<endl;
            file_name="Names_Stats/Spells.txt";
            break;
        default:
            return;
            break;
    }
    ifstream infile(file_name.c_str());
    istringstream is;
    int count_lines=0;
    
    while (getline(infile,line)) //Count lines in the file
    {
        count_lines++;
    }
    
    count_lines--; //Ignore first line
    
    infile.clear();
    infile.seekg(0,ios::beg);
    getline(infile,line);//Consume first line
    
    Weapon **WeaponsMenu=new Weapon*[count_lines];
    Armor **ArmorsMenu=new Armor*[count_lines];
    Potion **PotionsMenu=new Potion*[count_lines];
    Spell **SpellsMenu=new Spell*[count_lines];
    
    for (j=0; j<count_lines; j++) //Create requested menu
    {
        getline(infile,Name,'-'); //read name
        if (answer==1) //weapon
        {
            int Hands,Damage;
            getline(infile,Str,'-'); //read number of hands
            is.str(Str);  //convert to number
            is.clear();
            is>>Hands;
            getline(infile,Str,'-'); //read price
            is.str(Str);  //convert to number
            is.clear();
            is>>Price;
            getline(infile,Str,'-'); //read minimum level
            is.str(Str);  //convert to number
            is.clear();
            is>>MLvl;
            getline(infile,Str); //read damage
            is.str(Str);  //convert to number
            is.clear();
            is>>Damage;
            
            switch (Hands){
                case 1:
                    WeaponsMenu[j]=
                    new Weapon(Name,Price,MLvl,Damage,Weapon::one_hand);
                    break;
                case 2:
                    WeaponsMenu[j]=
                    new Weapon(Name,Price,MLvl,Damage,Weapon::both_hands);
                    break;
            }
            
            cout<<j+1<<" :"<<WeaponsMenu[j]->GetName()<<" with features:"<<endl;
            WeaponsMenu[j]->DisplayInfo();
        }
        else if (answer==2)//armor
        {
            int DamRed;
            getline(infile,Str,'-'); //read price
            is.str(Str);  //convert to number
            is.clear();
            is>>Price;
            getline(infile,Str,'-'); //read minimum level
            is.str(Str);  //convert to number
            is.clear();
            is>>MLvl;
            getline(infile,Str); //read damage reduction
            is.str(Str);  //convert to number
            is.clear();
            is>>DamRed;
            
            ArmorsMenu[j]=new Armor(Name,Price,MLvl,DamRed);
            
            cout<<j+1<<" :"<<ArmorsMenu[j]->GetName()<<" with features:"<<endl;
            ArmorsMenu[j]->DisplayInfo();
        }
        else if (answer==3)//potion
        {
            string Type;
            int Inc;
            getline(infile,Type,'-'); //read type
            getline(infile,Str,'-'); //read price
            is.str(Str);  //convert to number
            is.clear();
            is>>Price;
            getline(infile,Str,'-'); //read minimum level
            is.str(Str);  //convert to number
            is.clear();
            is>>MLvl;
            getline(infile,Str); //read increase
            is.str(Str);  //convert to number
            is.clear();
            is>>Inc;
            
            if (!(Type.compare("strength")))
                PotionsMenu[j]=new Potion(Name,Price,MLvl,Potion::strength,Inc);
            else if (!(Type.compare("dexterity")))
                PotionsMenu[j]=new Potion(Name,Price,MLvl,Potion::dexterity,Inc);
            else if (!(Type.compare("agility")))
                PotionsMenu[j]=new Potion(Name,Price,MLvl,Potion::agility,Inc);
            
            cout<<j+1<<" :"<<PotionsMenu[j]->GetName()<<" with features:"<<endl;
            PotionsMenu[j]->DisplayInfo();
        }
        else if (answer==4)//spell
        {
            string Type;
            int MinDam,MaxDam,EnReq,RedP;
            getline(infile,Type,'-'); //read type
            getline(infile,Str,'-'); //read price
            is.str(Str);  //convert to number
            is.clear();
            is>>Price;
            getline(infile,Str,'-'); //read minimum level
            is.str(Str);  //convert to number
            is.clear();
            is>>MLvl;
            getline(infile,Str,'-'); //read minimum gamage
            is.str(Str);  //convert to number
            is.clear();
            is>>MinDam;
            getline(infile,Str,'-'); //read maximum damage
            is.str(Str);  //convert to number
            is.clear();
            is>>MaxDam;
            getline(infile,Str,'-'); //read energy requirement
            is.str(Str);  //convert to number
            is.clear();
            is>>EnReq;
            getline(infile,Str); //read reduction percent
            is.str(Str);  //convert to number
            is.clear();
            is>>RedP;
            
            if (!(Type.compare("IceSpell")))
                SpellsMenu[j]=new IceSpell(Name,Price,MLvl,MinDam,MaxDam,EnReq,RedP);
            else if (!(Type.compare("FireSpell")))
                SpellsMenu[j]=new FireSpell(Name,Price,MLvl,MinDam,MaxDam,EnReq,RedP);
            else if (!(Type.compare("LightingSpell")))
                SpellsMenu[j]=new LightingSpell(Name,Price,MLvl,MinDam,MaxDam,EnReq,RedP);
            
            cout<<j+1<<" :"<<SpellsMenu[j]->GetName()<<" with features:"<<endl;
            SpellsMenu[j]->DisplayInfo();
        }
    }
    
    cout<<"Choice(Enter 0 to return to the initial menu): ";
    cin>>answer2;

    while (answer2<0 || answer2>count_lines)//if input is invalid
    {
        cout<<"Invalid input. Choose a number between 0 and "
        <<count_lines<<":"<<endl;
        cout<<"Choice(Enter 0 to return to the initial menu): ";
        cin>>answer2;
    }
    cout<<endl;
    answer2--;
    
    switch (answer)
    {
        case 1:
            if (answer2!=-1)
                H->GetHWeaponsP()->BuyWeapon(WeaponsMenu[answer2]);
            for (j=0; j<count_lines; j++){
                delete WeaponsMenu[j];
            }
            break;
        case 2:
            if (answer2!=-1)
                H->GetHArmorsP()->BuyArmor(ArmorsMenu[answer2]);
            for (j=0; j<count_lines; j++){
                delete ArmorsMenu[j];
            }
            break;
        case 3:
            if (answer2!=-1)
                H->GetHPotionsP()->BuyPotion(PotionsMenu[answer2]);
            for (j=0; j<count_lines; j++){
                delete PotionsMenu[j];
            }
            break;
        case 4:
            if (answer2!=-1)
                H->GetHSpellsP()->BuySpell(SpellsMenu[answer2]);
            for (j=0; j<count_lines; j++){
                delete SpellsMenu[j];
            }
            break;
    }
    
    delete[] WeaponsMenu;
    delete[] ArmorsMenu;
    delete[] PotionsMenu;
    delete[] SpellsMenu;
    
    return;
}

void Grid::market::sell(Hero *H)
{
    int answer,answer2;
    bool NoProduct=false;
    
    cout<<"Choose product(number) that you want to sell:"<<endl;
    cout<<"1 :Weapon"<<endl;
    cout<<"2 :Armor"<<endl;
    cout<<"3 :Potion"<<endl;
    cout<<"4 :Spell"<<endl;
    cout<<"5 :Go back to the initial menu"<<endl;

    cout<<"Choice: ";
    cin>>answer;
    cout<<endl;
    
    switch (answer)
    {
        case 1:
            cout<<"Choose weapon(number) to sell:"<<endl;
            NoProduct=!(H->GetHWeaponsP()->DisplayProducts());
            break;
        case 2:
            cout<<"Choose armor(number) to sell:"<<endl;
            NoProduct=!(H->GetHArmorsP()->DisplayProducts());
            break;
        case 3:
            cout<<"Choose potion(number) to sell:"<<endl;
            NoProduct=!(H->GetHPotionsP()->DisplayProducts());
            break;
        case 4:
            cout<<"Choose spell(number) to sell:"<<endl;
            NoProduct=!(H->GetHSpellsP()->DisplayProducts());
            break;
        default:
            return;
            break;
    }

    if (NoProduct) //If there no such product hold by hero
        return;
    
    cout<<"Choice(Enter 0 to return to the initial menu): ";
    cin>>answer2;
    cout<<endl;
    if (answer2==0)
        return;
    
    answer2--;
    
    switch (answer)
    {
        case 1:
            H->GetHWeaponsP()->SellWeapon(answer2);
            break;
        case 2:
            H->GetHArmorsP()->SellArmor(answer2);
            break;
        case 3:
            H->GetHPotionsP()->SellPotion(answer2);
            break;
        case 4:
            H->GetHSpellsP()->SellSpell(answer2);
            break;
        default:
            break;
    }
    return;
}

bool Grid::market::Enter()
{
    int answer=0,answer_h,i;
    
    Grid::Accessible::Enter(); //Enter square
    
    cout<<"Team entered market"<<endl;
    while (answer!=3)
    {
        cout<<"Choose a transaction(number):"<<endl;
        cout<<"1 :Buy"<<endl;
        cout<<"2 :Sell"<<endl;
        cout<<"3 :Quit market"<<endl;
        cout<<"Choice: ";
        cin>>answer;
        cout<<endl;
        if (answer==1 || answer==2)
        {
            cout<<"Choose a hero(number):"<<endl;
            for (i=0;i<grid->heroes_num;i++)
            {
                cout<<i+1<<" :"<<grid->Team[i]->GetName()<<endl;
            }
            cout<<"Choice: ";
            cin>>answer_h;
            cout<<endl;
            answer_h--;
            if (answer_h<3 && answer_h>=0 && grid->Team[answer_h]!=NULL)
            {
                switch (answer){
                    case 1:
                        buy(grid->Team[answer_h]);
                        break;
                    case 2:
                        sell(grid->Team[answer_h]);
                        break;
                }
            }
            else
            {
                cout<<"Invalid hero, try again!"<<endl;
            }
        }
        else if (answer!=3)
        {
            cout<<"Invalid answer, try again!"<<endl;
        }
        cout<<endl;
    }
    
    return true;
}

void Grid::market::Print() const
{
    if (TeamIsHere)
        cout<<"T M";
    else
        cout<<" M ";
    return;
}

Grid::common::common(Grid *grid, bool TeamIsHere):Accessible(grid)
{
    this->TeamIsHere=TeamIsHere;
}

bool Grid::common::Enter()
{
    Grid::Accessible::Enter(); //Enter square
    
    srand((unsigned int) time(NULL));
    
    if ((rand()%10)<3) //30% for battle 
    {
        Battle B(grid->Team[0],grid->Team[1],grid->Team[2]);
        B.startBattle();
    }

    return true;
}

void Grid::common::Print() const
{
    if (TeamIsHere)
        cout<<" T ";
    else
        cout<<"   ";
    return;
}
