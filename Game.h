//
//  Game.h
//  proj3
//
//  Created by Megan Cho on 5/17/20.
//  Copyright © 2020 Yuna Cho. All rights reserved.
//

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

// You may add data members and other member functions to this class.

#include "dungeon.h"    // Using dungeon pointer
#include "actors.h"     // Using actors pointer

class Game
{
public:
    Game(int goblinSmellDistance);
    ~Game();
    void play();
private:
    dungeon* map;   // Pointer to dungeon
    player* gPlayer;    // Pointer to player
    int n_goblinSmellDistance;  // Goblin smell distance
};

#endif // GAME_INCLUDED
