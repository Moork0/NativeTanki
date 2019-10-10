#include "../header/io.h"
#include "../header/std.h"
#include "../header/game.h"
#include "../header/rand.h"

int main(){

    char userground [CELL][CELL] = {{'\0'}};
    char cpuground [CELL][CELL] = {{'\0'}};
    struct Game play = {.id = {0,1,2,3,4}, .type = {'s', 't', 'c', 'h', 'n'}};
    insertcputroops(cpuground, &play);
    insertcputroops(userground, &play);
//    system("clear");
    CLEAR();
    myputs("\n======= Your Troops Map ========\n\n");
    printTroops(userground, 0);
    myputs("\n======= Enemy Troops Map ========\n");
    printTroops(cpuground, 1);
    int cpukills = 0, userkills = 0, turn = 0, x = 0, y = 0;
    char result;
    int scanresult[5];
    myputs("You wanna start? (y=yes, n=no)\n-> ");
    scan("%c", 0, &result, 0);
    if(result != 'y' && result != 'Y'){
        printError("Quiting...");
        myexit(EXIT_SUCCESS);
    }
    result = '\0';
    while(cpukills != TROOPS && userkills != TROOPS){
        if(turn % 2 == 0){
//            system("clear");
            CLEAR();
            myputs("\n======= Your Troops Map ========\n\n");
            printTroops(userground, 0);
            myputs("\n======= Enemy Troops Map ========\n\n");
            printTroops(cpuground, 1);
            int arg[1] = {CELL-1};
            char* color[] = {ORANGE, NORMAL}; 
            printout("\n%sYour Turn to fire! Enter an coordinate(between 0 to %d)\n->%s"
            , arg, 0, color);
            scan("%d%d", scanresult, 0, 0);
            x = scanresult[0];
            y = scanresult[1];
            result = shoot(cpuground, x, y);
            if(result != 'S'){
                printError("You Shot Wrong!");
            }else{
                userkills++;
                printSuccess("You Shutted Down An Enemy Troop!\n");
            }
            turn++;
            continue;
        }else{
            printWarn("Enemy Turn To Shoot!\n");
//            srand(time(NULL));
            x = _rand() % CELL;
            y = _rand() % CELL;
            result = shoot(userground, x, y);
            if(result == 'S'){
                cpukills++;
                printWarn("Enemy Has shut down one of your troops!\n");
            }else{
                printWarn("Enemy shot wrong place!\n");
            }
            myputs("Continue ? (y,n)\n-> ");
            scan("%c", 0, &result, 0);
            if(result != 'y' && result != 'Y'){
                printError("Quiting...");
                myexit(EXIT_SUCCESS);
            }
            result = '\0';
            turn++;
        }
    }
    if(userkills == TROOPS){
        printSuccess("You Won!\n");
    }else{
        printError("You lose!");
    }
}
