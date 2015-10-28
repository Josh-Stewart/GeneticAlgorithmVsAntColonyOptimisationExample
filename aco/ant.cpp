//ant.cpp

#include "ant.h"

CAnt::CAnt() {

	int i;

	//set all ants tours to 0,1,2,3,4..
	for (i=0;i<NUM_TOWNS;i++) {
		tour[i] = i;
	}

}

void CAnt::ResetAnt() {

	int i;

	//set all ants tours to 0,1,2,3,4..
	for (i=0;i<NUM_TOWNS;i++) {
		tour[i] = i;
	}

}


void CAnt::SetDistance(double val) {

	distance = val;

}

double CAnt::GetDistance() {

	return distance;
}

void CAnt::SetTown(int pos, int val) {

	int temp;
	int i; //loop counter
	
	//traverse tour looking for val.. store position
	for (i=0;i<NUM_TOWNS;i++) {
		
		if (tour[i] == val) {
			
			temp = i;

			//swap values from pos with value from temp
			tour[temp] = tour[pos]; //put whatever is at pos, into temp
			tour[pos] = val;
		}

	}

	
}

int CAnt::GetTown(int pos) {

	return tour[pos];
}

CAnt::~CAnt() {
}