//
//  objects.hpp
//  proj3
//
//  Created by Megan Cho on 5/17/20.
//  Copyright © 2020 Yuna Cho. All rights reserved.
//

#ifndef OBJECTS_INCLUDED
#define OBJECTS_INCLUDED

#include <string>
using namespace std;

class dungeon;  // Dungeon accesses objects
class player;   // Player accesses objects

class object{
public:
    object();   // For golden idol & stairs
    object(string name, string action); // Create weapon
    object(string name, string action, int row, int col);   // To set weapon on map
    object(string name, int row, int col);  // To set object on map
    virtual ~object(){}; // Virtual destructor so dynamically created derived class objects can delete themselves
    // Accessors
    string getName() const;
    string getAction() const;
    int getRow() const;
    int getCol() const;

    // Mutator
    void setPos(int r, int c);
private:
    string m_name;
    string m_action;
    int m_row;
    int m_col;
};

// Weapon derived from object
class weapon:public object{
public:
    weapon(string name, string action, int dpBonus, int damage);
    weapon(string name, string action, int dpBonus, int damage, int row, int col);
    virtual ~weapon(){};
    
    // Accessors
    int getdpBonus() const;
    int getDamage() const;
private:
    int m_dpBonus;  // Dexterity bonus; whether attacker hits victim
    int m_damage;   // Damage amount to victim who is hit
};

// Scroll derived from object
class scroll:public object{
public:
    // Effects taken into account within dungeon.cpp when user
    // reads scroll
    scroll(string name, string effect);
    scroll(string name, string effect, int row, int col);
    virtual ~scroll(){};
};

///////////////////////////////////////////////////////////////////////////////////////////// Weapon objects

class mace:public weapon{
public:
    mace();
    virtual ~mace(){};
};

class shortSword:public weapon{
public:
    shortSword();
    virtual ~shortSword(){};
};

class longSword:public weapon{
public:
    longSword();
    virtual ~longSword(){};
};

class magicAxe:public weapon{
public:
    magicAxe();
    // Cannot be randomly placed, must kill monster for it
    magicAxe(int row, int col);
    virtual ~magicAxe(){};
};

class magicFang:public weapon{
public:
    magicFang();
    // Cannot be randomly placed, must kill monster for it
    magicFang(int row, int col);
    virtual ~magicFang(){};
};

///////////////////////////////////////////////////////////////////////////////////////////// Scroll objects

class teleportScroll:public scroll{
public:
    // For adding object to dungeon's object list
    teleportScroll();
    // Cannot be randomly placed, must kill monster for it
    teleportScroll(int row, int col);
    virtual ~teleportScroll(){};
};

class armorScroll:public scroll{
public:
    // For adding object to dungeon's object list
    armorScroll();
    // For placing scroll
    armorScroll(int row, int col);
    virtual ~armorScroll(){};
};

class strengthScroll:public scroll{
public:
    // For adding object to dungeon's object list
    strengthScroll();
    // For placing scroll
    strengthScroll(int row, int col);
    virtual ~strengthScroll(){};
};

class healthScroll:public scroll{
public:
    // For adding object to dungeon's object list
    healthScroll();
    // For placing scroll
    healthScroll(int row, int col);
    virtual ~healthScroll(){};
};

class dexterityScroll:public scroll{
public:
    // For adding object to dungeon's object list
    dexterityScroll();
    // For placing scroll
    dexterityScroll(int row, int col);
    virtual ~dexterityScroll(){};
};

#endif
