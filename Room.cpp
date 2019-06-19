#include "Room.h"
#include <string>
#include <queue>

std::string fList[10];
int fCount = 0;
Position* pos = new Position;
Wall* wall = new Wall;

bool contains(std::string fList[], const char* s) {
    for (int i = 0; i < fCount; ++i) {
        if (fList[i] == s)
            return true;
    }
    return false;
}

/*---------------Functions to allocate and delete 2d arrays------------*/
template <class T>
void allocate(T** &arr, int rows, int cols) {
    arr = new T*[rows];
    for (int j = 0; j < rows; j++)
        arr[j] = new T[cols];
}

template <class T>
void doDelete(T** &arr, int size) {
    for (int j = 0; j < size; j++)
        delete [] arr[j];
    delete [] arr;
    arr = NULL;
}

Room::Room() {
    xsize = ysize = 0;
    tArr = NULL;
}
void Room::clear() {
    for (int i = 1; i < xsize+1; i++) {
        for (int j = 1; j < ysize+1; j++) {
            //delete tile only if it's a chest or a door
            if ((tArr[i][j] != NULL) && (tArr[i][j]->getType() == 'C' || tArr[i][j]->getType() == 'D'))
                delete tArr[i][j];
        }
    }
    // delete the board
    doDelete(tArr, xsize+2);
}
void Room::checkType() {
    if (tArr[player.xpos][player.ypos] == NULL)
        return;
    char ch = tArr[player.xpos][player.ypos]->getType();
    if (ch == 'C') {
        int num = tArr[player.xpos][player.ypos]->getGold();
        printf("*** Congratulations on opening a chest. You collected %d coins. ***\n", num);
        player.score += num;
    }
    else if (ch == 'D') {
        char* file = tArr[player.xpos][player.ypos]->getFile();
        if (strcmp(file, "end") == 0) {
            printf("*** Congratulations you reached the exit ***\nScore: %d\n", player.score);
            exit( 0);
        }
        processFile(file);
    }
    delete tArr[player.xpos][player.ypos];
}
void Room::moveLeft() {
    if ((tArr[player.xpos][player.ypos-1] == NULL) || (tArr[player.xpos][player.ypos-1]->getType() != 'W')) {
        tArr[player.xpos][player.ypos] = NULL;
        player.ypos -= 1;
        checkType();
        tArr[player.xpos][player.ypos] = pos;
        player.moveNumber++;
    }
}
void Room::moveUp() {
    if ((tArr[player.xpos-1][player.ypos] == NULL) || (tArr[player.xpos-1][player.ypos]->getType() != 'W')) {
        tArr[player.xpos][player.ypos] = NULL;
        player.xpos -= 1;
        checkType();
        tArr[player.xpos][player.ypos] = pos;
        player.moveNumber++;
    }
}
void Room::moveRight() {
    if ((tArr[player.xpos][player.ypos+1] == NULL) || (tArr[player.xpos][player.ypos+1]->getType() != 'W')) {
        tArr[player.xpos][player.ypos] = NULL;
        player.ypos += 1;
        checkType();
        tArr[player.xpos][player.ypos] = pos;
        player.moveNumber++;
    }
}
void Room::moveDown() {
    if ((tArr[player.xpos+1][player.ypos] == NULL) || (tArr[player.xpos+1][player.ypos]->getType() != 'W')) {
        tArr[player.xpos][player.ypos] = NULL;
        player.xpos += 1;
        checkType();
        tArr[player.xpos][player.ypos] = pos;
        player.moveNumber++;
    }
}
void Room::showCommands() {
    printf("The commands for this program are:\n");
    printf("    q - quit and exit program\n");
    printf("    w - move up\n");
    printf("    a - move left\n");
    printf("    s - move down\n");
    printf("    d - move right\n");
    printf("    h - show commands\n");
    printf("    g - show score\n");
    printf("    f - find a door\n");
    printf("    b - find shortest path to a door\n");
    printf("In a room, \u2593\u2593\u2593 represents a chest, \u2587 a wall and \u2590\u2590 a door.\n\n");
}
void Room::processFile(char* str)
{
    if (contains(fList, str)) {
        printf("Door leads to a room you've been before. You can't return to a room after leaving it.\n");
        return;
    }
    /* Try to open the input file. */
    inFile = fopen(str, "r");
    
    //if opening file was not successful
    if (inFile == NULL) {
        printf("Can't open input file: %s\n", str);
        exit(-1);
    }
    fList[fCount++] = std::string(str);
    doReset();
    // loop until all lines are read from the input
    char command = ' ';
    while (!feof(inFile))
    {
        fscanf(inFile, " %c", &command);
        if (command == ' ');
        else if (command == 'S')
            doStart();
        else if (command == 'O')
            doWall();
        else if (command == 'C')
            doChest();
        else if (command == 'D')
            doDoor();
        else
            printf("Command \"%c\" is not known\n", command);
    }
    fclose(inFile);
}

void Room::doReset() {
    if (tArr != NULL)
        clear();
    
    fscanf(inFile, " %d %d", &xsize, &ysize);
    
    //Allocate memory for the room. Add extra space for the borders
    allocate(tArr, xsize+2, ysize+2);
    
    //Initialize the room to blank tiles
    for (int i = 1; i < xsize+1; i++) {
        for (int j = 1; j < ysize+1; j++) {
            tArr[i][j] = NULL;
        }
    }
    // mark the borders of the room as a wall
    for(int i = 0; i < xsize+2; i++) {
        tArr[i][0] = wall;
        tArr[i][ysize+1] = wall;
    }
    for(int j = 0; j < ysize+2; j++) {
        tArr[0][j] = wall;
        tArr[xsize+1][j] = wall;
    }
}

void Room::doStart() {
    int val1, val2;
    fscanf(inFile, " %d %d", &val1, &val2);
    
    if ((val1 > xsize) || (val1 < 1) || (val2 > ysize) || (val2 < 1))   //if it's not within range
        return;
    if (tArr[val1][val2] != NULL)       //if the position is occupied
        return;
    player.xpos = val1; player.ypos = val2;
    tArr[player.xpos][player.ypos] = pos;
}

void Room::doWall() {
    int val1, val2;
    fscanf(inFile, " %d %d", &val1, &val2);
    
    if ((val1 > xsize) || (val1 < 1) || (val2 > ysize) || (val2 < 1))   //if it's not within range
        return;
    if (tArr[val1][val2] != NULL)       //if the position is occupied
        return;
    tArr[val1][val2] = wall;
}

void Room::doChest() {
    int val1, val2, val3;
    fscanf(inFile, " %d %d %d", &val1, &val2, &val3);
    
    if ((val1 > xsize) || (val1 < 1) || (val2 > ysize) || (val2 < 1))   //if it's not within range
        return;
    if (tArr[val1][val2] != NULL)       //if the position is occupied
        return;
    tArr[val1][val2] = new Chest(val3);
}
void Room::doDoor() {
    int val1, val2;
    char filename[15];
    fscanf(inFile, " %d %d %s", &val1, &val2, filename);
    
    if ((val1 > xsize) || (val1 < 1) || (val2 > ysize) || (val2 < 1))   //if it's not within range
        return;
    if (tArr[val1][val2] != NULL)       //if the position is occupied
        return;
    tArr[val1][val2]  = new Door(filename);
}
void Room::displayRoom() {
    if (xsize > 18 || ysize > 22) {
        displaySmallRoom();
        return;
    }
    printf("\n");
    for (int i = 0; i < xsize+2; ++i) {
        for(int j = 0; j < ysize+2; j++) {
            if (i == 0 && j == 0)
                printf("  \u250F  ");
            else if (i == 0 && j == ysize+1)
                printf("  \u2513  ");
            else if (i == xsize+1 && j == 0)
                printf("  \u2517  ");
            else if (i == xsize+1 && j == ysize+1)
                printf("  \u251B  ");
            else if (i == 0 || i == xsize+1)
                printf(" \u2550\u2550\u2550 ");
            else if (j == 0 || j == ysize+1)
                printf("  \u2551  ");
            else if (tArr[i][j] == NULL)
                printf("  .  ");
            else if (tArr[i][j]->getType() == 'C')
                printf(" \u2593\u2593\u2593 ");
            else if (tArr[i][j]->getType() == 'D')
                printf(" \u2590\u2590  ");
            else if (tArr[i][j]->getType() == 'W')
                printf("  \u2587  ");
            else
                printf("  \u2055  ");
        }
        printf("\n\n");
    }
}
void Room::displaySmallRoom() {
    printf("\n");
    for (int i = 0; i < xsize+2; ++i) {
        for(int j = 0; j < ysize+2; j++) {
            if (i == 0 && j == 0)
                printf(" \u250F ");
            else if (i == 0 && j == ysize+1)
                printf(" \u2513 ");
            else if (i == xsize+1 && j == 0)
                printf(" \u2517 ");
            else if (i == xsize+1 && j == ysize+1)
                printf(" \u251B ");
            else if (i == 0 || i == xsize+1)
                printf(" \u2550 ");
            else if (j == 0 || j == ysize+1)
                printf(" \u2016 ");
            else if (tArr[i][j] == NULL)
                printf(" . ");
            else if (tArr[i][j]->getType() == 'C')
                printf(" \u2593 ");
            else if (tArr[i][j]->getType() == 'D')
                printf("\u2590\u2590 ");
            else if (tArr[i][j]->getType() == 'W')
                printf(" \u2587 ");
            else
                printf(" \u2055 ");
        }
        printf("\n");
    }
}
void Room::printSolution(bool sol[]) {
    for (int i = 0; i < xsize+2; i++) {
        for (int j = 0; j < ysize+2; j++) {
            if (i == 0 && j == 0)
                printf(" \u250F ");
            else if (i == 0 && j == ysize+1)
                printf(" \u2513 ");
            else if (i == xsize+1 && j == 0)
                printf(" \u2517 ");
            else if (i == xsize+1 && j == ysize+1)
                printf(" \u251B ");
            else if (i == 0 || i == xsize+1)
                printf(" \u2550 ");
            else if (j == 0 || j == ysize+1)
                printf(" \u2016 ");
            else if ((tArr[i][j] != NULL) && (tArr[i][j]->getType() == 'W'))
                printf(" \u2587 ");
            else if (sol[i*(ysize+2)+j] == true)
                printf(" \u2055 ");
            else
                printf(" . ");
        }
        printf("\n");
    }
}
void Room::dfs(int pos, int end, bool sol[]) {
    static int moves[4] = {ysize+2, -1, 1, -(ysize+2)};
    if (pos == end)
        return;
    
    for (int j = 0; j < 4; j++) {
        int adj = pos + moves[j];
        int x = adj/(ysize+2);
        int y = adj%(ysize+2);
        if ((tArr[x][y] == NULL || tArr[x][y]->getType() != 'W') && !sol[adj] && !sol[end]) {
            sol[adj] = true;
            dfs(adj, end, sol);
        }
    }
}
void Room::bfs(int pos, int end, bool sol[]) {
    int moves[4] = {ysize+2, -1, 1, -(ysize+2)};
    std::queue<int> q;
    q.push(pos);
    while (!q.empty()) {
        int c = q.front(); q.pop();
        if (c == end)
            return;
        else {
            for (int j = 0; j < 4; j++) {
                int adj = c + moves[j];
                int x = adj/(ysize+2);
                int y = adj%(ysize+2);
                if ((tArr[x][y] == NULL || tArr[x][y]->getType() != 'W') && !sol[adj] && !sol[end]) {
                    sol[adj] = true;
                    q.push(adj);
                }
            }
        }
    }
}
void Room::findDoor(char dim) {
    int door;
    bool seenDoor = false;
    for (int i = 0; i < xsize+2; i++) {
        for (int j = 0; j < ysize+2; j++) {
            if (tArr[i][j] != NULL && tArr[i][j]->getType() == 'D') {
                seenDoor = true;
                door = i*(ysize+2)+j;
            }
        }
    }
    if (!seenDoor)
        return;
    bool sol[(xsize+2)*(ysize+2)];
    for (int i = 0; i < xsize+2; i++) {
        for (int j = 0; j < ysize+2; j++) {
            sol[i*(ysize+2)+j] = false;
        }
    }
    sol[player.xpos*(ysize+2)+player.ypos] = true;
    if (dim == 'd')
        dfs(player.xpos*(ysize+2)+player.ypos, door, sol);
    else
        bfs(player.xpos*(ysize+2)+player.ypos, door, sol);
    if (!sol[door]) {
        printf("No door found in this room\n");
    }
    else
        printSolution(sol);
}