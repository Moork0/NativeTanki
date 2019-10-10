#include "../header/io.h"
#include "../header/rand.h"

static char input[10];
static int RANDOM_INPUT = 10;

int _rand(){
    if(RANDOM_INPUT > 8){
        unsigned int f = myfopen("/dev/urandom", 'r');
        mynfread(f, input, 9);
        myfclose(f);
        RANDOM_INPUT = 0;
    }
    int random = UINT(input[RANDOM_INPUT]);
    random = ((random * 1000) - random) & SEED;
    while (random < (RMAX / 10)){
        random *= random;
    }
    ++RANDOM_INPUT;
    return random;
}
