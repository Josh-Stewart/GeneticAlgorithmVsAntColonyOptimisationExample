//ant.h

#ifndef ANT_HEADER
#define ANT_HEADER

#include "global.h"

class CAnt {

public:
	
	CAnt();
	
	void ResetAnt();

	
	void SetTown(int pos, int val);
	int GetTown(int pos);
	
	void SetDistance(double val);
	double GetDistance();

	~CAnt();

private:

	int tour[NUM_TOWNS]; //ants current tour info
	double distance; 

};



#endif  //ANT_HEADER