//edge.h

#ifndef EDGE_HEADER
#define EDGE_HEADER

#include "global.h"

typedef struct {

	int town1;
	int town2;
	int distance;
	double pheromone;

}Edge;

class CEdge {

public:

	CEdge();
	
	//given two towns.. get data for edge
	int GetDistance(int t1, int t2);
	int GetTotalDistance();
	
	double GetPhero(int t1, int t2);
	int SetPhero(int t1, int t2, double phero);

	void Evaporate(int rate);

	~CEdge();

private:
	
	int num; //num combinations (work out given NUM_TOWNS)
	Edge *list; //all possible edge combintions, array

	int CalcEdges(int n);
};



#endif  //EDGE_HEADER