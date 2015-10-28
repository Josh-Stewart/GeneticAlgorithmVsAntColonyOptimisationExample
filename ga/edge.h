//edge.h

#ifndef EDGE_HEADER
#define EDGE_HEADER

#include "global.h"

typedef struct {

	int town1;
	int town2;
	int distance;

}Edge;

class CEdge {

public:

	CEdge();

	//given two towns.. get distance
	int GetDistance(int t1, int t2);


	~CEdge();

private:
	
	int num; //num combinations (work out given NUM_TOWNS)
	Edge *list; //all possible edge combintions, array

	int CalcEdges(int n);
};



#endif  //EDGE_HEADER