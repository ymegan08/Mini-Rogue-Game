# Mini-Rogue-Game

Rogue-like game in which the user defeats monsters and collects weapons in an inventory to descend 5 levels and ultimately win through attaining the golden idol.

# Design Description + Non-Trivial Algorithms

My program runs when the Game class is called, which has a pointer to the dungeon and the player. The Game class constructor allocates its dungeon pointer to a new dungeon, and the Game’s goblin smell distance parameter is sent to that dungeon as well. <br/>

The dungeon constructor assigns one of its private variables to the goblin smell distance it receives from the game, so that the Goblin class can later on access that value. The dungeon assigns a random number of rooms and sends it to its createMap function that generates the characters to place in the dungeon’s grid coordinates. The createMap function uses a struct for creating each randomly generated room, and the roomCompare function sort the generated rooms and create corridors using their dimensions. <br/>

**void dungeon::createMap(int m_nRooms)** <br/>
Initialize dungeon map by filling it with ‘#’ walls <br/>
Create an array of new rooms with randomly generated number (m_nRooms) <br/>
For the amount of rooms: <br/>
Set the coordinates of the rooms with its lengths and widths from, contained in each room struct <br/>
Check if the randomly created rooms are valid; they are within bounds and do not overlap <br/>
Set allocated rooms as blank spaces <br/>
Sort rooms to create pathways connecting rooms <br/>
For the number of rooms created: <br/>
Generate random starting point of corridor <br/>
Compare rows and columns of sorted rooms to connect the corridor to a random point on another room as the end point <br/>
Deallocate pointers of rooms <br/>

Then, the dungeon generates a random number of objects between 2-3. This value is accessed as the Game constructor calls the dungeon’s addObject function. For the amount of objects to be created, this function randomly chooses and creates new object pointers to push into the dungeon’s list of object pointers. This function then calls setObject to access the list of object pointers. It iterates through the list and sets the corresponding symbols with its object type into a random valid spot on the map. The dungeon differentiates its objects by weapons and scrolls. The golden idol and stairs are newly allocated within setObject depending on the current level. The stairs and golden idol are created by setting the dungeon’s respective object pointer variables m_stair and m_goldenIdol to a new stair/golden idol within the setObject function. <br/>

The dungeon class contains public getter functions for all the dynamically allocated items and map features it has created for the actor functions to access while each actor takes their turn. The dungeon’s getStair() function will allow the game to check if the user is on top of a ‘<’ to go to the next level. The dungeon’s getGoldenIdol() function will allow the player to check if he/she has picked up a golden idol, which only gets created in the last level. <br/>

The Game constructor calls the dungeon’s addMonster function to then add the monsters to the map. The number of monsters is generated randomly within this function. Similarly to addObject, addMonster also contains a loop for the number of monsters and randomly chooses and creates new monster pointers to push into the dungeon’s list of monster pointers. The type of monsters that can be generated is determined here. This function then calls setMonster that iterates through the newly generated list of monster pointers. For each generated monster, the function chooses a random valid point on the map to create and assign the corresponding character symbols. <br/>

**void dungeon::addMonster()**<br/>
Generate random # of monsters between 2 to 5*(m_level+1)+1<br/>
For the amount of monsters to be created:<br/>
	If level 0 or 1<br/>
		Generate random number between 0-2<br/>
		Switch statement to push either new goblin or new snakewoman<br/>
	If level 2<br/>
		Generate random number between 0-3<br/>
Switch statement to push either new goblin, new snakewoman, or new bogeyman<br/>
	If level 3 and higher<br/>
		Generate random number between 0-4<br/>
	Switch statement to push either new goblin, new snakewoman, new bogeyman, or new dragon<br/>
Call setMonster()<br/>

**void dungeon::setObject()**<br/>
Iterate through dungeon’s list of object pointers, generated from addObject<br/>
For each object in the list:<br/>
	Set its coordinates to random valid spots on the map<br/>
Dynamic cast to identify weapons<br/>
		If dynamic cast isn’t a null pointer<br/>
			Set weapon coordinates to ‘)’<br/>
		If dynamic cast is a null pointer<br/>
			Set scroll coordinates to ‘?’<br/>
	Generate random coordinates for stair or golden idol<br/>
		If level is not 4<br/>
			Set dungeon’s m_stair pointer to a new stair object<br/>
			Set its coordinates to ‘>’<br/>
		If level is 4<br/>
Set dungeon’s m_goldenIdol pointer to a new golden idol object<br/>
			Set its coordinates to ‘&’<br/>

The player is newly created within the Game constructor, and the player takes in a pointer to the dungeon it is in. The Game uses the dungeon’s addPlayer which assigns the dungeon’s player pointer variable to it. The dungeon differentiates its player from the monsters by having a player class pointer and a separate list of monster class pointers. The dungeon simply adds the player within the display function.<br/>

The dungeon only gets destructed when the Game gets destructed. Descending the stairs into the next level calls the dungeon’s deleteDungeon function which acts similarly to a constructor and destructor combined, but serves the ultimate purpose of maintaining the statistics of the player throughout the game. Thus, deleteDungeon removes the dynamically allocated objects and pointers while also incrementing the current level with new monster & object pointers.<br/>

**void dungeon::deleteDungeon(Dungeon * d)**<br/>
Increment the dungeon’s level<br/>
Create and set random number of rooms<br/>
Creates map template with rooms and corridors<br/>
Set the new dungeon level’s player to be the m_player constructed initially<br/>
Set random number of objects<br/>
Deallocate dynamically allocated object list pointers of previous level<br/>
Deallocate dynamically allocated monster list pointers of previous level<br/>
Add new monsters to new map<br/>
Add new objects to new map<br/>

Game’s play function attains a pointer to track within the game. This pointer will mainly manage getCharacter inputs by the user. The dungeon’s display function is called, which initially clears the screen. It will empty the coordinates of dead monsters with each move. Simply, it will print the map coordinates with the objects, monsters, stair/golden idol, player that the dungeon created with the functions aforementioned. It also prints the dungeon level with player statistics, followed by any action messages within the dungeon’s vector of strings. The vector of action strings get cleared for the next turn. The Game’s play function takes in user inputs to make the player perform something, and then a monster list iterator will make the monsters on the map perform each of their actions if applicable. This will be like that of a loop until the player’s win status is true, and the game ends, calling the map’s destructor. The dungeon knows to destruct its list of pointer monsters & objects to prevent memory leakage, and either the golden idol’s or stair’s destructor depending on which level the user leaves the game. Summarily, the play function loops in order of display-> player action -> monster action.<br/>

The actor class is a base class that holds the commonalities between the player and monsters. For instance, both player and monsters can move, can attack, and have access to the dungeon they’re in and the weapon they’re wielding through pointers. Since the actor class sets each player and monsters’ weapons (from the weapon class), it contains a destructor for the pointers as well. Monsters and players get constructed with names so the program can identify further details about the actor. The differences of the player and monsters are represented in their corresponding classes derived from the actor class.<br/>

The player class has a list of object pointers to access its inventory, so it also contains a destructor for new dynamically allocated inventory items. playerMove is a function specifically for the player. It takes in the arrow keys (or ‘h’,’l’,’k’,’j’) to either attack a monster or move in that direction. This function checks on the player’s death status and sleep status. Similarly, the player’s readScroll and wieldWeapon functions take in user keys ‘r’ and ‘w’ respectively to either read a scroll or wield a weapon. This is done in both functions by iterating through the player’s inventory and using dynamic_cast to further identify the scroll/weapon. The player’s seeInventory is triggered by an ‘i’ input, and will simply print the player’s list of object pointers.<br/>

The player adds to his/her list of object pointers (inventory) with a ‘g’ input to call the pickUpObj function. This function first checks if the player has picked up a golden idol. If so, the player has won and the game ends. Otherwise, it calls the player’s isObjThere function to first check if that spot does have an object. This is done through comparing the player’s coordinates with the coordinates of the objects on the map. If isObjThere returns true, pickUpObj evaluates the size of the inventory. If there is still enough room, the function will iterate through the dungeon’s list of objects and the object with the same coordinates as the player will get added to the player’s inventory. Dynamic_cast is used here to identify the object picked up, and will push to the dungeon’s vector of action strings to indicate what the user picked up.<br/>

The monster class defines monster generalizations, such as checking if the player is in range (isInRange), choosing a direction (chooseDir), and maybe dropping an object if killed (drop). All of these functions take in dungeon pointers because they evaluate the contents of the current map they’re in. The drop function specifies what gets dropped by also taking in a pointer to the monster, to then evaluate its name to push back a new item to the dungeon’s object pointer list. Bogeymen, Snakewomen, Dragons, and Goblins are all derived from this base class. The monster class’s doSomething function is where the monsters will call their parent functions to either move to a spot closer to the player, or attack the player if possible. Thus, each monster has its own doSomething implementations.<br/>

**bool monster::isInRange(dungeon * d, int dist)** <br/>
Define monster’s coordinates<br/>
Define player’s coordinates<br/>
Get row difference of player and monster<br/>
Get column difference of player and monster<br/>
Add the row & column distances<br/>
Returns true if the distance is less than or equal to the monster’s smell distance, false otherwise<br/>

**bool monster::chooseDir(dungeon * d, int row, int col)** <br/>
Set end coordinates as player’s coordinates<br/>
Set start coordinates as monster’s coordinates<br/>
	If monster’s row < player’s row and is movable<br/>
		Return ARROW_DOWN<br/>
	If monster’s row > player’s row and is movable<br/>
		Return ARROW_UP<br/>
	If monster’s col < player’s col and is movable<br/>
		Return ARROW_RIGHT<br/>
	If monster’s col > player’s col and is movable<br/>
		Return ARROW_LEFT<br/>

The goblin class, derived from the monster class, has further detailing. Its setDistance takes in the dungeon’s smell distance value that was passed into the Game’s constructor as mentioned before. Its goblinSmell function is designed to be recursive, taking in the goblin’s coordinates and comparing it to the player’s coordinates on the map. Every recursive call checks if either up, down, left, or right is movable and within the goblin’s smell distance. For every true instance, it pushes the corresponding direction into a queue of characters. The function ultimately returns true if it can reach the player, and false if its distance parameter runs out before reaching the player. This is evaluated in the goblin’s doSomething function, where the queue is popped to return the direction of the first added character and the goblin either moves or attacks in that direction. After the goblin has executed an action, the queue will clear to take in the next queue of directions for the goblin’s next turn.<br/>

**bool goblin::goblinSmell(int r, int c, int distance)**<br/>
Initialize player row and column<br/>
Stopping condition: If distance between goblin and player is less than 1<br/>
Stopping condition: If evaluated coordinates are equal to player’s coordinates<br/>
If the goblin’s row is smaller than the player’s row<br/>
	Recursive call to try moving down<br/>
		Push ARROW_DOWN to goblin’s direction queue<br/>
	Recursive call to try moving up<br/>
		Push ARROW_UP to goblin’s direction queue<br/>
	Recursive call to try moving right<br/>
		Push ARROW_RIGHT to goblin’s direction queue<br/>
	Recursive call to try moving left<br/>
		Push ARROW_LEFT to goblin’s direction queue<br/>
There is no path: return false<br/>

**bool goblin::doSomething(dungeon * d)**<br/><br/>
If asleep, decrease sleep time<br/>
If dead, or the player is dead, return nothing<br/>
Call recursive function:goblinSmell with goblin’s coordinates & smell distance<br/>
	Initialize goblin row and column<br/>
	Initialize a character direction at the front of goblin’s queue<br/>
	Pop the first character off the queue<br/>
	Switch statement for character:<br/>
		ARROW_UP moves goblin up or attacks player located above<br/>
		ARROW_DOWN moves goblin down or attacks player located below<br/>
		ARROW_LEFT moves goblin left or attacks player at left<br/>
		ARROW_RIGHT moves goblin right or attacks player at right<br/>
	Clear queue container for next turn<br/>

The object class contains the commonalities of weapons and scrolls, such as its name and action string. It doesn’t point to any other class, because other classes point to it instead. It derives the object class and the weapon class. The objects and weapons are further detailed through their own derived classes. The weapon class derives the mace, short sword, long sword, magic axe, magic fang of sleep classes. The scroll class derives the teleportation, armor, strength, health, and dexterity scrolls.<br/>
