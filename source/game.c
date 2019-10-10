#include "../header/game.h"
#include "../header/io.h"
#include "../header/rand.h"

void printTroops(char troops_arr[CELL][CELL], int flag){
    char troop;
    char* colorargs[] = {RED, NORMAL};
    myputs("\n\n");
    for(int i = 0; i < CELL; i++){
        for(int j = 0; j < CELL; j++){
            troop = troops_arr[i][j];
            if(troop == '\0' && flag){  
                //printout("\t\t%c\t", 0, &troop, 0);   
                myputs("\t\t\t");         
            }else if(troop == 'X'){                
                printout("\t\t%s[%c]%s\t", 0, &troop, colorargs);
            }else if(troop == 'H'){
                colorargs[0] = GREEN;
                printout("\t\t%s[%c]%s\t", 0, &troop, colorargs);
            }else{
                printout("\t\t[%c]\t", 0, &troop, 0);
            }
        }
        myputs("\n\n");
    }
    myputs("\n\n");
}

void insertusertroops(char userground [CELL][CELL]){
    int x,y;
    char t;
    int intarg[10] = {CELL-1};
    int scanresult[5];
    for(int i = 0; i < TROOPS; i++){
        printout("Enter Your troops coordinates: (x[space]y) between 0 to %d\n", intarg, 0, 0);
        intarg[1] = i+1;
        printout("%d -> ", &intarg[1], 0, 0);
        scan("%d%d", scanresult, 0, 0);
        x = scanresult[0];
        y = scanresult[1];
        if(userground[x][y] != '\0'){
            i--;
            printError("You Already choosed this coordinates. try again");
            continue;
        }
        myputs("Enter Your troops type : (s=soldier, t=tank, c=center, h=helicopter, n=navy)");
        printout("%d -> ", &intarg[1], 0, 0);
        scan("%c", 0, &t, 0);
        userground[x][y] = t;
    }
}

void printError(char msg[100]){
    char* strargs[] = {RED, msg, NORMAL};
    printout("\n##### %s%s%s #####\n", 0, 0, strargs);
}

void insertcputroops(char cpuground[CELL][CELL], struct Game* play){
//    srand(time(NULL));
    int randi;
    int randj;
    int randt;
//    char troop;
    for(int i = 0; i < TROOPS; i++){
        randi = _rand() % CELL;
        randj = _rand() % CELL;
        if(cpuground[randi][randj] != '\0'){
            i--;
            continue;
        }
        randt = _rand() % TROOPS;
        cpuground[randi][randj] = play->type[randt];
    }
}

void printSuccess(char msg[100]){
    colorprint(msg, GREEN, NORMAL);
}

void printWarn(char msg[100]){
    colorprint(msg, ORANGE, NORMAL);
}

char shoot(char enemyground[CELL][CELL], int x, int y){
    if(x >= CELL || y >= CELL){
        return 'F';
    }
    if(enemyground[x][y] != '\0' && enemyground[x][y] != 'H' && enemyground[x][y] != 'X'){
        enemyground[x][y] = 'H';
        return 'S';
    }else{
        enemyground[x][y] = 'X';
        return 'W';
    }
}

void colorprint(char* msg, char* color, char* normal){
  char* a[] = {color, msg, normal};
  printout("%s%s%s", 0, 0, a);
}