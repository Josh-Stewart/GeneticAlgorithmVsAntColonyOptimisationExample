//main.cpp
#define USE_CONSOLE

#include "global.h" // TSP algorithm specific defines are in here
#include "edge.h"
#include "tour.h"

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
	CEdge edgelist;	
	CTour generation[POP];
	CTour breeding[POP];

	int besttour = MAX_DISTANCE*NUM_TOWNS; //best result ever
	int currtour = MAX_DISTANCE*NUM_TOWNS; //best result from last tour

	int i,j; //universal for loop counters

	int cycles=0;

	srand(time(NULL));    //random number stuff
	int limit = NUM_TOWNS; 


	// initialise the population with random valid tours		
	for (i=0;i<POP;i++) { //for each element in generation			
		for (j=0;j<NUM_TOWNS;j++) {  //swap towns around randomly lots
			generation[i].SwapTowns(rand()%limit,rand()%limit);
		}
	}		
		
	// Start Cycling
	while (!key[KEY_ESC] && cycles < MAX_CYCLES) {
		
		textprintf(screen,font,5,5,makecol(255,0,0),"Press Esc to hault processing [Cycle %d]",cycles);
		
		// calculate fitness of generation by
		// computing length of each tour
		
		for (i=0;i<POP;i++) { //for each member in population
			
			int distance = 0;
			
			//reset currtour
			currtour = MAX_DISTANCE*NUM_TOWNS;
			
			printf("\ntour %d [%d",i,generation[i].GetTown(0));//display the tour in console window
			
			for (int j=1;j<NUM_TOWNS;j++) { //for each edge in the tour
				
				printf(",%d",generation[i].GetTown(j));//display the tour in console window
				distance += edgelist.GetDistance(generation[i].GetTown(j-1), generation[i].GetTown(j));
				
			}
			
			printf("]");//display the tour in console window
			
			//add on loopback distance
			distance += edgelist.GetDistance(generation[i].GetTown(NUM_TOWNS-1), generation[i].GetTown(0));
			
			printf("=%d",distance); //display the tour

			//keep record of best tours
			if (distance < besttour)
				besttour = distance;
			if (distance < currtour)
				currtour = distance;
			
			//store distance on tour
			generation[i].SetDistance(distance);
		}

		//tornament selection
		//pairs are selected at random.. the best of the two is added to the breeding program :)
		for (i=0;i<POP;i++) {
			
			int pos1 = rand()%limit;
			int pos2 = rand()%limit;

			if (generation[pos1].GetDistance() < generation[pos2].GetDistance()) 
				breeding[i] = generation[pos1];
			else
				breeding[i] = generation[pos2];

		}


		//crossover to make new generation (using PMX)
		for (i=0;i<POP;i+=2) { //process a pair at a time
			
			limit = 100;

			if (rand()%limit < XOVER_PROB) {
				
				limit = NUM_TOWNS/2;
				int xover1 = rand()%limit;         // xover point 1
				int xover2 = xover1 + rand()%limit; // xover point 2
				
				//do crossover
				for (j=xover1;j<=xover2;j++) {

					int t1 = breeding[i].GetTown(j);
					int t2 = breeding[i+1].GetTown(j);

					breeding[i].SetTown(j,t2);
					breeding[i+1].SetTown(j,t1);
				}
			}
			
			//mutate i
			limit = 100;
			if (rand()%limit < MUTATE_PROB) {
				
				limit = NUM_TOWNS;

				int pos = rand()%limit; //randomly select position
				
				breeding[i].SetTown(pos,rand()%limit);
			}
			
			//mutate i+1
			limit = 100;
			if (rand()%limit < MUTATE_PROB) {
				
				limit = NUM_TOWNS;

				int pos = rand()%limit; //randomly select position
				
				breeding[i+1].SetTown(pos,rand()%limit);
			}
			
			//save results to pop
			generation[i] = breeding[i];
			generation[i+1] = breeding[i+1];
		
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

