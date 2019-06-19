#ifndef ROOM_H
#define ROOM_H

#include <cstdio>
#include <cstring>

class Tile {
    private:
    char type;
    
    public:
    Tile(char t): type(t) {}   //qualified constructor
    char getType() {return type;}
    virtual int getGold() { return 0;}
    virtual char* getFile() {return NULL;}
};
// Chest class - subclass of the base class Tile
class Chest: public Tile {
    private:
    int gold;

    public:
    Chest(int v): Tile('C'), gold(v) {}
    int getGold() {return gold;}
};
// Wall class - subclass of the base class Tile
class Wall: public Tile {
    public:
    Wall(): Tile('W') {}
};
// Door class - subclass of the base class Tile
class Door: public Tile {
    private:
    char* filename;

    public:
    Door(const char* str): Tile('D') {
        filename = new char[strlen(str)+1];
        strcpy(filename, str);
    }
    char* getFile() {return filename;}
};
// Position class - subclass of the base class Tile
class Position: public Tile {
    public:
    Position(): Tile('P') {}
};

class Room;

class Player {
    friend class Room;
    
    private:
    int xpos, ypos;    // to keep track of current position in a room
    int score;
    int moveNumber;
    
    public:
    Player(): xpos(0), ypos(0), score(0), moveNumber(1) {}
};

/*-------------------------------------------------------------*/
class Room {
    private:
    int xsize, ysize;    // size of the room
    Player player;
    Tile** *tArr;     // 2D array of Tile pointers
    FILE* inFile;     // for reading from a file
    void doReset();
    void doStart();
    void doWall();
    void doChest();
    void doDoor();
    void displaySmallRoom();
    void checkType();
    void dfs(int pos, int end, bool sol[]);
    void bfs(int pos, int end, bool sol[]);
    
    public:
    int getScore() {return player.score;}
    int getMove() {return player.moveNumber;}
    Room();   //default constructor
    void showCommands();
    void processFile(char* str);
    void displayRoom();
    void printSolution(bool so[]);
    void moveLeft();
    void moveUp();
    void moveDown();
    void moveRight();
    void findDoor(char dim = 'd');
    void clear();
};

#endif