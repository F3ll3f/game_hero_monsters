#ifndef FILE_BATTLE_H_INCLUDED
#define FILE_BATTLE_H_INCLUDED

#include <vector>
#include "living.h"
#include "product.h"


class Battle{//A battle between a team of heroes and some monsters
private:
    class SpellEffect{ //Keeps information about the used spells
    public:
        Spell *spell; //Spell Used
        int rounds_left;//Remaining rounds for spell to end its effect
        Monster *monster;//Inflicted monster
    };
    
    Hero *heroes[3]; //heroes in battle
    int heroes_num; //number of heroes
    Monster *monsters[5]; //monsters in battle (values 1-5)
    int monsters_num; //number of monsters
    std::vector <SpellEffect> SpellsInEffect;/*Keeps track of the spells which currently
                                         effect monsters*/
    
    bool MonstersAndHeroesAreAlive() const;/*Keep track if at least one hero and
                                      one monster are alive*/
    void SpellsEndOfRound(); /*Reduce a round for all spells and stop the effects
                              for finished spells*/
    
    bool attack(Hero *hero);
    bool castSpell(Hero *hero);
    bool use(Hero *hero);
    bool change(Hero *hero); //change armor or weapon
    void displayStats() const;
public:
    Battle(Hero *hero1,Hero *hero2=NULL,Hero *hero3=NULL);
    ~Battle();
    
    bool startBattle(); //Return false if player chooses to quit battle and game
};

#endif
