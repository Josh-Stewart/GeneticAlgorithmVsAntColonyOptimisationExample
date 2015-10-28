//main.cpp
#define USE_CONSOLE

#include "global.h" // TSP algorithm specific defines are in here
#include "edge.h"
#include "ant.h"

#include <math.h>


/////////////////////////////////////
//// PROTOTYPES /////////////////////
/////////////////////////////////////
void Draw(int cycles, int currtour, int besttour);


/////////////////////////////////////
//// MAIN ///////////////////////////
/////////////////////////////////////
int main(int argc, char *argv[])
{
 
/////////////////////////////////////
// START GUI SPECIFIC CODE //////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////
	int bpp; // color depth 
	int ret; // for processing return values
	
	//initialise allegro stuff
	allegro_init(); 
	install_keyboard(); 
	install_timer();

	bpp = desktop_color_depth();
	set_color_depth(bpp);
	
	/* Lets play the color depth game!  tries 32, 24, then 16 bit color modes */
	// set resolution to play intro movie
	ret = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	/* did the video mode set properly? */
	if (ret != 0) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Error setting %d bit graphics mode\n%s\nLets try another color depth!", bpp, allegro_error);
		
		bpp = 32;
		set_color_depth(bpp);
		
		// set resolution
		ret = set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
		/* did the video mode set properly? */
		if (ret != 0) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Error setting %d bit graphics mode\n%s\nLets try another color depth!", bpp, allegro_error);

			bpp = 16;
			set_color_depth(bpp);

			// set resolution
			ret = set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
			/* did the video mode set properly? */
			if (ret != 0) {
				set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
				allegro_message("Error setting %d bit graphics mode\n%s\nIm all out of options. Exiting", bpp, allegro_error);
				return 1;
			}
		}
	}
	
///////////////////////////
// END OF GUI SPECIFIC //////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////

//////////////////////////
///// VARIABLES //////////
//////////////////////////

	CEdge edgelist; //the edges
	int besttour = MAX_DISTANCE*NUM_TOWNS; //best result ever
	int currtour = MAX_DISTANCE*NUM_TOWNS; //best result from last tour
	int bestant = -1; //stores ant that did really well and adds more phero to its trail
	CAnt colony[NUM_ANTS]; //the ants

	int i,j; //universal for loop counters
	
	int cycles=0;
	
	srand(time(NULL));		 //seed random number
	int limit;				//universal random number limit.. (rand()%limit will give a number between 0 and limit-1)

	// Start Cycling
	while (!key[KEY_ESC] && cycles < MAX_CYCLES) {
		
		textprintf(screen,font,5,5,makecol(255,0,0),"Press Esc to hault processing [Cycle %d]",cycles);
		
		//initialise the ants before they all do a tour	
		//sets each ants first town to somewhere random
		
		limit = NUM_TOWNS;
		for (i=0;i<NUM_ANTS;i++) {			
			//colony[i].SetTown(0,i); //set each ants first tour element to a town 
			colony[i].SetTown(0,rand()%limit); //random version
		}	
		
		
		//each time though loop add another node to each ants tour list 
		//starting at the 2nd node (the 1st node is already set)
		limit = NUM_TOWNS+1;				
		double distance;
		double pheromone;	
		double probability;
		double total;
		double p[NUM_TOWNS];

		for (int tourcount=1;tourcount<NUM_TOWNS;tourcount++) {
			
			for (i=0;i<NUM_ANTS;i++) { //for each ant.. add 1 more tour
				
				//reset probabilities
				probability=0;
				total=0;
				
				//calculate best probability from all possible new paths
				
				//loop through all remaining possible tours left on the current ant
				//each time through the loop calc probability and keep track of best
				int k;
				for (j=tourcount,k=0;j<NUM_TOWNS;j++,k++) {
					
					//get variables used to calc probability
					distance = edgelist.GetDistance(colony[i].GetTown(tourcount-1), colony[i].GetTown(j));
					pheromone = edgelist.GetPhero(colony[i].GetTown(tourcount-1), colony[i].GetTown(j));
					
					probability = pow(pheromone, ALPHA);
					probability = probability * pow(distance, -BETA);
					
					p[k] = probability;
					total += probability;
					
				}
				
				//choose new town based on probability
				int best = 0;
				for (j=0;j<k;j++) {
					
					if ((p[best]/total)  > (p[j]/total)) {
						//new best
						best = j; 
					}
					
				}
				
				//move current ant to new town
				colony[i].SetTown(tourcount, colony[i].GetTown(tourcount+best));
				
			}//for ant
		}
		

		//////////////////////////////////////
		//ants now have full tour lists
		//compute length of each ants list
		//////////////////////////////////////
		for (i=0;i<NUM_ANTS;i++) { //for each ant
			
			int distance = 0;
			
			//reset currtour
			currtour = MAX_DISTANCE*NUM_TOWNS;
			
			printf("\nant %d [%d",i,colony[i].GetTown(0));//display ants tour in console window
			
			for (int j=1;j<NUM_TOWNS;j++) { //for each edge in the ants tour
				
				printf(",%d",colony[i].GetTown(j));//display ants tour in console window
				distance += edgelist.GetDistance(colony[i].GetTown(j-1), colony[i].GetTown(j));
				
			}
			
			printf("]");//display ants tour in console window
			
			//add on loopback distance
			distance += edgelist.GetDistance(colony[i].GetTown(NUM_TOWNS-1), colony[i].GetTown(0));
			
			printf("=%d",distance);//display ants tour distance in console window

			//keep record of best tours
			if (distance < besttour) {
				besttour = distance;
				bestant = i; //we only add extra phero to a tour when a best tour "record" is broken
			}
			if (distance < currtour)
				currtour = distance;

			//store distance on ant
			colony[i].SetDistance(distance);
		}

		
		/////////////////////////
		//evaporate pheromones
		/////////////////////////
		edgelist.Evaporate(EVAP);


		//////////////////////////////////////////////////
		//lay new pheromones based on colony[i].distance
		//////////////////////////////////////////////////
		for (i=0;i<NUM_ANTS;i++) {
			
			double phero;
			
			for (int j=1;j<NUM_TOWNS;j++) { //for each edge in the ants tour
				
				phero = edgelist.GetPhero(colony[i].GetTown(j-1), colony[i].GetTown(j)); //get old phero
				phero = phero + QUE / colony[i].GetDistance(); //calc new phero
				
				//reinforce best path
				if (bestant != -1 && i == bestant) {
					phero += ADD_PHERO;
				}

				edgelist.SetPhero(colony[i].GetTown(j-1), colony[i].GetTown(j), phero);
			}
			
			//loop back 
			phero = edgelist.GetPhero(colony[i].GetTown(NUM_TOWNS-1), colony[i].GetTown(0)); //get old phero
			phero = phero + QUE / colony[i].GetDistance(); //calc new phero
			
			//reinforce best path
			if (bestant != -1 && i == bestant) {
				phero += ADD_PHERO;
				bestant = -1;
			}

			edgelist.SetPhero(colony[i].GetTown(NUM_TOWNS-1), colony[i].GetTown(0), phero); //set phero
		}

		
		/////////////////////////////
		//reset ants and do it again
		/////////////////////////////
		for (i=0;i<NUM_ANTS;i++) {
			
			colony[i].ResetAnt();
			
		}
				
		//cycle done
		
		cycles ++;
		Draw(cycles, currtour, besttour);

	}

	clear_keybuf();
	textprintf_centre(screen,font,SCREEN_W/2,SCREEN_H/2-60,makecol(255,0,0),"Best Tour = %d",besttour);
	textout_centre(screen,font,"Press a key to exit",SCREEN_W/2,SCREEN_H/2-40,makecol(255,0,0));
	readkey();

	return 0;
}

void Draw(int cycles, int currtour, int besttour) {

	/////////////////////////////////////////////////////////////////////////////////////////////
	// draw a graph (drawing towns and paths is too hard with the current system of random distances)
	// is graph of tour length at each step as well as the current best tour length
	//
	// One important thing with draw() is that it takes the current and best tour lengths and divides them
	// by the number of towns.. this is so that no matter how many towns there are, it wont draw off the screen
	// unless the MAX_DISTANCE is set over 500
	//
	/////////////////////////////////////////////////////////////////////////////////////////////
	acquire_screen();
	
	//graph axis
	vline(screen, 40, 40, SCREEN_H-40, makecol(255,255,255));
	hline(screen, 40, 440, SCREEN_W-40, makecol(255,255,255));
	
	//graph axis text
	textout_centre(screen,font,"CYCLES",SCREEN_W/2, SCREEN_H-20,makecol(0,200,200));
	textout(screen,font,"DISTANCE",1, SCREEN_H/2,makecol(0,200,200));

	//graph legend
	rectfill(screen, SCREEN_W-200, 20, SCREEN_W-170, 40, makecol(0,200,200));
	textprintf(screen,font,SCREEN_W-160, 30,makecol(255,255,255),"Current Tour");
	rectfill(screen, SCREEN_W-200, 50, SCREEN_W-170, 70, makecol(255,0,0));
	textprintf(screen,font,SCREEN_W-160, 60,makecol(255,255,255),"Best Tour");
	
	//draw current cycles distance
	vline(screen,cycles+41,SCREEN_H-40,(SCREEN_H-40)-(currtour/NUM_TOWNS),makecol(0,200,200));
	vline(screen,cycles+41,SCREEN_H-40,(SCREEN_H-40)-(besttour/NUM_TOWNS),makecol(255,0,0));
	

	release_screen();

	
}


END_OF_MAIN();

