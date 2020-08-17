//
//  dungeon.hpp
//  proj3
//
//  Created by Megan Cho on 5/17/20.
//  Copyright © 2020 Yuna Cho. All rights reserved.
//

  
#ifndef DUNGEON_INCLUDED
#define DUNGEON_INCLUDED

#include "actors.h" // Access to player + monsters
#include <list>
#include <vector>

// Struct to create rooms
struct room{
    int sr; // Starting x coordinate
    int sc; // Starting y coordinate
    int length;
    int width;
};

// Dungeon accesses actors + objects
// Creates grid template
class dungeon
{
public:
    // Constructor takes in goblin smell distance
    dungeon(int smell);
    ~dungeon();
    
    // Accessors
    player* getPlayer() const;
    monster* getMonster(int r, int c);  // Get singular monster
    list<monster*>& getMonsters();   // List of all monsters
    list<object*>& getObjects();    // List of all objects
    object* getStair() const;
    object* getGoldenIdol() const;
    vector<string>& getMessages();    // Get list of message strings
    int getGoblinSmell() const;
    int getLevel() const;
    char getGrid(int r, int c) const;
    
    // Mutators
    bool isMovable(int r, int c);   // Is spot movable for actors?
    bool isMonster(int r, int c) const; // Check to identify monster
    bool isPlayer(int r, int c) const;  // Check to identify player
    bool isStair(int r, int c) const;   // Check to identify stairs
    bool isGoldenIdol(int r, int c) const;  // Check to identify golden idol
    void setPlayer();   // Set player position
    void addPlayer(player *p);  // Add player to map
    void setMonster();  // Set monster position
    void addMonster();  // Add monster to map
    void deleteMonster();   // Delete dead monsters
    void addObject();   // Add object to map
    void setObject();   // Set object position
    void setSymbol(int r, int c, char symb);    // Show symbol of actor when moving
    void display(); // Display grid
    void deleteDungeon(dungeon* d);
    
private:
    void createMap(int m_nRooms);   // Create dungeon
    int m_nRooms;    // Number of rooms
    int m_level;    // Level in map
    char m_grid[18][70];    // Dungeon dimensions
    int m_goblinSmell;  // Goblin smell
    int m_nObjects; // Number of objects
    
    player* m_player;
    list<monster*> m_monsters;  // List of pointers to monsters in map
    list<object*> m_objects;    // List of pointers to objects in map
    object* m_goldenIdol;
    object* m_stair;
    vector<string> m_messages;
};

// Helper function
bool roomCompare(room r1, room r2);

#endif // defined DUNGEON_INCLUDED
