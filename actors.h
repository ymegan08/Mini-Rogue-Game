//
//  actors.h
//  proj3
//
//  Created by Megan Cho on 5/17/20.
//  Copyright © 2020 Yuna Cho. All rights reserved.
//

#ifndef ACTORS_INCLUDED
#define ACTORS_INCLUDED

#include <list>
#include <vector>
#include "objects.h"
#include <queue>
using namespace std;

class dungeon;  // Dungeon accesses actors

class actor{
public:
    actor(dungeon* d, int row, int col);
    actor(dungeon* d, int hp, int maxhp, int ap, int sp, int dp, int st, string name);
    virtual ~actor();
    
    // Accessors
    int getRow() const;
    int getCol() const;
    int getHp() const;
    int getMaxHp() const;
    int getAp() const;
    int getSp() const;
    int getDp() const;
    int getSt() const;
    string getName() const;
    dungeon* getDungeon() const;
    weapon* getWeapon() const;  // Acccess actor's weapon
    bool isDead() const;
    
    // Mutators
    void changeMaxHp(int h);    // Change maximum health points
    void changeSt(int s);    // Change sleep time
    void changeHp(int h);   // Change health points
    void changeSp(int s);   // Change strength points
    void changeAp(int a);   // Change armor points
    void changeDp(int d);   // Change dexterity points
    
    void move(char dir);    // All actors can move
    bool setPos(int r, int c);  // Dungeon will set position of actors
    void setWeapon(weapon* w);   // Set actor weapons
    void attack(actor* attacker, actor* defender);  // Actors attack
    
private:
    dungeon* m_dungeon;
    weapon* m_weapon;
    int m_row;
    int m_col;
    int m_hp;
    int max_hp;
    int m_ap;
    int m_sp;
    int m_dp;
    int m_st;
    bool m_dead;
    string m_name;
};

// Player class derived from actor base class
class player: public actor{
public:
    player(dungeon* d);
    virtual ~player();
    
    // Accessors
    list<object*> getInventory() const; // Get inventory list
    bool didWin() const;    // Check if player won
    
    // Mutators
    void playerMove(char dir);  // Take in player move direction from game.cpp
                                // Player move & attack
    void cheat();   // Cheat mode
    void randomAddHp(); // 1 in 10 chance of regaining 1 hp up to maxhp
    void seeInventory();   // See inventory
    void pickUpObj();   // Pick up an object
    void readScroll(char key);  // Read scroll
    void wieldWeapon(char key); // Wield weapon
    bool isObjThere();  // Check if object is there
    
private:
    // Checks if player has won
    bool m_win;
    // Access to list of inventory
    list<object*> m_inventory;
};

// Monster class derived from actor base class
class monster: public actor{
public:
    monster(dungeon* d, int hp, int maxhp, int ap, int sp, int dp, int st, string name);
    virtual ~monster(){};
    
    virtual void doSomething(dungeon* d) = 0; // Move & attack
    bool isInRange(dungeon* d, int dist);   // Check if player is in range
    void drop(dungeon* d, monster* m);  // Monsters might drop items when they die
    char chooseDir(dungeon* d, int row, int col);   // Choose direction to move in
};

// Bogeyman class
class bogeyman: public monster{
public:
    bogeyman(dungeon* d);
    
    virtual ~bogeyman(){};
    virtual void doSomething(dungeon* d);
};

// Snakewoman class
class snakewoman: public monster{
public:
    snakewoman(dungeon* d);
    
    virtual ~snakewoman(){};
    virtual void doSomething(dungeon* d);
};

// Dragon class
class dragon: public monster{
public:
    dragon(dungeon* d);
    virtual ~dragon(){};
    
    virtual void doSomething(dungeon* d);
    void recover();
};

// Goblin class
class goblin: public monster{
public:
    goblin(dungeon* d);
    virtual ~goblin(){};

    void setDistance(); // Set goblin's smell distance from game.cpp
    
    virtual void doSomething(dungeon* d);
    bool goblinSmell(int r, int c, int distance);   // Recursive function
    bool isGobMovable(int r, int c);    // Can goblin move
    
private:
    int g_smellDistance;    // Goblin's smell distance
    queue<char> dir;  // Direction queue for goblin's movement
};

#endif // defined ACTORS_INCLUDED
