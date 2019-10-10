#ifndef RAND_H
#define RAND_H 1

#define UINT(c) ((unsigned int)c)		/* cast c to Unsigned int */
#define RMAX 1000000					/* max number for random generator */
#define SEED 0x7fff31f					/* Seed for random generator */
int _rand();
#endif