#include "Room.h"

int main (int argc, char **argv)
{
    char* startFile;
    if(argc < 2) {
        startFile = new char[15];
        printf("Enter filename: ");
        scanf(" %s", startFile);
    }
    else
        startFile = argv[1];
    
    /* Display a welcome message */
    printf("\n\u2554");
    for (int j = 0; j < 19; j++)
        printf(" \u2550");
    printf(" \u2557\n\u2016  Welcome to Dungeons. Let's get Rich. \u2016\n\u255A");
    for (int j = 0; j < 19; j++)
        printf(" \u2550");
    printf(" \u255D\n");
    
    Room room;
    room.processFile(startFile);
    room.showCommands();
    
    char userInput = ' ';
    // Infinite loop to run program until user selects exit, or reaches an exit
    while(true)
    {
        room.displayRoom();
        printf("%d. Your move: ", room.getMove());
        userInput = getchar();
        while (userInput == ' ' || userInput == '\n' || userInput == '\r')
            userInput = getchar();
        
        switch (userInput) {
            case 'q': printf("Thanks for playing. Exiting program... \n\n"); return 0;
            case 'a': room.moveLeft(); break;
            case 'w': room.moveUp(); break;
            case 's': room.moveDown(); break;
            case 'd': room.moveRight(); break;
            case 'h': room.showCommands(); break;
            case 'g': printf("Score is: %d\n", room.getScore()); break;
            case 'f': room.findDoor();    break;
            case 'b': room.findDoor('b'); break;
            default: printf("Invalid input, please retry.\n");
        }
    }
    return 0;
}
