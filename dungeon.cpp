//
//  dungeon.cpp
//  proj3
//
//  Created by Megan Cho on 5/17/20.
//  Copyright © 2020 Yuna Cho. All rights reserved.
//

#include "dungeon.h"
#include <iostream>
#include <algorithm>
#include <string>
#include "utilities.h"
#include "objects.h"
#include "actors.h"
using namespace std;

// Dungeon constructor
dungeon::dungeon(int smell){
    //cout << "New dungeon constructed" << endl;
    m_goblinSmell = smell;
    m_nRooms = randInt(3, 5);    // Set random number of rooms
    createMap(m_nRooms);        // Creates map with rooms
    m_nObjects = randInt(2,3);  // Set random number of objects
    m_level = 0;    // Start at level 0
}

// Dungeon destructor
dungeon::~dungeon(){
    //cout << "Dungeon destructor called" << endl;
    // Delete dungeon when game ends
    delete m_goldenIdol;
    // Delete object list
    for (list<object*>::iterator i = m_objects.begin(); i != m_objects.end(); i++){
        delete *i;
    }
    m_objects.clear();
    // Delete monster list
    for (list<monster*>::iterator i = m_monsters.begin(); i != m_monsters.end(); i++){
        delete *i;
    }
    m_monsters.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////  Accessor implementations

player* dungeon::getPlayer() const{
    return m_player;
}

// Loop through list of monsters until match
monster* dungeon::getMonster(int r, int c){
    for(list<monster*>::iterator i = m_monsters.begin(); i != m_monsters.end(); i++){
        if((*(*i)).getRow() == r && (*(*i)).getCol() == c){
            return *i;
        }
    }
    return nullptr; // No monster match
}

// Get all monsters
list<monster*>& dungeon:: getMonsters(){
    return m_monsters;
}

// Get stair
object* dungeon::getStair() const{
    return m_stair;
}

// Get golden idol
object* dungeon::getGoldenIdol() const{
    return m_goldenIdol;
}

// Get all objects
list<object*>& dungeon::getObjects(){
    return m_objects;
}

// Get string of all messages
vector<string>& dungeon::getMessages(){
    return m_messages;
}

int dungeon::getGoblinSmell() const{
    return m_goblinSmell;
}

int dungeon::getLevel() const{
    return m_level;
}

char dungeon::getGrid(int r, int c) const{
    return m_grid[r][c];
}

////////////////////////////////////////////////////////////////////////////////////////////  Mutator implementations

bool dungeon::isPlayer(int r, int c) const{
    if(m_grid[r][c] == '@' || (r == m_player->getRow() && c == m_player->getCol())){
        return true;
    }
    return false;
}

// Set player in random place of dungeon
void dungeon::setPlayer(){
    int playerRow;
    int playerCol;
    // Keep finding random coordinates until a valid spot is found
    do{
        playerRow = randInt(1, 17);
        playerCol = randInt(1, 69);
    }
    while(getGrid(playerRow, playerCol) == '#' ||
          getGrid(playerRow, playerCol) == 'B' ||
          getGrid(playerRow, playerCol) == 'S' ||
          getGrid(playerRow, playerCol) == 'G' ||
          getGrid(playerRow, playerCol) == 'D');
    // Set player position to valid spot
    m_player->setPos(playerRow, playerCol);
    // cout << playerRow << playerCol << endl;
}

// Add player to dungeon
void dungeon::addPlayer(player *p){
    m_player = p;
    setPlayer();
}

bool dungeon::isMonster(int r, int c) const{
    if(m_grid[r][c] == 'B' ||
       m_grid[r][c] == 'S' ||
       m_grid[r][c] == 'G' ||
       m_grid[r][c] == 'D'){
        return true;
    }
    return false;
}

// Set monster in random place of the dungeon
void dungeon::setMonster(){
    // Iterate through monster list in dungeon
    for(list<monster*>::iterator i = m_monsters.begin(); i != m_monsters.end(); i++){
        int monsterRow;
        int monsterCol;
        // Iterate through monster list and place on random valid positions
        do{
            monsterRow = randInt(1, 17);
            monsterCol = randInt(1, 69);
        }while(getGrid(monsterRow, monsterCol) == 'B' ||
               getGrid(monsterRow, monsterCol) == 'S' ||
               getGrid(monsterRow, monsterCol) == 'G' ||
               getGrid(monsterRow, monsterCol) == 'D' ||
               getGrid(monsterRow, monsterCol) == '#' ||
               getGrid(monsterRow, monsterCol) == '@');
        // Set monster position to valid spot
        (*(*i)).setPos(monsterRow, monsterCol);
        //cout << "Monster: " << (*(*i)).getName() << " Row: " << monsterRow << " Col: " << monsterCol << endl;
        // Connect names with symbols
        if((*(*i)).getName() == "Bogeyman"){
            m_grid[monsterRow][monsterCol] = 'B';
        }
        else if((*(*i)).getName() == "Goblin"){
            m_grid[monsterRow][monsterCol] = 'G';
        }
        else if((*(*i)).getName() == "Snakewoman"){
            m_grid[monsterRow][monsterCol] = 'S';
        }
        else if((*(*i)).getName() == "Dragon"){
            m_grid[monsterRow][monsterCol] = 'D';
        }
    }
}

// Add monster to dungeon
void dungeon::addMonster(){
    //cout << "Monsters added" << endl;
    int numMonsters = randInt(2, 5*(m_level+1)+1);  // Random # of monsters to place depending on level
    for(int i = 0; i < numMonsters; i++){
        // Goblins & Snakewomen in all levels
        if(m_level == 0 || m_level == 1){
            int random = randInt(2);
            switch(random) {
                case 0:
                    //this: refers to the dungeon level it's located in
                    m_monsters.push_back(new goblin(this));
                    break;
                case 1:
                    m_monsters.push_back(new snakewoman(this));
                    break;
                default:
                    break;
            }
        }
        // Bogeymen level 2 or deeper
        if(m_level == 2){
            int random = randInt(3);
            switch(random){
                case 0:
                    m_monsters.push_back(new goblin(this));
                    break;
                case 1:
                    m_monsters.push_back(new snakewoman(this));
                    break;
                case 2:
                    m_monsters.push_back(new bogeyman(this));
                    break;
                default:
                    break;
            }
        }
        // Dragons level 3 or deeper
        if(m_level > 2){
            int random = randInt(4);
            switch(random){
                case 0:
                    m_monsters.push_back(new goblin(this));
                    break;
                case 1:
                    m_monsters.push_back(new snakewoman(this));
                    break;
                case 2:
                    m_monsters.push_back(new bogeyman(this));
                    break;
                case 3:
                    m_monsters.push_back(new dragon(this));
                    break;
                default:
                    break;
            }
        }
    }
    setMonster();
}

// Clear monsters when they die
void dungeon::deleteMonster(){
    for(list<monster*>::iterator i = m_monsters.begin(); i != m_monsters.end();){
        // Delete monsters from list and dungeon
        if((*i)->isDead() == true){
            m_grid[(*i)->getRow()][(*i)->getCol()] = ' ';
            // Erases pointers
            delete(*i);
            i = m_monsters.erase(i);
        }
        // Iterate until dead monster
        else{
            i++;
        }
    }
}

// Check if specific location is a stair, called in game.cpp '>'
bool dungeon::isStair(int r, int c) const{
    int stairRow = getStair()->getRow();
    int stairCol = getStair()->getCol();
    if(r == stairRow && c == stairCol)
        return true;
    else
        return false;
}

// Check if specific location is a golden idol
bool dungeon::isGoldenIdol(int r, int c) const{
    if(m_grid[r][c] == '&'){
        return true;
    }
    else
        return false;
}

// Set objects to level
void dungeon::setObject(){
    // Add objects from list
    for(list<object*>::iterator i = m_objects.begin(); i != m_objects.end(); i++){
        int objectRow;
        int objectCol;
        do{
            objectRow = randInt(1, 17);
            objectCol = randInt(1, 69);
        } // Find random valid positions to place objects
        while(getGrid(objectRow, objectCol) == '#' ||
              getGrid(objectRow, objectCol) == '>' ||
              getGrid(objectRow, objectCol) == '&' ||
              getGrid(objectRow, objectCol) == '?' ||
              getGrid(objectRow, objectCol) == ')');
        // Set object to valid spot
        (*(*i)).setPos(objectRow, objectCol);
        // Check if object is weapon or scroll using dynamic_cast
        weapon* obj = dynamic_cast<weapon*>(*i);
        // If derived pointer points to weapon
        if(obj != nullptr){
            m_grid[objectRow][objectCol] = ')'; // Set weapon to ')'
        }
        // If derived pointer doesn't point to weapon, it's a scroll
        else{
            m_grid[objectRow][objectCol] = '?'; // Set scroll to '?'
        }
    }
    // Add golden idol or stair
    int setRow;
    int setCol;
    do {
        setRow = randInt(1, 17);
        setCol = randInt(1, 69);
    }   // Actor allowed to stand on top of idol & stair
    while (getGrid(setRow, setCol) == '#' ||
             getGrid(setRow, setCol) == ')' ||
             getGrid(setRow, setCol) == '?');
    // Set stairs until last level
    if (m_level != 4){
        m_stair = new object("stair", setRow, setCol);
        m_grid[setRow][setCol] = '>';
    }
    // Set golden idol on last level
    else{
        m_goldenIdol = new object("golden idol", setRow, setCol);
        m_grid[setRow][setCol] = '&';
    }
}

// Add objects to list to add on map
void dungeon::addObject(){
    //cout << "Objects added" << endl;
    // Number of objects chosen when dungeon is constructed
    // Magic fangs of sleep, magic axes, teleportation scrolls never placed on level
    for (int i = 0; i < m_nObjects; i++){
        // Add objects
        int key = randInt(7);
        switch(key){
            case 0:
                m_objects.push_back(new mace());    // Push pointer of new object to list of pointers
                break;
            case 1:
                m_objects.push_back(new shortSword());
                break;
            case 2:
                m_objects.push_back(new longSword());
                break;
            case 3:
                m_objects.push_back(new armorScroll());
                break;
            case 4:
                m_objects.push_back(new strengthScroll());
                break;
            case 5:
                m_objects.push_back(new healthScroll());
                break;
            case 6:
                m_objects.push_back(new dexterityScroll()); 
                break;
            default:
                break;
        }
    }
    setObject();
}

// Is the next step a valid move?
bool dungeon::isMovable(int r, int c){
    if(m_grid[r][c] == '>' || m_grid[r][c] == '&' || m_grid[r][c] == ' '
       || m_grid[r][c] == '?' || m_grid[r][c] == ')'){
        return true;
    }
    // Cannot move on top of actors or walls
    else{
        return false;
    }
}

// Called in actor.cpp's move function
void dungeon::setSymbol(int r, int c, char symb){
    m_grid[r][c] = symb;
}

// Sort rooms
bool roomCompare(room r1, room r2){
    return r1.sr > r2.sr;
}

// ONLY creates dungeon walls + rooms + pathways
void dungeon::createMap(int m_nRooms){
    // Reset dungeon with all walls
    for (int r = 0; r < 18; r++){
        for (int c = 0; c < 70; c++){
             m_grid[r][c] = '#';
        }
    }
    // Create new rooms
    room* rooms = new room[m_nRooms];
    for (int i = 0; i < m_nRooms; i++){
        // Set the coordinates of the rooms with its lengths and widths
        rooms[i].length = randInt(4, 10);
        rooms[i].width = randInt(5, 13);
        rooms[i].sc = randInt(12 - rooms[i].length)+3;
        rooms[i].sr = randInt(64 - rooms[i].width)+3;
        // Check if valid rooms
        bool validRoom = false;
        for (int j = 0; j < i; j++){
            int roomWidth = rooms[j].sr + rooms[j].width;
            int roomLength = rooms[j].sc + rooms[j].length;
            if(rooms[i].sr > roomWidth+1 ||
               rooms[i].sr + rooms[i].width < rooms[j].sr-1 ||
               rooms[i].sc > roomLength +1 ||
               rooms[i].sc + rooms[i].length < rooms[j].sc-1){
                validRoom = true;   // Rooms don't overlap
                if(j == i-1){
                    break;
                }
            }
            else{
                validRoom = false;
            }
            if(validRoom == false && i > 0){
                i--;
            }
            else{
                continue;
            }
        }
    }
    // Assuming rooms are in scope and don't overlap
    // Set the allocated rooms as blank spaces
    for (int i = 0; i < m_nRooms; i++){
        for(int y = 0; y < rooms[i].length; y++){
            for(int x = 0; x < rooms[i].width; x++){
                m_grid[rooms[i].sc + y][rooms[i].sr + x] = ' ';
            }
        }
    }
    // Sort rooms to create pathways connecting rooms
    sort(rooms, rooms + m_nRooms, roomCompare);
    // Create corridors
    for (int i = 0; i < m_nRooms-1; i++){
        int sRow = randInt(rooms[i].width) + rooms[i].sr;
        int sCol = randInt(rooms[i].length) + rooms[i].sc;
        int eRow = randInt(rooms[i+1].width) + rooms[i+1].sr;
        int eCol = randInt(rooms[i+1].length) + rooms[i+1].sc;
        if(sRow <= eRow && sCol > eCol){
            for(int i = sRow; i <= eRow; i++){
                m_grid[eCol][i] = ' ';
            }
            for(int i = eCol; i <= sCol; i++){
                m_grid[i][eRow] = ' ';
            }
        }
        if (sRow > eRow && sCol <= eCol){
            for(int i = eRow; i <= sRow; i++){
                m_grid[eCol][i] = ' ';
            }
            for(int i = sCol; i <= eCol; i++){
                m_grid[i][sRow] = ' ';
            }
        }
        if(sRow > eRow && sCol > eCol){
            for(int i = eRow; i <= sRow; i++){
                m_grid[eCol][i] = ' ';
            }
            for(int i = eCol; i <= sCol; i++){
                m_grid[i][sRow] = ' ';
            }
        }
        if(sRow <= eRow && sCol <= eCol){
            for(int i = sRow; i <= eRow; i++){
                m_grid[eCol][i] = ' ';
            }
            for(int i = sCol; i <= eCol; i++){
                m_grid[i][eRow] = ' ';
            }
        }
    }
    // Delete array of pointers to rooms
    delete[] rooms;
}

// Displays dungeon in game AFTER setting up dungeon with
// rooms, corridors, actors, objects
// New display with every actor move
void dungeon::display(){
    clearScreen();
    // Clear dead monsters
    deleteMonster();
    // Display either stairs or golden idol on level
    if (m_level != 4){
        // Created in setObject
        m_grid[m_stair->getRow()][m_stair->getCol()] = '>';
        // cout << "Stair at :" << getStair()->getRow() << getStair()->getCol() << endl;
    }
    else{
        // Created in setObject
        m_grid[m_goldenIdol->getRow()][m_goldenIdol->getCol()] = '&';
        // cout << "Golden Idol at : " << getGoldenIdol()->getRow() << getGoldenIdol()->getCol() << endl;
    }
    // Display objects
    for(list<object*>::iterator i = m_objects.begin(); i != m_objects.end(); i++){
        int objectRow = (*i)->getRow();
        int objectCol = (*i)->getCol();
        // Check if object is weapon or scroll using dynamic_cast
        weapon* obj = dynamic_cast<weapon*>(*i);
        // If derived pointer points to weapon
        if(obj != nullptr){
            m_grid[objectRow][objectCol] = ')';
        }
        // If derived pointer doesn't point to weapon, it's a scroll
        else{
            m_grid[objectRow][objectCol] = '?';
        }
    }
    
    // Display monsters
    for(list<monster*>::iterator i = m_monsters.begin(); i != m_monsters.end(); i++){
        int monsterRow = (*i)->getRow();
        int monsterCol = (*i)->getCol();
        // Connect names with symbols
        if((*(*i)).getName() == "Bogeyman"){
            m_grid[monsterRow][monsterCol] = 'B';
        }
        else if((*(*i)).getName() == "Goblin"){
            m_grid[monsterRow][monsterCol] = 'G';
        }
        else if((*(*i)).getName() == "Snakewoman"){
            m_grid[monsterRow][monsterCol] = 'S';
        }
        else if((*(*i)).getName() == "Dragon"){
            m_grid[monsterRow][monsterCol] = 'D';
        }
    }
    // Display player as '@' if created
    if (m_player != nullptr){
        m_grid[m_player->getRow()][m_player->getCol()] = '@';
    }
    //cout << "Player location: "<< m_player->getRow() <<", " << m_player->getCol() << endl;
    for (int x = 0; x < 18; x++)
    {
        for (int y = 0; y < 70; y++)
        {
            cout << m_grid[x][y];
        }
        cout << endl;
    }
    // Display stats
    cout << "Dungeon Level: " << m_level << ", Hit points: "
    << m_player->getHp() << ", Armor: " << m_player->getAp() <<
    ", Strength: " << m_player->getSp() << ", Dexterity: " << m_player->getDp() << endl;
    cout << endl;
    
    // Display actions (picking up items, using items, attacking, etc.)
    for (unsigned long i = 0; i < m_messages.size(); i++){
        cout << m_messages[i] << endl;
    }
    // Check & return if player's dead for game's stopping condition
    if(getPlayer()->isDead()){
        return;
    }
    // Clear previous message strings
    // Delete message iterator??
    m_messages.clear();
}

// Delete dungeon & create new one in new level
void dungeon::deleteDungeon(dungeon* d){
    m_level++;
    m_nRooms = randInt(3, 5);    // Set random number of rooms
    createMap(m_nRooms);        // Creates map with rooms
    addPlayer(d->m_player); // Add new player in new level
    m_nObjects = randInt(2,3);  // Set random number of objects
    // Delete object list
    for (list<object*>::iterator i = m_objects.begin(); i != m_objects.end(); i++){
        delete *i;
        i = m_objects.erase(i);
    }
    for (list<monster*>::iterator i = m_monsters.begin(); i != m_monsters.end(); i++){
        delete *i;
        i = m_monsters.erase(i);
    }
    delete m_stair;
    addMonster();
    addObject();
}
