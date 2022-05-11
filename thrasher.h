#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <fcntl.h>

struct thrasher_node{
	struct thrasher_node* prev;
	int x;
	int y;
};

void thrasher_generate(char* buffer, int r, int c){
	const int DIR_UP = 0, DIR_DOWN = 1, DIR_LEFT = 2, DIR_RIGHT = 3;
	int stack_depth = 0;
	memset(buffer,'!',((2*r+1)*(2*c+1)*sizeof(char)));
	char* visited = (char*)malloc(r*c*sizeof(char));
	memset(visited,0,r*c);
	struct thrasher_node* root = (struct thrasher_node*) malloc(sizeof(struct thrasher_node));
	root->prev=NULL;
	root->x=0;
	root->y=0;
	struct thrasher_node* current = root;
	stack_depth=1;
	int i, rs = open("/dev/urandom",O_RDONLY);
	while(stack_depth){
		visited[current->y*c+current->x]=1;
		//int offset = (c*2+1)*2*(0+current->y)+current->x*2+1;
		int offset = (c*2+1)*(current->y*2+1)+current->x*2+1;
		buffer[offset]=' ';
		int directions[4];
		int numdirs = 0;
		if(current->y>0 && !visited[(current->y-1)*c+current->x]){
			directions[numdirs++]=DIR_UP;
		}
		if(current->x>0 && !visited[current->y*c+current->x-1]){
			directions[numdirs++]=DIR_LEFT;
		}
		if(current->x<c-1 && !visited[current->y*c+current->x+1]){
			directions[numdirs++]=DIR_RIGHT;
		}
		if(current->y<r-1 && !visited[(current->y+1)*c+current->x]){
			directions[numdirs++]=DIR_DOWN;
		}
		if(!numdirs){
			if(stack_depth==1){
				break;
			}
			struct thrasher_node* oldcurrent = current;
			current = current->prev;
			stack_depth--;
			free(oldcurrent);
			continue;
		}
		unsigned int random;
		read(rs , &random, sizeof(unsigned int));
		random%=numdirs;
		int dir = directions[random];
		struct thrasher_node* new_node = (struct thrasher_node*)malloc(sizeof(struct thrasher_node));
		switch(dir){	
			case 0:
				//DIR_UP
				new_node->x = current->x;
				new_node->y = current->y-1;
				buffer[offset-(2*(c)+1)]=' ';
				break;
			case 1:
				//DIR_DOWN
				new_node->x = current->x;
				new_node->y = current->y+1;
				buffer[offset+(2*(c)+1)]=' ';
				break;
			case 2:
				//DIR_LEFT
				new_node->x = current->x-1;
				new_node->y = current->y;
				buffer[offset-1]=' ';
				break;
			case 3:
				//DIR_RIGHT
				new_node->x = current->x+1;
				new_node->y = current->y;
				buffer[offset+1]=' ';
				break;
		}
		new_node->prev=current;
		current=new_node;
		stack_depth++;
	}
	free(visited);
}
