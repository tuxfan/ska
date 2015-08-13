#ifndef PROBLEM_H
#define PROBLEM_H

#include "base.h"

#define NUM_PROBLEMS 7

typedef enum {
	MTUT=0, MIXED=1, SIN=2, TSINE=3, DIAMOND=4, ELECTRODE=5, JUMP=6
} problem_id;

extern const char *problem_name[NUM_PROBLEMS];
extern const char *problem_key[NUM_PROBLEMS];

typedef struct {
	int is[2];
	int ie[2];
	int positive;
} electrode;

typedef struct {
	problem_id id;
	func rhs;
	func sol;
	int nd;
	int boundary[6];
	electrode interior[2];
} problem;

problem *problem_create(problem_id, int nd, int map_id);

void problem_destroy(problem*);

#endif
