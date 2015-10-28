//edge.cpp

#include "edge.h"


CEdge::CEdge() {

	int i;

	num = CalcEdges(NUM_TOWNS);

	list = new Edge[num];

	srand(time(NULL));
	int limit = MAX_DISTANCE; 
	
	int t1,t1prev;
	int t2,t2prev;

	t1 = t1prev = 0;
	t2 = t2prev = 1;

	for (i=0;i<num;i++) {

		list[i].town1 = t1;
		list[i].town2 = t2;

		list[i].distance = rand()%limit;
		list[i].pheromone = STARTING_PHERO;
		
		//update t1,t2
		if (t2 == NUM_TOWNS-1) {
			t2prev++;
			t2 = t2prev;
			t1++;
		}	
		else
			t2++;
	}

}

int CEdge::CalcEdges(int n) {
	
	int edges;

	if (n == 2)
		return 1;
	else 
		edges =	CalcEdges(n-1) + n-1;
		return edges;

}


int CEdge::GetDistance(int t1, int t2) {

	int i;
	for (i=0;i<num;i++) { //for each edge
		//if there is a match
		if ((list[i].town1 == t1 && list[i].town2 == t2) || (list[i].town1 == t2 && list[i].town2 == t1)) {
			return list[i].distance;
		}
	}

	printf("\nno edge match when trying to get distance"); //debug
	
	return NULL;
}

int CEdge::GetTotalDistance() {

	int i;
	int total=0;

	for (i=0;i<num;i++) {

		total += list[i].distance;

	}
	
	return total;
}

double CEdge::GetPhero(int t1, int t2) {

	int i;
	for (i=0;i<num;i++) { //for each edge
		//if there is a match
		if ((list[i].town1 == t1 && list[i].town2 == t2) || (list[i].town1 == t2 && list[i].town2 == t1)) {
			return list[i].pheromone;
		}
	}

	printf("\nno edge match when trying to get pheromone"); //debug
	
	return NULL;


}
	
//returns 1 on error
int CEdge::SetPhero(int t1, int t2, double phero) {

	int i;
	for (i=0;i<num;i++) { //for each edge
		//if there is a match
		if ((list[i].town1 == t1 && list[i].town2 == t2) || (list[i].town1 == t2 && list[i].town2 == t1)) {
			list[i].pheromone = phero;
			return 0;
		}	
	}

	printf("\nno edge match when trying to set pheromone"); //debug
	
	return 1;

}

//evaporate edgelist given the evaportation rate
void CEdge::Evaporate(int rate) {

	int i;

	// traverse edge list
	for (i=0;i<num;i++) {
		
		list[i].pheromone = list[i].pheromone * rate; //half the phero (if rate = 0.5) 

	}
}

CEdge::~CEdge() {

	delete list;

}