//tour.h

#ifndef TOUR_HEADER
#define TOUR_HEADER

#include "global.h"

class CTour {

public:
	
	CTour();
	
	//void ResetTour();
	
	void SwapTowns(int pos1, int pos2);
	void SetTown(int pos, int val);
	int GetTown(int pos);
	
	void SetDistance(int val);
	int GetDistance();

	~CTour();

private:

	int tour[NUM_TOWNS]; //current tour info
	int distance; //this is used as the fitness... it stores the total distance of the tour

};



#endif  //TOUR_HEADER