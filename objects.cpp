//
//  objects.cpp
//  proj3
//
//  Created by Megan Cho on 5/17/20.
//  Copyright © 2020 Yuna Cho. All rights reserved.
//
#include "objects.h"

// Default constructor
object::object(){};

object::object(string name, string action){
    //cout << "Object constructor 1" << endl;
    m_name = name;
    m_action = action;
}

object::object(string name, int row, int col){
    //cout << "Object constructor 2" << endl;
    m_name = name;
    m_row = row;
    m_col = col;
}

object::object(string name, string action, int row, int col){
    //cout << "Object constructor 3" << endl;
    m_name = name;
    m_action = action;
    m_row = row;
    m_col = col;
}

// Object accessors

string object::getName() const{
    return m_name;
}

string object::getAction() const{
    return m_action;
}

int object::getRow() const{
    return m_row;
}

int object::getCol() const{
    return m_col;
}

// Object's set position
void object::setPos(int r, int c){
    m_row = r;
    m_col = c;
}

///////////////////////////////////////////////////////////////////////////////////////////////////// Weapon Implementation

weapon::weapon(string name, string action, int dpBonus, int damage):object(name, action){
    //cout << "Weapon constructor 1" << endl;
    // get name & action from object creation
    m_dpBonus = dpBonus;
    m_damage = damage;
}

weapon::weapon(string name, string action, int dpBonus, int damage, int row, int col):object(name, action, row, col){
    //cout << "Weapon constructor 2" << endl;
    // get name, action, row, col from object creation
    m_dpBonus = dpBonus;
    m_damage = damage;
}

int weapon::getdpBonus() const{
    return m_dpBonus;
}

int weapon::getDamage() const{
    return m_damage;
}

mace::mace():weapon("mace", "swings", 0 , 2){}
shortSword::shortSword():weapon("short sword", "slashes", 0, 2){}
longSword::longSword():weapon("long sword", "swings", 2, 4){}
magicAxe::magicAxe():weapon("magic axe", "chops", 5, 5){}
magicAxe::magicAxe(int row, int col):weapon("magic axe", "chops", 5, 5, row, col){}
magicFang::magicFang():weapon("magic fangs of sleep", "strikes", 3, 2){}
magicFang::magicFang(int row, int col):weapon("magic fangs of sleep", "strikes", 3, 2, row, col){}

///////////////////////////////////////////////////////////////////////////////////////////////////// Scroll Implementation

scroll::scroll(string name, string effect):object(name, effect){}
scroll::scroll(string name, string effect, int row, int col):object(name, effect, row, col){}

teleportScroll::teleportScroll():scroll("scroll of teleportation", "You feel your body wrenched in space and time."){};
teleportScroll::teleportScroll(int row, int col):scroll("scroll of teleportation", "You feel your body wrenched in space and time.", row, col){};
armorScroll::armorScroll():scroll("scroll of improve armor", "Your armor glows blue."){}
armorScroll::armorScroll(int row, int col):scroll("scroll of improve armor", "Your armor glows blue.", row, col){}
strengthScroll::strengthScroll():scroll("scroll of raise strength", "Your muscles bulge."){};
strengthScroll::strengthScroll(int row, int col):scroll("scroll of raise strength", "Your muscles bulge.", row, col){};
healthScroll::healthScroll():scroll("scroll of enhance health", "You feel your heart beating stronger."){}
healthScroll::healthScroll(int row, int col):scroll("scroll of enhance health", "You feel your heart beating stronger.", row, col){}
dexterityScroll::dexterityScroll():scroll("scroll of enhance dexterity", "You feel like less of a klutz."){}
dexterityScroll::dexterityScroll(int row, int col):scroll("scroll of enhance dexterity", "You feel like less of a klutz.", row, col){}
