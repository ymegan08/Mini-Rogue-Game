//
//  Game.cpp
//  proj3
//
//  Created by Megan Cho on 5/17/20.
//  Copyright © 2020 Yuna Cho. All rights reserved.
//

#include "Game.h"
#include "utilities.h"
#include "dungeon.h"    // Interacts with dungeon
#include <iostream>
using namespace std;

// Implement these and other Game member functions you may have added.

// Game constructor
Game::Game(int goblinSmellDistance)
{
    // Set goblin's smell distance
    n_goblinSmellDistance = goblinSmellDistance;
    // Point to new dungeon
    map = new dungeon(goblinSmellDistance); // Pass goblin smell dist. to dungeon
    map->addObject();   // Add objects to map
    map->addMonster();  // Add monsters to dungeon
    // Point to new player
    gPlayer = new player(map);
    // Add player into dungeon
    map->addPlayer(gPlayer);
}

// Game destructor
Game::~Game()
{
    //cout << "Game destructor called" << endl;
    // Delete dungeon pointer
    delete map;
}

// Play game
void Game::play()
{
    // Pointer p to player on the map
    player* p = map->getPlayer();
    char c; // getCharacter to move player
    do{
        map->display();
        c = getCharacter();
        switch(c){
            // Quit game
            case 'q':
                exit(0);
            // Cheat mode
            case 'c':
                p->cheat();
                break;
            // Pick up an item
            case 'g':
                p->pickUpObj();
                break;
            // See inventory
            case 'i':
            {
                p->seeInventory();
                getCharacter(); // Press anything to exit inventory
                break;
            }
            // Read scroll
            case 'r':
            {
                p->seeInventory();
                char selected = getCharacter();
                p->readScroll(selected);
                break;
            }
            // Wield weapon
            case 'w':
            {
                p->seeInventory();
                char selected = getCharacter();
                p->wieldWeapon(selected);
                break;
            }
            case '>':
            {
                // Check if user is on top of stairs
                // If true, delete current dungeon & create new one
                if(map->isStair(map->getPlayer()->getRow(), map->getPlayer()->getCol()))
                {
                    map->deleteDungeon(map);
                }
                break;
            }
            // Moving directions
            case ARROW_LEFT:
            case ARROW_RIGHT:
            case ARROW_UP:
            case ARROW_DOWN:
                p->playerMove(c);
            // If other input, only monsters move
            default:
                break;
        }
        // Monster moves with player move
        for (list<monster*>::iterator i = map->getMonsters().begin(); i != map->getMonsters().end(); i++){
            //cout << (*i)->getName() << "'s turn!"<< endl;
            (*i)->doSomething(map);
        }
    }
    while(!(p->didWin()));
}
