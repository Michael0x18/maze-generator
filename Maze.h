#ifndef MAZE_H
#define MAZE_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#define MAZE_VISITED 1
#define MAZE_OPEN_RIGHT 2
#define MAZE_OPEN_DOWN 4

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
//#define MIDDLE_PRINT

static void print_maze(int8_t *buf, int16_t mr, int16_t mc){
	gotoxy(0, 0);
	fputc('#', stdout);
	for(int i = 0; i < mc; ++i){
		fputc('#', stdout);
		fputc('#', stdout);
	}
	puts("");
	for(int r = 0; r < mr; r++){
		fputc('#', stdout);
		// Process lr
		for(int c = 0; c < mc; c++){
			int8_t d = buf[r * mc + c];
			fputc(d&MAZE_VISITED ? ' ' : '#', stdout);
			fputc(d&MAZE_OPEN_RIGHT ? ' ':'#', stdout);
		}
		puts("");
		fputc('#', stdout);
		for(int c = 0; c < mc; c++){
			int8_t d = buf[r*mc+c];
			fputc(d&MAZE_OPEN_DOWN ? ' ':'#', stdout);
			fputc('#', stdout);
		}
		puts("");
	}
}

static void mk_maze_internal(int8_t *buf, int16_t r, int16_t c, int16_t mr, int16_t mc, int(*rf)(void)){
#ifdef MIDDLE_PRINT
	usleep(500);
	print_maze(buf, mr, mc);
#endif
	int8_t *cpos = buf + r * mc + c;
	if(*cpos & MAZE_VISITED)
		return;
	// Mark current visited
	*cpos |= MAZE_VISITED;
	// Recurse in random direction
	int16_t ra;
	
	int8_t valid_dirs = 0;
top:
	ra = rand();
	ra %= 4;
	valid_dirs = 0;
	valid_dirs |= (r > 0 && !(MAZE_VISITED&*(cpos-mc)));
	valid_dirs |= (r < mr-1 && !(MAZE_VISITED&*(cpos+mc))) << 1;
	valid_dirs |= (c < mc-1 && !(MAZE_VISITED&*(cpos+1))) << 2;
	valid_dirs |= (c > 0 && !(MAZE_VISITED&*(cpos-1))) << 3;

	if(!valid_dirs)
		return;

	switch(ra){
	case 0: // UP
		if(valid_dirs&1){
			*(cpos-mc) |= MAZE_OPEN_DOWN;
			mk_maze_internal(buf, r-1, c, mr, mc, rf);
		};
		break;
	case 1: // DOWN
		if(valid_dirs&2){
			*(cpos) |= MAZE_OPEN_DOWN;
			mk_maze_internal(buf, r+1, c, mr, mc, rf);
		};
		break;
	case 2: // RIGHT
		if(valid_dirs&4){
			*(cpos) |= MAZE_OPEN_RIGHT;
			mk_maze_internal(buf, r, c+1, mr, mc, rf);
		};
		break;
	case 3:
		if( c > 0 && valid_dirs&8){
			*(cpos-1) |= MAZE_OPEN_RIGHT;
			mk_maze_internal(buf, r, c-1, mr, mc, rf);
		};
		break;
	}
	goto top;// Recurse again
}

void mk_maze(int8_t *buf, int16_t r, int16_t c, int(*rand_func)(void)){
	memset(buf, 0, r * c);
	mk_maze_internal(buf, 0, 0, r, c, rand_func);
}

#endif
