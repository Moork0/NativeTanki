#ifndef GAME

#define GAME 1

static char* COLORS[] = {"\033[m", "\033[31m", "\033[32m", "\033[33m", "\033[37m"}; /* list of colors for printing */

#define CELL 5				/* Cells number */
#define TROOPS 5			/* Troops number */

/* pointer to colors in color list */

#define GREEN COLORS[2]
#define RED COLORS[1]
#define WHITE COLORS[4]
#define ORANGE COLORS[3]
#define NORMAL COLORS[0]


struct Game{
    int id[5];
    char type[TROOPS];
};

void printTroops(char troops_arr[CELL][CELL], int flag);
void insertusertroops(char userground [CELL][CELL]);
void insertcputroops(char cpuground[CELL][CELL], struct Game* play);
void printError(char msg[100]);
void printSuccess(char msg[100]);
void printWarn(char msg[100]);
char shoot(char enemyground[CELL][CELL], int x, int y);
void colorprint(char* msg, char* color, char* normal);

#endif