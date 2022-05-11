#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thrasher.h"

int main(int argc, char** argv){
	int _row = atoi(argv[1]);
	int _col = atoi(argv[2]);
	char* buffer = (char*)malloc((_row*2+1)*(_col*2+1)*sizeof(char)+10000);
	thrasher_generate(buffer, _row,_col);
	for(int i = 0; i < _row*2+1; i++){
		for(int j = 0; j < _col*2+1; j++){
			putc(buffer[i*(_col*2+1)+j],stdout);
		}
		puts("");
	}
}
