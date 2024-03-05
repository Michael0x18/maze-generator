#include<stdio.h>
#include<stdlib.h>

#include "Maze.h"

#define x 100
#define y 100

int main(void){
	srand(time(NULL));
	static char buf[x * y];
	mk_maze(buf, x, y, rand);
	print_maze(buf, x, y);
}
