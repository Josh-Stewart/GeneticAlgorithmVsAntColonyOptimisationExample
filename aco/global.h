//global defines for ACO solution

// TSP specific defines
#define NUM_ANTS 25
#define NUM_TOWNS 25
#define MAX_DISTANCE 400
#define ALPHA 1
#define BETA 0.5
#define QUE 10
#define EVAP 0.3 //evaporation rate
#define STARTING_PHERO 0.5 //initial pheromone deposits
#define ADD_PHERO 0.5 //add more phero to the best tour
#define MAX_CYCLES 560


// GUI stuff
#include "allegro.h"
#include <stdio.h>