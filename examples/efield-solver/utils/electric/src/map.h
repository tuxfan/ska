#ifndef MAP_H
#define MAP_H

#include "base.h"

#define NUM_MAPS 7

typedef enum {
	MAP_IDENTITY=0, MAP_STRETCH=1, MAP_SECOND=2, MAP_CURL=3, MAP_DIV=4, MAP_DIAMOND=5, MAP_POLAR=6
} map_id;

extern const char *map_name[NUM_MAPS];
extern const char *map_key[NUM_MAPS];

typedef struct {
	func x;
	func y;
	func z;
	int id;
} map;


map *map_create(map_id);


void map_destroy(map*);


#endif
