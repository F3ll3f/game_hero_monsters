#ifndef FILE_GRID_H_INCLUDED
#define FILE_GRID_H_INCLUDED

#include "living.h"
#include "product.h"


class Grid{
private:
    class Square;
    class nonAccessible; //derived class of Square
    class Accessible; //derived class of Square
    class market; //derived class of Accessible
    class common; //derived class of Accessible

    enum direction{
        UP=1,
        RIGHT=2,
        DOWN=3,
        LEFT=4
    };
    
    int size[2]; //Grid has "size[0]" lines and "size[1]" columns
    Square ***squares; //The squares of the grid(A matrix of pointers to squares)
    int position[2]; //position of the team in the grid
    Hero *Team[3]; //The team of heroes. If there are less than 3 heroes,
                   //the rest of the array has NULL.
    int heroes_num;//number of heroes
    
    static bool DoQuit; //Shows if player chose to quit game.
                        //Used to quit during a battle
    void move(direction d);
    void checkInventory(Hero *H) const;
    void equip(Hero *H);
    void use(Hero *H);
    void displayInfo() const;
    void displayMap() const;
    void quitGame();
    
public:
    Grid(bool CreateMap=false,char **Map=NULL, int rows=0, int columns=0);/*Map is
    a rowsXcolumns matrix where the element (i,j) is:'N' for nonAccesible squares
                                                     :'C' for common squares
                                                     :'M' for market squares
    If CreateMap variable is true, user creates a map.
    If a map is not provided and CreateMap variable is false, a default map
    is selected.
    Team starts at (0,0), so this square must be accesible*/
    ~Grid();
    
    static void ForceQuit(); //Change variable DoQuit to true
    
    void startGame();//Start the game with a number of heroes selected
                     //by the player
};

class Grid::Square{
public:
    Grid *grid;
    
    Square(Grid *grid);
    virtual ~Square()=0;
    virtual bool Enter()=0;
    virtual void Exit()=0;
    virtual void Print() const=0;
};
class Grid::nonAccessible: public Grid::Square{
public:
    nonAccessible(Grid *grid);
    
    bool Enter(); //Returns false and prints that access is not possible
    void Exit();  //Prints that the team returns to the previous square
    void Print() const; //Print "NNN"
};
class Grid::Accessible: public Grid::Square{
public:
    bool TeamIsHere;
    
    Accessible(Grid *grid);
    virtual ~Accessible()=0;
    
    virtual bool Enter();
    void Exit();
    virtual void Print() const=0;
};
class Grid::market: public Grid::Accessible{
public:
    market(Grid *grid);
    
    void buy(Hero *H);  //buy a product for Hero H
    void sell(Hero *H); //sell a product from Hero H
    bool Enter();
    void Print() const; //Print " M " or "T M"
};
class Grid::common: public Grid::Accessible{
public:
    common(Grid *grid, bool TeamIsHere=false);
    
    bool Enter();
    void Print() const; //Print "   " or " T "
};

#endif
