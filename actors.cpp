//
//  actors.cpp
//  proj3
//
//  Created by Megan Cho on 5/17/20.
//  Copyright © 2020 Yuna Cho. All rights reserved.
//

#include "actors.h"
#include "dungeon.h"
#include "utilities.h"
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////// Actor Implementations

actor::actor(dungeon* d, int row, int col){
    //cout << "Actor constructor 1" << endl;
    m_dungeon = d;
    m_row = row;
    m_col = col;
}

actor::actor(dungeon* d, int hp, int maxhp, int ap, int sp, int dp, int st, string name){
    //cout << "Actor constructor 2" << endl;
    m_dungeon = d;
    m_hp = hp;
    max_hp = maxhp;
    m_ap = ap;
    m_sp = sp;
    m_dp = dp;
    m_st = st;
    m_name = name;
    m_dead = false;
}

// Delete weapon object when deleting actor object
actor::~actor(){
    delete m_weapon;
}

int actor::getRow() const{
    return m_row;
}

int actor::getCol() const{
    return m_col;
}

int actor::getHp() const{
    return m_hp;
}

int actor::getMaxHp() const{
    return max_hp;
}

int actor::getAp() const{
    return m_ap;
}

int actor::getSp() const{
    return m_sp;
}

int actor::getDp() const{
    return m_dp;
}

int actor::getSt() const{
    return m_st;
}

string actor::getName() const{
    return m_name;
}

dungeon* actor::getDungeon() const{
    return m_dungeon;
}

// Access actor's weapon
weapon* actor::getWeapon() const{
    return m_weapon;
}

bool actor::isDead() const{
    return m_dead;
}

// Set each actor's weapon to what is passed into parameter
void actor::setWeapon(weapon* w){
    m_weapon = w;
}

// Called by move functions of derived class actors
// JUST the action of moving in dungeon with passed in direction
void actor::move(char dir){
    switch(dir){
        case ARROW_LEFT:
            if(m_dungeon->isMovable(m_row, m_col-1)){
                // Update last location to empty
                m_dungeon->setSymbol(m_row, m_col, ' ');
                // Update new position of actor
                m_col--;
            }
            break;
        case ARROW_RIGHT:
            if(m_dungeon->isMovable(m_row, m_col+1)){
                // Update last location to empty
                m_dungeon->setSymbol(m_row, m_col, ' ');
                // Update new position of actor
                m_col++;
            }
            break;
        case ARROW_DOWN:
            if(m_dungeon->isMovable(m_row+1, m_col)){
                // Update last location to empty
                m_dungeon->setSymbol(m_row, m_col, ' ');
                // Update new position of actor
                m_row++;
            }
            break;
        case ARROW_UP:
            if(m_dungeon->isMovable(m_row-1, m_col)){
                // Update last location to empty
                m_dungeon->setSymbol(m_row, m_col, ' ');
                // Update new position of actor
                m_row--;
            }
            break;
        default:
            break;
    }
    // Display updated dungeon with movement of actor
    this->getDungeon()->display();
}

// Set position of actor
bool actor:: setPos(int r, int c){
    m_row = r;
    m_col = c;
    return true;
}

// Increase MAX Hit points
void actor::changeMaxHp(int h){
    if(max_hp + h > 99){
        max_hp = 99;
    }
    else
        max_hp += h;
}

// Change sleep time
void actor::changeSt(int s){
    if(m_st + s > 9){
        m_st = 9;
    }
    else if(m_st + s < 0){
        m_st = 0;
    }
    else
    m_st += s;
}

// Change hit points
void actor::changeHp(int h){
    // If increasing HP is less or equal to maxHP
    if(m_hp + h <= max_hp){
        m_hp += h;
    }
    else if(m_hp < 0){
        m_hp = 0;
    }
    // If increasing HP exceeds maxHp, don't change
    else{
        m_hp = max_hp;
    }
}

// Change armor points
void actor::changeAp(int a){
    if(m_ap + a > 99){
        m_ap = 99;
    }
    else
        m_ap += a;
}

// Change strength points
void actor::changeSp(int s){
    if(m_sp + s > 99){
        m_sp = 99;
    }
    else
        m_sp += s;
}

// Change dexterity points
void actor::changeDp(int d){
    if(m_dp + d > 99){
        m_dp = 99;
    }
    else
        m_dp += d;
}

// Actor's attack function
void actor::attack(actor* attacker, actor* defender){
    int attackerPoints = attacker->m_dp + attacker->m_weapon->getdpBonus();
    int defenderPoints = defender->m_dp + defender->m_ap;
    int attP = randInt(1, attackerPoints);
    int defP = randInt(1, defenderPoints);
    // Attacker has hit defender
    if(attP >= defP){
        // Randomly generated damage amount for defender
        int damagePoints = randInt(attacker->m_sp + attacker->m_weapon->getDamage());
        defender->m_hp -= damagePoints; // Update damage done
        // Defender is alive and hit with a weapon other than magic fangs of sleep (regular point damage + message)
        if(defender->m_hp > 0 && attacker->getWeapon()->getName() != "magic fangs of sleep"){
            getDungeon()->getMessages().push_back("The " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " " + attacker->getWeapon()->getName() + " at " + defender->getName() + " and hits.");
        }
        // Conditionals for magic fangs of sleep
        else{
            // Defender is alive and hit with magic fangs of sleep
            if(defender->m_hp > 0 && attacker->getWeapon()->getName() == "magic fangs of sleep"){
                // 1 in 5 chance that magic fangs of sleep will put defender to sleep
                bool goSleep = trueWithProbability(1.0/5);
                // Defender was not asleep
                if(goSleep && defender->getSt() == 0){
                    // Sleep lasts random number of moves from 2 to 6
                    defender->changeSt(randInt(2,6));
                    // Show message
                    getDungeon()->getMessages().push_back("The " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " magic fangs at " + defender->getName() + " and hits, putting " + defender->getName() + " to sleep.");
                }
                // Defender was asleep
                else if(goSleep && defender->getSt() > 0){
                    // Defender's sleep time becomes max of their sleep time and random integer from 2 to 6
                    int sleepTime = max(defender->getSt(), randInt(2, 6));
                    defender->changeSt(sleepTime - defender->getSt());
                    // Show message
                    getDungeon()->getMessages().push_back("The " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " magic fangs at " + defender->getName() + " and hits, putting " + defender->getName() + " to sleep.");
                }
                // Defender was not put to sleep
                else if(!goSleep){
                    // Show message
                    getDungeon()->getMessages().push_back("The " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " magic fangs at " + defender->getName() + " and hits.");
                }
            }
            // Defender is dead
            if(defender->m_hp <= 0){
                // Set death status to true
                defender->m_dead = true;
                monster* isMonster = dynamic_cast<monster*>(defender);
                // Defender is a monster
                if(isMonster != nullptr){
                    // Maybe drops an item
                    isMonster->drop(defender->getDungeon(), isMonster);
                    // Show message
                    getDungeon()->getMessages().push_back("The " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " " + attacker->getWeapon()->getName() + " at " + defender->getName() + ", dealing a final blow.");
                }
                // Defender is a player
                else{
                    player* isPlayer = dynamic_cast<player*>(defender);
                    if(isPlayer != nullptr){
                        // Show message
                        getDungeon()->getMessages().push_back("The " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " " + attacker->getWeapon()->getName() + " at " + defender->getName() + ", dealing with a final blow.");
                        // Exit game?
                        getDungeon()->getMessages().push_back("Press q to exit game.");
                    }
                }
            }
        }
    }
    // Defender is not hit
    else if(attP < defP && attacker->getWeapon()->getName() != "magic fangs of sleep"){
        getDungeon()->getMessages().push_back("The " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " " + attacker->getWeapon()->getName() + " at " + defender->getName() + " and misses.");
    }
    // Attacker misses hit with magic fangs of sleep (to get name in message correct)
    else if(attP < defP && attacker->getWeapon()->getName() == "magic fangs of sleep"){
        getDungeon()->getMessages().push_back("The " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " magic fangs at " + defender->getName() + " and misses.");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////// Player Implementations

player::player(dungeon* d):actor(d, 20, 20, 2, 2, 2, 0, "Player"){
    m_win = false;
    weapon* startWeapon = new shortSword(); // Player starts with short sword
    actor::setWeapon(startWeapon);  // Set player's weapon as short sword
    m_inventory.push_back(startWeapon); // Add short sword to inventory
}

// Delete inventory list associated with game
player::~player(){
    for(list<object*>::iterator i = m_inventory.begin(); i != m_inventory.end(); i++){
        delete *i;
        i = m_inventory.erase(i);
    }
}

// Get inventory list of player
list<object*> player::getInventory() const{
    return m_inventory;
}

// Returns whether player won or not
bool player::didWin() const{
    return m_win;
}

// Show inventory when 'i' from game.cpp
void player::seeInventory(){
    if(isDead())
        return;
    // If player is asleep, this move will decrease sleep time
    if(getSt() > 0){
        changeSt(-1);
        return;
    }
    clearScreen();  // Clear map to show inventory
    cout << "Inventory: " << endl;
    char pt = 'a';
    // Show all inventory items with alphabetical list
    // *i gives the address
    for(list<object*>::iterator i = m_inventory.begin(); i != m_inventory.end(); i++, pt++){
        cout << " " << pt << ". " << (*i)->getName() << endl;
    }
}

// Check if there is an object to pick up at player's location
bool player::isObjThere(){
    list<object*> check = getDungeon()->getObjects();
    for(list<object*>::iterator i = check.begin(); i != check.end(); i++){
        int r = (*i)->getRow();
        int c = (*i)->getCol();
        if(r == getRow() && c == getCol())
            return true;
    }
    return false;
}

// Pick up an object on the map, updates action messages
void player::pickUpObj(){
    int playRow = getRow();
    int playCol = getCol();
    if(getDungeon()->getGoldenIdol() != nullptr &&
       getDungeon()->isGoldenIdol(playRow, playCol)){
        m_win = true;
        cout << "You pick up the golden idol" << endl;
        cout << "Congratulations, you won!" << endl;
        cout << "Press q to exit game." << endl;
        char c = getCharacter();
        if(c == 'q'){
            exit(1);
        }
        return;
    }
    // User can't pick up an object if there isn't one at the spot
    if(!isObjThere()){
        return;
    }
    // User is on top of a valid object
    else{
        // If inventory is full
        if(m_inventory.size() > 25){
            // Push full capacity warning to dungeon's message list
            getDungeon()->getMessages().push_back("Your knapsack is full; you can't pick that up.");
            return;
        }
        // If inventory is not full
        for(list<object*>::iterator i = getDungeon()->getObjects().begin();
            i != getDungeon()->getObjects().end(); i++){
            // If user's coordinates match item's coordinates
            if(getRow() == (*i)->getRow() && getCol() == (*i)->getCol()){
                 // Add object to inventory list
                m_inventory.push_back(*i);
                // Use dynamic_cast to show weapon messages
                weapon* isWeapon = dynamic_cast<weapon*>(*i);
                // If object is a weapon
                if (isWeapon != nullptr){
                    if((*i)->getName() == "mace"){
                        getDungeon()->getMessages().push_back("You pick up a mace.");
                    }
                    else if((*i)->getName() == "short sword"){
                        getDungeon()->getMessages().push_back("You pick up a short sword.");
                    }
                    else if((*i)->getName() == "long sword"){
                        getDungeon()->getMessages().push_back("You pick up a long sword.");
                    }
                    else if((*i)->getName() == "magic axe"){
                        getDungeon()->getMessages().push_back("You pick up a magic axe.");
                    }
                    else if((*i)->getName() == "magic fangs of sleep"){
                        getDungeon()->getMessages().push_back("You pick up a magic fang of sleep.");
                    }
                    // Erase the object from the dungeon's object list
                    getDungeon()->getObjects().erase(i);
                    break;
                }
                // Use dynamic cast to show scroll messages
                scroll* isScroll = dynamic_cast<scroll*>(*i);
                // If object is a scroll
                if (isScroll != nullptr){
                    if((*i)->getName() == "scroll of teleportation"){
                        getDungeon()->getMessages().push_back("You pick up a scroll called scroll of teleportation.");
                    }
                    else if((*i)->getName() == "scroll of improve armor"){
                        getDungeon()->getMessages().push_back("You pick up a scroll called scroll of improve armor.");
                    }
                    else if((*i)->getName() == "scroll of raise strength"){
                        getDungeon()->getMessages().push_back("You pick up a scroll called scroll of raise strength.");
                    }
                    else if((*i)->getName() == "scroll of enhance health"){
                        getDungeon()->getMessages().push_back("You pick up a scroll called scroll of enhance health.");
                    }
                    else if((*i)->getName() == "scroll of enhance dexterity"){
                        getDungeon()->getMessages().push_back("You pick up a scroll called scroll of enhance dexterity.");
                    }
                    // Erase the object from the dungeon's object list
                    getDungeon()->getObjects().erase(i);
                    break;
                }
            }
        }
    }
}

// Read scroll when 'r'
void player::readScroll(char key){
    // Store key + number of objects to evaluate
    int numObj = key - 'a';
    int objChoice = 0;
    for (list<object*>::iterator i = m_inventory.begin(); i != m_inventory.end(); i++){
        // Out of bounds choice
        if(objChoice > numObj){
            break;
        }
        // Object choice matches object in inventory
        if(objChoice == numObj){
            // Dynamic cast to check if it is a valid scroll
            scroll* isScroll = dynamic_cast<scroll*>(*i);
            // Choice is a scroll
            if(isScroll != nullptr){
                // Get name of scroll
                string scrollName = (*i)->getName();
                if(scrollName == "scroll of improve armor"){
                    // Player's ap increased by random integer from 1 to 3
                    actor::changeAp(randInt(1, 3));
                }
                else if(scrollName == "scroll of raise strength"){
                    // Player's sp increased by random integer from 1 to 3
                    actor::changeSp(randInt(1, 3));
                }
                else if(scrollName == "scroll of enhance health"){
                    // Player's max increased by random integer from 3 to 8
                    // Doesn't affect player's current HP
                    actor::changeMaxHp(randInt(3, 8));
                }
                else if(scrollName == "scroll of enhance dexterity"){
                    // Player's dp increased by 1
                    actor::changeDp(1);
                }
                // Player randomly moved to another place in the level not occupied by a wall or monster
                else if(scrollName == "scroll of teleportation"){
                    dungeon* currentDungeon = getDungeon();
                    int teleportRow;
                    int teleportCol;
                    // Random generation of valid spots to teleport to
                    do{
                        teleportRow = randInt(1, 17);
                        teleportCol = randInt(1, 69);
                    }
                    while(currentDungeon->getGrid(teleportRow, teleportCol) == '@' ||
                          currentDungeon->getGrid(teleportRow, teleportCol) == '#' ||
                          currentDungeon->getGrid(teleportRow, teleportCol) == 'B' ||
                          currentDungeon->getGrid(teleportRow, teleportCol) == 'G' ||
                          currentDungeon->getGrid(teleportRow, teleportCol) == 'S' ||
                          currentDungeon->getGrid(teleportRow, teleportCol) == 'D');
                    // Set player's current position as empty
                    this->getDungeon()->setSymbol(this->getRow(), this->getCol(), ' ');
                    // Move player to teleported location
                    this->setPos(teleportRow,teleportCol);
                }
                // Display messages after reading scroll
                getDungeon()->getMessages().push_back("You read the scroll called "
                                                      + (*i)->getName());
                getDungeon()->getMessages().push_back((*i)->getAction());
                m_inventory.erase(i); // Delete scroll after reading
                break;
            }
            // Choice is not a scroll
            else{
                getDungeon()->getMessages().push_back("You can't read a "
                                                      + (*i)->getName());
                break;
            }
        }
        // Iterate until matching item
        else{
            objChoice++;
        }
    }
}

// Wield weapon when 'w'
void player::wieldWeapon(char key){
    // Store key + number of objects to evaluate
    int numObj = key - 'a';
    int objChoice = 0;
    for(list<object*>::iterator i = m_inventory.begin(); i != m_inventory.end(); i++){
        // Out of bounds choice
        if(objChoice > numObj){
            break;
        }
        // Object choice matches object in inventory
        if (objChoice == numObj){
            // Dynamic cast to check if item is a weapon
            weapon* isWeapon = dynamic_cast<weapon*>(*i);
            // Choice is a weapon
            if(isWeapon != nullptr){
                string weaponName = (*i)->getName();
                if(weaponName == "mace"){
                    actor::setWeapon(isWeapon);
                }
                else if(weaponName == "short sword"){
                    actor::setWeapon(isWeapon);
                }
                else if(weaponName == "long sword"){
                    actor::setWeapon(isWeapon);
                }
                else if(weaponName == "magic axe"){
                    actor::setWeapon(isWeapon);
                }
                else if(weaponName == "magic fangs of sleep"){
                    actor::setWeapon(isWeapon);
                }
                // Show messages after wielding weapon
                getDungeon()->getMessages().push_back("You are wielding " + (*i)->getName());
                break;
            }
            // Choice is not a weapon
            else{
                getDungeon()->getMessages().push_back("You can't wield " + (*i)->getName());
                break;
            }
        }
        // Iterate until matching item
        else{
            objChoice++;
        }
    }
}

// Cheat function, called by game.cpp's play()
void player::cheat(){
    actor::changeSp(9 - getSp());
    actor::changeMaxHp(50 - getMaxHp());
    actor::changeHp(50 - getHp());
}

// 1 in 10 chance of regaining 1 hp up to maxhp
void player::randomAddHp(){
    if (getHp() < getMaxHp()){
        if(trueWithProbability(1.0/10)){
            actor::changeHp(1);
        }
    }
}

// Recieves direction from Game::play()'s getCharacter
void player::playerMove(char dir){
    // Doesn't move if player is dead
    if(isDead())
        return;
    // If player is asleep, decrease sleep time by 1 and return
    if(getSt() > 0){
        changeSt(-1);
        return;
    }
    randomAddHp();  // 1 in 10 chance of hp + 1
    int playerRow = getRow();
    int playerCol = getCol();
    dungeon* current = getDungeon();
    // Player moves with getCharacter() direction from game.cpp or attacks
    switch (dir){
        case ARROW_LEFT:
            // Attack if next to monster
            if(current->isMonster(playerRow, playerCol-1)){
                monster* left = current->getMonster(playerRow, playerCol-1);
                attack(this, left);
            }
            else
                actor::move(dir);
            break;
        case ARROW_RIGHT:
            // Attack if next to monster
            if(current->isMonster(playerRow, playerCol+1)){
                monster* right = current->getMonster(playerRow, playerCol+1);
                attack(this, right);
            }
            else
                actor::move(dir);
            break;
        case ARROW_UP:
             // Attack if next to monster
            if(current->isMonster(playerRow-1, playerCol)){
                monster* up = current->getMonster(playerRow-1, playerCol);
                attack(this, up);
            }
            else
                actor::move(dir);
            break;
        case ARROW_DOWN:
             // Attack if next to monster
            if(current->isMonster(playerRow+1, playerCol)){
                monster *down = current->getMonster(playerRow+1, playerCol);
                attack(this, down);
            }
            else
                actor::move(dir);
            break;
        default:
            break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////// Monster Implementations

monster::monster(dungeon* d, int hp, int maxhp, int ap, int sp, int dp, int st, string name)
:actor(d, hp, maxhp, ap, sp, dp, st, name){}


// Construct monsters

bogeyman::bogeyman(dungeon* d):monster(d, randInt(5,10), randInt(5,10), 2, randInt(2,3), randInt(2,3), 0, "Bogeyman"){
    // Bogeyman starts with a short sword
    weapon* bogeymanWeapon = new shortSword();
    actor::setWeapon(bogeymanWeapon);   // Set bogeyman's weapon
}

snakewoman::snakewoman(dungeon* d):monster(d, randInt(3,6), randInt(3,6), 3, 2, 3, 0, "Snakewoman"){
    // Snakewoman starts with magic fangs of sleep
    weapon* snakewomanWeapon = new magicFang();
    actor::setWeapon(snakewomanWeapon); // Set snakewoman's weapon
}

dragon::dragon(dungeon* d):monster(d, randInt(20,25), randInt(20,25), 4, 4, 4, 0, "Dragon"){
    // Dragon starts with a long sword
    weapon* dragonWeapon = new longSword();
    actor::setWeapon(dragonWeapon); // Set dragon's weapon
}

goblin::goblin(dungeon* d):monster(d, randInt(15,20), randInt(15,20), 1, 3, 1, 0, "Goblin"){
    // Goblin starts with a short sword
    weapon* goblinWeapon = new shortSword();
    actor::setWeapon(goblinWeapon); // Set goblin's weapon
    setDistance();  // Set goblin smell distance
}


// Monster smell function (except for goblins)
// Returns true if player is in the monster's range
bool monster::isInRange(dungeon *d, int dist){
    int monsterRow = getRow();
    int monsterCol = getCol();
    int playerRow = d->getPlayer()->getRow();
    int playerCol = d->getPlayer()->getCol();
    // Get measurements from monster's position to player's position
    int rowDiff = max(playerRow, monsterRow) - min(playerRow, monsterRow);
    int colDiff = max(playerCol, monsterCol) - min(playerCol, monsterCol);
    int movesToPlayer = rowDiff + colDiff;
    // If # of coordinates to player <= distance input, return true
    if (movesToPlayer <= dist){
        return true;
    }
    return false;
}

// Monster movement direction, called in each monster's doSomething
char monster::chooseDir(dungeon *d, int row, int col){
    int endRow = d->getPlayer()->getRow();
    int endCol = d->getPlayer()->getCol();
    int startRow = row;
    int startCol = col;
    if(((startRow < endRow) && ((d->isMovable(startRow+1, startCol)))) || (d->isPlayer(startRow+1, startCol))){
        return ARROW_DOWN;
    }
    else if(((startRow > endRow) && ((d->isMovable(startRow-1, startCol)))) || (d->isPlayer(startRow-1, startCol))){
        return ARROW_UP;
    }
    else if(((startCol < endCol) && ((d->isMovable(startRow, startCol+1)))) || (d->isPlayer(startRow, startCol+1))){
        return ARROW_RIGHT;
    }
    else if(((startCol > endCol) && ((d->isMovable(startRow, startCol-1)))) || (d->isPlayer(startRow, startCol-1))){
        return ARROW_LEFT;
    }
    else
        return '\0';
}

// Monsters may drop an item if they are killed
void monster::drop(dungeon *d, monster* m){
    int monsterRow = m->getRow();
    int monsterCol = m->getCol();
    // If monster is dead and not on top of an existing item
    if (m->isDead() == true &&
        d->getGrid(monsterRow, monsterCol) != '(' &&
        d->getGrid(monsterRow, monsterCol) != '@' &&
        d->getGrid(monsterRow, monsterCol) != '>' &&
        d->getGrid(monsterRow, monsterCol) != '?' &&
        d->getGrid(monsterRow, monsterCol) != '&'){
        // If a bogeyman has died
        if(m->getName() == "Bogeyman"){
            // 1 in 10 chance of dropping a magic axe
            bool dropMagicAxe = trueWithProbability(1.0/10);
            if(dropMagicAxe){
                d->getObjects().push_back(new magicAxe(monsterRow, monsterCol));
            }
        }
        // If a snakewoman has died
        else if(m->getName() == "Snakewoman"){
            // 1 in 3 change of dropping a magic fang
            bool dropFang = trueWithProbability(1.0/3);
            if(dropFang){
                d->getObjects().push_back(new magicFang(monsterRow, monsterCol));
            }
        }
        // If a dragon has died
        else if(m->getName() == "Dragon"){
            // Randomly drop any of the 5 scrolls
            int dropScroll = randInt(5);
            switch(dropScroll){
                case 0:
                    d->getObjects().push_back(new strengthScroll(monsterRow, monsterCol));
                    break;
                case 1:
                    d->getObjects().push_back(new healthScroll(monsterRow, monsterCol));
                    break;
                case 2:
                    d->getObjects().push_back(new dexterityScroll(monsterRow, monsterCol));
                    break;
                case 3:
                    d->getObjects().push_back(new armorScroll(monsterRow, monsterCol));
                    break;
                case 4:
                    d->getObjects().push_back(new teleportScroll(monsterRow, monsterCol));
                    break;
                default:
                    break;
            }
        }
        // If a goblin has died
        else if(m->getName() == "Goblin"){
            // 1 in 3 chance of dropping either magic fangs of sleep or a magix axe
            bool goblinDrop = trueWithProbability(1.0/3);
            if(goblinDrop){
                int whichItem = randInt(2);
                switch(whichItem){
                    case 0:
                        d->getObjects().push_back(new magicAxe(monsterRow, monsterCol));
                        break;
                    case 1:
                        d->getObjects().push_back(new magicFang(monsterRow, monsterCol));
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////// Move + attack for monsters

void bogeyman::doSomething(dungeon* d){
    // If asleep
    if(getSt() > 0){
        changeSt(-1);
        return;
    }
    // If dead
    if(isDead() || d->getPlayer()->isDead()){
        return;
    }
    // If the player is in range
    if(monster::isInRange(d,5)){
        int monsterRow = getRow();
        int monsterCol = getCol();
        // Chooses direction for monster to move in
        char dir = monster::chooseDir(d, monsterRow, monsterCol);
        
        switch(dir){
            case ARROW_LEFT:
                // Attack if next to player
                if(d->isPlayer(monsterRow, monsterCol-1) ||
                   d->isPlayer(monsterRow, monsterCol+1) ||
                   d->isPlayer(monsterRow-1, monsterCol) ||
                   d->isPlayer(monsterRow+1, monsterCol)){
                    attack(this, d->getPlayer());
                }
                else{
                    actor::move(dir);
                }
                break;
            case ARROW_RIGHT:
                // Attack if next to player
                if(d->isPlayer(monsterRow, monsterCol-1) ||
                   d->isPlayer(monsterRow, monsterCol+1) ||
                   d->isPlayer(monsterRow-1, monsterCol) ||
                   d->isPlayer(monsterRow+1, monsterCol)){
                    attack(this, d->getPlayer());
                }
                else{
                    actor::move(dir);
                }
                break;
            case ARROW_UP:
                // Attack if next to player
                if(d->isPlayer(monsterRow, monsterCol-1) ||
                   d->isPlayer(monsterRow, monsterCol+1) ||
                   d->isPlayer(monsterRow-1, monsterCol) ||
                   d->isPlayer(monsterRow+1, monsterCol)){
                    attack(this, d->getPlayer());
                }
                else{
                    actor::move(dir);
                }
                break;
            case ARROW_DOWN:
                // Attack if next to player
                if(d->isPlayer(monsterRow, monsterCol-1) ||
                   d->isPlayer(monsterRow, monsterCol+1) ||
                   d->isPlayer(monsterRow-1, monsterCol) ||
                   d->isPlayer(monsterRow+1, monsterCol)){
                    attack(this, d->getPlayer());
                }
                else{
                    actor::move(dir);
                }
                break;
            default:
                break;
        }
    }
}
void snakewoman::doSomething(dungeon* d){
    // If asleep
    if(getSt() > 0){
        changeSt(-1);
        return;
    }
    // If dead
    if(isDead() || d->getPlayer()->isDead()){
        return;
    }
    // If the player is in range
    if(monster::isInRange(d,3)){
        int monsterRow = getRow();
        int monsterCol = getCol();
        // Chooses direction for monster to move in
        char dir = monster::chooseDir(d, monsterRow, monsterCol);
        //cout << "Snakewoman's direction: " << dir << endl;
        switch(dir){
            case ARROW_LEFT:
                // Attack if next to player
                if(d->isPlayer(monsterRow, monsterCol-1) ||
                   d->isPlayer(monsterRow, monsterCol+1) ||
                   d->isPlayer(monsterRow-1, monsterCol) ||
                   d->isPlayer(monsterRow+1, monsterCol)){
                    attack(this, d->getPlayer());
                }
                else{
                    actor::move(dir);
                }
                break;
            case ARROW_RIGHT:
                // Attack if next to player
                if(d->isPlayer(monsterRow, monsterCol-1) ||
                   d->isPlayer(monsterRow, monsterCol+1) ||
                   d->isPlayer(monsterRow-1, monsterCol) ||
                   d->isPlayer(monsterRow+1, monsterCol)){
                    attack(this, d->getPlayer());
                }
                else{
                    actor::move(dir);
                }
                break;
            case ARROW_UP:
                // Attack if next to player
                if(d->isPlayer(monsterRow, monsterCol-1) ||
                   d->isPlayer(monsterRow, monsterCol+1) ||
                   d->isPlayer(monsterRow-1, monsterCol) ||
                   d->isPlayer(monsterRow+1, monsterCol)){
                    attack(this, d->getPlayer());
                }
                else{
                    actor::move(dir);
                }
                break;
            case ARROW_DOWN:
                // Attack if next to player
                if(d->isPlayer(monsterRow, monsterCol-1) ||
                   d->isPlayer(monsterRow, monsterCol+1) ||
                   d->isPlayer(monsterRow-1, monsterCol) ||
                   d->isPlayer(monsterRow+1, monsterCol)){
                    attack(this, d->getPlayer());
                }
                else{
                    actor::move(dir);
                }
                break;
            default:
                break;
        }
    }
}

// Dragons will have 1 out of 10 probability to recover 1 hp
void dragon::recover(){
    if (getHp() < getMaxHp()){
        if(trueWithProbability(1.0/10)){
            changeHp(1);
        }
    }
}

// Doesn't move, only attacks when directly next to player
void dragon::doSomething(dungeon* d){
    // If asleep
    if(getSt() > 0){
        changeSt(-1);
        return;
    }
    // If dead
    if(isDead() || d->getPlayer()->isDead()){
        return;
    }
    recover();  // 1 in 10 chance of recovering 1 HP
    // Dragons don't move; they only attack
    int dragonRow = getRow();
    int dragonCol = getCol();
    if(d->isPlayer(dragonRow, dragonCol-1)){
        attack(this, d->getPlayer());
    }
    else if(d->isPlayer(dragonRow, dragonCol+1)){
        attack(this, d->getPlayer());
    }
    else if(d->isPlayer(dragonRow-1, dragonCol)){
        attack(this, d->getPlayer());
    }
    else if(d->isPlayer(dragonRow+1, dragonCol)){
        attack(this, d->getPlayer());
    }
}

// Set smell distance from game/dungeon parameter
void goblin::setDistance(){
    g_smellDistance = getDungeon()->getGoblinSmell();
}

// Is it possible for goblin to move
// '@' considered true for recursion to work
bool goblin::isGobMovable(int r, int c){
    if (getDungeon()->getGrid(r,c) == ' ' ||
        getDungeon()->getGrid(r,c) == '>' ||
        getDungeon()->getGrid(r,c) == ')' ||
        getDungeon()->getGrid(r,c) == '?' ||
        getDungeon()->getGrid(r,c) == '&' ||
        getDungeon()->getGrid(r,c) == '@')
        return true;
    else
        return false;
}

// Push directions into goblin's direction queue
bool goblin::goblinSmell(int r, int c, int distance){
    int playerRow = getDungeon()->getPlayer()->getRow();
    int playerCol = getDungeon()->getPlayer()->getCol();
    // Cannot overlap with player, stopping condition
    if (distance < 1)
        return false;
    // Stopping condition
    if (r == playerRow && c == playerCol)
        return true;
    if (r < playerRow){
        if(goblinSmell(r+1, c, distance-1) && isGobMovable(r+1, c)){
            dir.push(ARROW_DOWN);
            return true;
        }
    }
    if (r > playerRow){
        if(goblinSmell(r-1, c, distance-1) && isGobMovable(r-1, c)){
            dir.push(ARROW_UP);
            return true;
        }
    }
    if (c < playerCol){
        if(goblinSmell(r, c+1, distance-1) && isGobMovable(r, c+1)){
            dir.push(ARROW_RIGHT);
            return true;
        }
    }
    if (c > playerCol){
        if(goblinSmell(r, c-1, distance-1) && isGobMovable(r, c-1)){
            dir.push(ARROW_LEFT);
            return true;
        }
    }
    return false;
}

// Goblin's recursive funciton is called
void goblin::doSomething(dungeon* d){
    // If asleep
    if(getSt() > 0){
        changeSt(-1);
        return;
    }
    // If dead
    if(isDead() || d->getPlayer()->isDead()){
        return;
    }
    // Call recursive function
    if(goblinSmell(getRow(), getCol(), g_smellDistance)){
        int monsterRow = getRow();
        int monsterCol = getCol();
        char direction = dir.front(); // Take in queue directions
        dir.pop();  // Pop off directions as goblin moves
        switch(direction){
            case ARROW_UP:
                // Attack if next to player
                if(d->isPlayer(monsterRow, monsterCol-1) ||
                  d->isPlayer(monsterRow, monsterCol+1) ||
                  d->isPlayer(monsterRow-1, monsterCol) ||
                  d->isPlayer(monsterRow+1, monsterCol)){
                   attack(this, d->getPlayer());
                }
                else{
                   actor::move(direction);
                }
                break;
            case ARROW_DOWN:
                // Attack if next to player
                if(d->isPlayer(monsterRow, monsterCol-1) ||
                  d->isPlayer(monsterRow, monsterCol+1) ||
                  d->isPlayer(monsterRow-1, monsterCol) ||
                  d->isPlayer(monsterRow+1, monsterCol)){
                   attack(this, d->getPlayer());
                }
                else{
                   actor::move(direction);
                }
                break;
            case ARROW_LEFT:
                // Attack if next to player
                if(d->isPlayer(monsterRow, monsterCol-1) ||
                  d->isPlayer(monsterRow, monsterCol+1) ||
                  d->isPlayer(monsterRow-1, monsterCol) ||
                  d->isPlayer(monsterRow+1, monsterCol)){
                   attack(this, d->getPlayer());
                }
                else{
                   actor::move(direction);
                }
                break;
            case ARROW_RIGHT:
                // Attack if next to player
                if(d->isPlayer(monsterRow, monsterCol-1) ||
                  d->isPlayer(monsterRow, monsterCol+1) ||
                  d->isPlayer(monsterRow-1, monsterCol) ||
                  d->isPlayer(monsterRow+1, monsterCol)){
                   attack(this, d->getPlayer());
                }
                else{
                   actor::move(direction);
                }
                break;
            default:
                break;
        }
    }
    // Clear the queue container for next move
    queue <char> clear;
    dir.swap(clear);
}
