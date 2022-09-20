#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gfx.h"

void delay(int sec);
void scores(int *score);
void repeatorquit(int *fill, int *ptr, int *score);
void reset(int *fill, int *ptr);
void drawX(int x, int y);
int xo(int x, int y, int c, int *w, int a);
int checkfill(int *w);
int checkfill2(int *w);
void checkwin(int *w, int a, int *ptr, int *score);
int block(int *fill);
int filler(int *fill);
int comp(int *fill, int lvl, int move);
void lvlScreen();

int main()
{
	int ysize = 720, xsize = 720;	//game window size
	int x, y; char c;	//gfx event waiting var
	int cursor = 0;	//var to determine x or o shape
	int *ptr = &cursor; 	//pointer to cursor value
	int *fill; 		//array to check if box is filled and determine winner
	int com, yes;		//com to put comp move, yes to put checkfill result
	int mode;		//determine pvp or pvcom
	int *score;		//pointer to score value
	int level;		//Vs Com level
	int move;		//Vs Com moveNum
	
	fill = malloc(9 * sizeof(int));
	score = malloc(3 * sizeof(int)); 	//keep score of x & o & draw
	
	for (int i = 0; i < 3; i++){		//score[0] for x, score[1] for o, score[2] for draw
		score[i] = 0;
	}
	
	int xbox[9]; int ybox[9]; //init box size
	
	for (int i = 0; i < 3; i++){
		xbox[i*3] = 120;
		ybox[i] = 120;
	}
	for (int i = 0; i < 3; i++){
		xbox[(i*3)+1] = 360;
		ybox[i+3] = 360;
	}
	for (int i = 0; i < 3; i++){
		xbox[(i*3)+2] = 600;
		ybox[i+6] = 600;
	}

	srand(time(NULL));
	
	gfx_open (xsize,ysize,"TIC TAC TOE");	//opengfx
	
	gfx_color(255,255,255);		//MAIN MENU
	gfx_fillrectangle(270,30,20,120); 	//T
	gfx_fillrectangle(250,30,60,20);
	gfx_fillrectangle(350,30,20,120); 	//I
	gfx_fillrectangle(330,30,60,20);
	gfx_fillrectangle(330,130,60,20);
	gfx_fillrectangle(410,30,60,20); 	//C
	gfx_fillrectangle(410,30,20,120);
	gfx_fillrectangle(410,130,60,20);
	gfx_fillrectangle(270,170,20,120); 	//T
	gfx_fillrectangle(250,170,60,20);
	gfx_fillrectangle(330,170,20,120); 	//A
	gfx_fillrectangle(330,170,60,20);
	gfx_fillrectangle(370,170,20,120);
	gfx_fillrectangle(330,220,60,20);
	gfx_fillrectangle(410,170,60,20); 	//C
	gfx_fillrectangle(410,170,20,120);
	gfx_fillrectangle(410,270,60,20);
	gfx_fillrectangle(270,310,20,120); 	//T
	gfx_fillrectangle(250,310,60,20);
	gfx_fillrectangle(330,310,20,120); 	//O
	gfx_fillrectangle(330,310,60,20);
	gfx_fillrectangle(370,310,20,120);
	gfx_fillrectangle(330,410,60,20);
	gfx_fillrectangle(410,310,60,20); 	//E
	gfx_fillrectangle(410,310,20,120);
	gfx_fillrectangle(410,360,60,20);
	gfx_fillrectangle(410,410,60,20);
	gfx_fillrectangle(100,530,200,100);	//box1
	gfx_fillrectangle(420,530,200,100);	//box2
	gfx_color(0,0,0);
	gfx_fillrectangle(115,540,10,80); 	//box1 p
	gfx_fillrectangle(115,540,50,10);
	gfx_fillrectangle(155,540,10,40);
	gfx_fillrectangle(115,570,50,10);
	gfx_fillrectangle(175,540,10,40); 	//box1 v
	gfx_fillrectangle(215,540,10,40);
	gfx_fillrectangle(185,580,10,20);
	gfx_fillrectangle(205,580,10,20);
	gfx_fillrectangle(195,600,10,20);
	gfx_fillrectangle(235,540,10,80); 	//box1 p
	gfx_fillrectangle(235,540,50,10);
	gfx_fillrectangle(275,540,10,40);
	gfx_fillrectangle(235,570,50,10);
	gfx_fillrectangle(435,540,10,80);	//box2 c
	gfx_fillrectangle(435,540,50,10);
	gfx_fillrectangle(435,610,50,10);
	gfx_fillrectangle(495,540,10,80);	//box2 o
	gfx_fillrectangle(495,540,50,10);
	gfx_fillrectangle(535,540,10,80);
	gfx_fillrectangle(495,610,50,10);
	gfx_fillrectangle(555,540,10,80);	//box2 m
	gfx_fillrectangle(595,540,10,80);
	gfx_fillrectangle(565,550,10,10);
	gfx_fillrectangle(585,550,10,10);
	gfx_fillrectangle(575,560,10,30);
	
	while(1)				//player choose play mode
	{
		if(gfx_event_waiting()){
			c = gfx_wait();
			x = gfx_xpos();
			y = gfx_ypos();
		
			if(x>100 && x<300 && y>530 && y<630){
				mode = 1;
				break;
			}
			
			if(x>420 && x<620 && y>530 && y<630){
				mode = 2;
				break;
			}
		}
	}
	
	switch (mode) {
		case 1:	//playervsplayer mode
			reset(fill, ptr);
			while(1) { 		
				if(gfx_event_waiting()) {
					c = gfx_wait();			
					x = gfx_xpos();
					y = gfx_ypos();
					
					if(x > 0 && x < 240 && y > 0 && y < 240 && fill[0] == 0){
						cursor = xo(xbox[0], ybox[0], cursor, fill, 0);
					}
					else if(x > 240 && x < 480 && y > 0 && y < 240 && fill[1] == 0){
						cursor = xo(xbox[1], ybox[1], cursor, fill, 1);
					}
					else if(x > 480 && x < 720 && y > 0 && y < 240 && fill[2] == 0){
						cursor = xo(xbox[2], ybox[2], cursor, fill, 2);
					}
					else if(x > 0 && x < 240 && y > 240 && y < 480 && fill[3] == 0){
						cursor = xo(xbox[3], ybox[3], cursor, fill, 3);
					}
					else if(x > 240 && x < 480 && y > 240 && y < 480 && fill[4] == 0){
						cursor = xo(xbox[4], ybox[4], cursor, fill, 4);
					}
					else if(x > 480 && x < 720 && y > 240 && y < 480 && fill[5] == 0){
						cursor = xo(xbox[5], ybox[5], cursor, fill, 5);
					}
					else if(x > 0 && x < 240 && y > 480 && y < 720 && fill[6] == 0){
						cursor = xo(xbox[6], ybox[6], cursor, fill, 6);
					}
					else if(x > 240 && x < 480 && y > 480 && y < 720 && fill[7] == 0){
						cursor = xo(xbox[7], ybox[7], cursor, fill, 7);
					}
					else if(x > 480 && x < 720 && y > 480 && y < 720 && fill[8] == 0){
						cursor = xo(xbox[8], ybox[8], cursor, fill, 8);
					}
					yes = checkfill(fill);
					checkwin(fill, yes, ptr, score);
				}
			}
			break;
		case 2:			//playervscomp mode
			lvlScreen();		//screen for selecting level
			while(1) { 		//select level for Vs Com
				if(gfx_event_waiting()){
					c = gfx_wait();
					x = gfx_xpos();
					y = gfx_ypos();
				
					if(x>30 && x<230 && y>530 && y<630){
						level = 1;
						break;
					}
					
					if(x>260 && x<460 && y>530 && y<630){
						level = 2;
						break;
					}
					
					if(x>460 && x<660 && y>530 && y<630){
						level = 3;
						break;
					}
				}
			}
			
			reset(fill, ptr);
			
			while(1) {
				if(checkfill2(fill) == 0) {
					move = 0;
				}		
				if(gfx_event_waiting() && cursor == 0) {
					c = gfx_wait();			
					x = gfx_xpos();
					y = gfx_ypos();
					
					if(x > 0 && x < 240 && y > 0 && y < 240 && fill[0] == 0){
						cursor = xo(xbox[0], ybox[0], cursor, fill, 0);
					}
					else if(x > 240 && x < 480 && y > 0 && y < 240 && fill[1] == 0){
						cursor = xo(xbox[1], ybox[1], cursor, fill, 1);
					}
					else if(x > 480 && x < 720 && y > 0 && y < 240 && fill[2] == 0){
						cursor = xo(xbox[2], ybox[2], cursor, fill, 2);
					}
					else if(x > 0 && x < 240 && y > 240 && y < 480 && fill[3] == 0){
						cursor = xo(xbox[3], ybox[3], cursor, fill, 3);
					}
					else if(x > 240 && x < 480 && y > 240 && y < 480 && fill[4] == 0){
						cursor = xo(xbox[4], ybox[4], cursor, fill, 4);
					}
					else if(x > 480 && x < 720 && y > 240 && y < 480 && fill[5] == 0){
						cursor = xo(xbox[5], ybox[5], cursor, fill, 5);
					}
					else if(x > 0 && x < 240 && y > 480 && y < 720 && fill[6] == 0){
						cursor = xo(xbox[6], ybox[6], cursor, fill, 6);
					}
					else if(x > 240 && x < 480 && y > 480 && y < 720 && fill[7] == 0){
						cursor = xo(xbox[7], ybox[7], cursor, fill, 7);
					}
					else if(x > 480 && x < 720 && y > 480 && y < 720 && fill[8] == 0){
						cursor = xo(xbox[8], ybox[8], cursor, fill, 8);
					}
					yes = checkfill(fill);
					checkwin(fill, yes, ptr, score);

				} else if(cursor == 1) {
					delay(500);
					com = comp(fill, level, move);
					cursor = xo(xbox[com], ybox[com], cursor, fill, com);
					yes = checkfill(fill);
					checkwin(fill, yes, ptr, score);
					move++;
				}
			}
			break;	
		default:
			break;
	}
	
	return(0);
}

void delay(int sec) { //delay time function
	int milli_seconds = 1000*sec;
	clock_t start = clock();
	while(clock() < start + milli_seconds){
		//printf("-");
	}
}

void scores(int *score){	//print scores on screen

	char inttostr[10] = "";
	
	gfx_text("SCORE:",0,10);
	gfx_text("X =",0,30);
	sprintf(inttostr,"%d",score[0]);
	gfx_text(inttostr,25,30);
	gfx_text("O =",0,45);
	sprintf(inttostr,"%d",score[1]);
	gfx_text(inttostr,25,45);
	gfx_text("DRAW =",0,60);
	sprintf(inttostr,"%d",score[2]);
	gfx_text(inttostr,45,60);
	
}

void repeatorquit(int *fill, int *ptr, int *score){ //page to ask user to continue or quit
	int x,y; char c;
	
	gfx_clear();
	gfx_color(255,255,255);
	gfx_fillrectangle(170,170,20,120);	//c
	gfx_fillrectangle(170,170,60,20);
	gfx_fillrectangle(170,270,60,20);
	gfx_fillrectangle(250,170,20,120);	//o
	gfx_fillrectangle(250,170,60,20);
	gfx_fillrectangle(290,170,20,120);
	gfx_fillrectangle(250,270,60,20);
	gfx_fillrectangle(330,170,20,120);	//n
	gfx_fillrectangle(330,170,60,20);
	gfx_fillrectangle(370,170,20,120);
	gfx_fillrectangle(410,170,60,20);	//t
	gfx_fillrectangle(430,170,20,120);
	gfx_fillrectangle(490,170,60,20);	//?
	gfx_fillrectangle(530,170,20,60);
	gfx_fillrectangle(510,220,20,40);
	gfx_fillrectangle(510,270,20,20);
	gfx_fillrectangle(100,530,200,100); 	//box 1
	gfx_fillrectangle(420,530,200,100); 	//box 2
	gfx_color(0,0,0);
	gfx_fillrectangle(175,540,10,20); 	//box 1 y
	gfx_fillrectangle(215,540,10,20);
	gfx_fillrectangle(185,560,10,20);
	gfx_fillrectangle(205,560,10,20);
	gfx_fillrectangle(195,580,10,40);
	gfx_fillrectangle(495,540,10,80); 	//box 2 n
	gfx_fillrectangle(535,540,10,80);
	gfx_fillrectangle(505,550,10,20);
	gfx_fillrectangle(515,570,10,20);
	gfx_fillrectangle(525,590,10,20);
	gfx_color(255,255,255);
	
	scores(score);
	
	while(1){
		if(gfx_event_waiting()){
			c = gfx_wait();
			x = gfx_xpos();
			y = gfx_ypos();
			
			if(x>100 && x<300 && y>530 && y<630){
				reset(fill, ptr);
				break;
			}
			
			if(x>420 && x<620 && y>530 && y<630){
				exit(1);
			}
		}
	}
}

void reset(int *fill, int *ptr){	//reset fill array & draw blank table on screen
	gfx_clear();
	
	for (int i = 0; i<9; i++){ 
		fill[i] = 0;
	}
	
	gfx_color(255,255,255);
	gfx_line(240,0,240,720);
	gfx_line(480,0,480,720);
	gfx_line(0,240,720,240);
	gfx_line(0,480,720,480);
	
	*ptr = 0;
}

void drawX(int x, int y){	//draw X shape on screen
	gfx_color(255,255,255);
	gfx_line(x-50,y-50,x+50,y+50);
	gfx_line(x+50,y-50,x-50,y+50);
}

int xo(int x, int y, int c, int *w, int a){	//func to determine to draw x or o on screen
	
	if (c == 0){
		drawX(x,y);
		c = !c;
		w[a] = 2;
	} else {
		gfx_circle(x,y,150);
		c = !c;
		w[a] = 3;
	}
	return c;
}

int checkfill(int *w){		//return 1 if any element in array is empty 
	int y = 0;

	for(int i = 0; i < 9; i++){
		if(w[i] == 0){
			y = 1;
			break;
		}
	}
	return y;	
}

int checkfill2(int *w){	//return 1 if any element in array is not empty  
	int y = 0;

	for(int i = 0; i < 9; i++){
		if(w[i] != 0){
			y = 1;
			break;
		}
	}
	return y;	
}

void checkwin(int *w, int a, int *ptr, int *score){		//check winning condition
	
	if( (w[0] == 2 && w[1] == 2 && w[2] == 2) ||  	//X WIN CONDITION
	    (w[0] == 2 && w[4] == 2 && w[8] == 2) ||
	    (w[0] == 2 && w[3] == 2 && w[6] == 2) ||
	    (w[1] == 2 && w[4] == 2 && w[7] == 2) ||
	    (w[3] == 2 && w[4] == 2 && w[5] == 2) ||
	    (w[2] == 2 && w[5] == 2 && w[8] == 2) ||
	    (w[2] == 2 && w[4] == 2 && w[6] == 2) ||
	    (w[6] == 2 && w[7] == 2 && w[8] == 2) ) {
		printf("X WIN\n");
		score[0]++;
		repeatorquit(w, ptr, score);
	} else if( (w[0] == 3 && w[1] == 3 && w[2] == 3) ||   //O WIN CONDITION
		   (w[0] == 3 && w[4] == 3 && w[8] == 3) ||
		   (w[0] == 3 && w[3] == 3 && w[6] == 3) ||
		   (w[1] == 3 && w[4] == 3 && w[7] == 3) ||
		   (w[3] == 3 && w[4] == 3 && w[5] == 3) ||
		   (w[2] == 3 && w[5] == 3 && w[8] == 3) ||
		   (w[2] == 3 && w[4] == 3 && w[6] == 3) ||
		   (w[6] == 3 && w[7] == 3 && w[8] == 3) ) {
		printf("O WIN\n");
		score[1]++;
		repeatorquit(w, ptr, score);
	} else if(a == 0){
		printf("DRAW\n");
		score[2]++;
		repeatorquit(w, ptr, score);
	}
	
}

int block(int *fill){		//Comp strat- put O at beside X exclude center
	int y = 0;
	if(fill[0] == 2) {
		if(fill[1] == 0) {
			y = 1;
		} else if(fill[3] == 0) {
			y = 3;
		}
	}  else if(fill[1] == 2) {
		if(fill[0] == 0) {
			y = 0;
		} else if(fill[2] == 0) {
			y = 2;
		}
	} else if(fill[2] == 2) {
		if(fill[1] == 0) {
			y = 1;
		} else if(fill[5] == 0) {
			y = 5;
		}
	} else if(fill[3] == 2) {
		if(fill[0] == 0) {
			y = 0;
		} else if(fill[6] == 0) {
			y = 6;
		}
	} else if(fill[5] == 2) {
		if(fill[2] == 0) {
			y = 2;
		} else if(fill[8] == 0) {
			y = 8;
		}
	} else if(fill[6] == 2) {
		if(fill[3] == 0) {
			y = 3;
		} else if(fill[7] == 0) {
			y = 7;
		}
	} else if(fill[7] == 2) {
		if(fill[6] == 0) {
			y = 6;
		} else if(fill[8] == 0) {
			y = 8;
		}
	} else if(fill[8] == 2) {
		if(fill[0] == 0) {
			y = 0;
		} else if(fill[5] == 0) {
			y = 5;
		} else if(fill[7] == 0) {
			y = 7;
		}
	} else {
		while(1){
			y = rand() % 9;		
			if(fill[y] == 0){
				break;
			}
		}
	}
	return y;
}

int filler(int*fill){			//Comp strat- fill in between or end of streak 
	int y = 0;
	if(fill[0] == 3 && fill[1] == 3 && fill[2] == 0)	//o0
		y = 2;
	else if(fill[0] == 3 && fill[2] == 3 && fill[1] == 0)
		y = 1;
	else if (fill[0] == 3 && fill[3] == 3 && fill[6] == 0)
		y = 6;
	else if (fill[0] == 3 && fill[6] == 3 && fill[3] == 0)
		y = 3;
	else if (fill[0] == 3 && fill[4] == 3 && fill[8] == 0)
		y = 8;
	else if (fill[0] == 3 && fill[6] == 3 && fill[4] == 0)
		y = 4;
	else if (fill[1] == 3 && fill[2] == 3 && fill[0] == 0)	//o1
		y = 0;
	else if (fill[1] == 3 && fill[4] == 3 && fill[7] == 0)
		y = 7;
	else if (fill[1] == 3 && fill[7] == 3 && fill[4] == 0)
		y = 4;
	else if (fill[2] == 3 && fill[4] == 3 && fill[6] == 0)	//o2
		y = 6;
	else if (fill[2] == 3 && fill[5] == 3 && fill[8] == 0)
		y = 8;
	else if (fill[2] == 3 && fill[6] == 3 && fill[4] == 0)
		y = 4;
	else if (fill[2] == 3 && fill[8] == 3 && fill[5] == 0)
		y = 5;
	else if (fill[3] == 3 && fill[4] == 3 && fill[5] == 0)	//o3
		y = 5;
	else if (fill[3] == 3 && fill[5] == 3 && fill[4] == 0)
		y = 4;
	else if (fill[3] == 3 && fill[6] == 3 && fill[0] == 0)
		y = 0;
	else if (fill[4] == 3 && fill[5] == 3 && fill[3] == 0)	//o4
		y = 3;
	else if (fill[4] == 3 && fill[6] == 3 && fill[2] == 0)
		y = 2;
	else if (fill[4] == 3 && fill[7] == 3 && fill[1] == 0)
		y = 1;
	else if (fill[4] == 3 && fill[8] == 3 && fill[0] == 0)
		y = 0;
	else if (fill[5] == 3 && fill[8] == 3 && fill[2] == 0)	//o5
		y = 2;
	else if (fill[6] == 3 && fill[7] == 3 && fill[8] == 0)	//o6
		y = 8;
	else if (fill[6] == 3 && fill[8] == 3 && fill[7] == 0)
		y = 7;
	else if (fill[7] == 3 && fill[8] == 3 && fill[6] == 0)	//o7
		y = 6;
	else if(fill[0] == 2 && fill[1] == 2 && fill[2] == 0)	//x0
		y = 2;
	else if(fill[0] == 2 && fill[2] == 2 && fill[1] == 0)
		y = 1;
	else if (fill[0] == 2 && fill[3] == 2 && fill[6] == 0)
		y = 6;
	else if (fill[0] == 2 && fill[6] == 2 && fill[3] == 0)
		y = 3;
	else if (fill[0] == 2 && fill[4] == 2 && fill[8] == 0)
		y = 8;
	else if (fill[0] == 2 && fill[6] == 2 && fill[4] == 0)
		y = 4;
	else if (fill[1] == 2 && fill[2] == 2 && fill[0] == 0)	//x1
		y = 0;
	else if (fill[1] == 2 && fill[4] == 2 && fill[7] == 0)
		y = 7;
	else if (fill[1] == 2 && fill[7] == 2 && fill[4] == 0)
		y = 4;
	else if (fill[2] == 2 && fill[4] == 2 && fill[6] == 0)	//x2
		y = 6;
	else if (fill[2] == 2 && fill[5] == 2 && fill[8] == 0)
		y = 8;
	else if (fill[2] == 2 && fill[6] == 2 && fill[4] == 0)
		y = 4;
	else if (fill[2] == 2 && fill[8] == 2 && fill[5] == 0)
		y = 5;
	else if (fill[3] == 2 && fill[4] == 2 && fill[5] == 0)	//x3
		y = 5;
	else if (fill[3] == 2 && fill[5] == 2 && fill[4] == 0)
		y = 4;
	else if (fill[3] == 2 && fill[6] == 2 && fill[0] == 0)
		y = 0;
	else if (fill[4] == 2 && fill[5] == 2 && fill[3] == 0)	//x4
		y = 3;
	else if (fill[4] == 2 && fill[6] == 2 && fill[2] == 0)
		y = 2;
	else if (fill[4] == 2 && fill[7] == 2 && fill[1] == 0)
		y = 1;
	else if (fill[4] == 2 && fill[8] == 2 && fill[0] == 0)
		y = 0;
	else if (fill[5] == 2 && fill[8] == 2 && fill[2] == 0)	//x5
		y = 2;
	else if (fill[6] == 2 && fill[7] == 2 && fill[8] == 0)	//x6
		y = 8;
	else if (fill[6] == 2 && fill[8] == 2 && fill[7] == 0)
		y = 7;
	else if (fill[7] == 2 && fill[8] == 2 && fill[6] == 0)	//x7
		y = 6;
	else {
		while(1){
			y = rand() % 9;		
			if(fill[y] == 0){
				break;
			}
		}
	}
	return y;
}

int comp(int *fill, int lvl, int move){	//Comp decide which place to put o
	int place;
	switch (lvl) {				//strategy diff based on level
		case 1:
			while(1){
				place = rand() % 9;		
				if(fill[place] == 0){
					break;
				}
			}
			break;
		case 2:
			if(move == 0) {
				place = block(fill);
				break;
			} else {
				place = filler(fill);
				break;
			}
			break;
		case 3:
			if(move == 0) {
				if(fill[4] == 0){
					place = 4;
				} else {
					place = 0;
				}
			} else {
				place = filler(fill);
				break;
			}
			break;
		default:
			break;
	}
	return place;
}

void lvlScreen(){
	gfx_clear();
	gfx_color(255,255,255);		
	gfx_fillrectangle(30,530,200,100); 	//Level 1
	gfx_fillrectangle(260,530,200,100);	//Level 2
	gfx_fillrectangle(490,530,200,100); 	//Level 3
	gfx_fillrectangle(130,170,20,120);	//L
	gfx_fillrectangle(130,270,100,20);
	gfx_fillrectangle(250,170,20,60);	//V
	gfx_fillrectangle(330,170,20,60);
	gfx_fillrectangle(270,230,20,40);
	gfx_fillrectangle(310,230,20,40);
	gfx_fillrectangle(290,270,20,20);
	gfx_fillrectangle(370,170,20,120);	//L
	gfx_fillrectangle(370,270,100,20);
	gfx_fillrectangle(490,170,100,20);	//?
	gfx_fillrectangle(490,170,20,40);
	gfx_fillrectangle(570,170,20,40);
	gfx_fillrectangle(530,210,60,20);
	gfx_fillrectangle(530,210,20,40);
	gfx_fillrectangle(530,270,20,20);
	gfx_color(0,0,0);
	gfx_fillrectangle(120,540,20,80);	//i
	gfx_fillrectangle(330,540,20,80);	//ii
	gfx_fillrectangle(370,540,20,80);
	gfx_fillrectangle(540,540,20,80);	//iii
	gfx_fillrectangle(580,540,20,80);
	gfx_fillrectangle(620,540,20,80);
}
