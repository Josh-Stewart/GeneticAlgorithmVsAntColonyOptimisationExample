//tour.cpp

#include "tour.h"

CTour::CTour() {

	int i;

	//set all tours to 0,1,2,3,4.
	for (i=0;i<NUM_TOWNS;i++) {
		tour[i] = i;
	}

}

void CTour::SetDistance(int val) {

	distance = val;

}

int CTour::GetDistance() {

	return distance;
}

void CTour::SetTown(int pos, int val) {

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

void CTour::SwapTowns(int pos1, int pos2) {

	int temp;

	temp = tour[pos1]; //save data at position 1
	tour[pos1] = tour[pos2]; //move pos2 into pos1
	tour[pos2] = temp;
}


int CTour::GetTown(int pos) {

	return tour[pos];
}

CTour::~CTour() {
}