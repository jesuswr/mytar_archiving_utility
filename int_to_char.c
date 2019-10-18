/*
	int_to_char function. return a stream of 4 characters that represents the int x.
*/

#include "tar.h"

char * int_to_char(int x, char * ret){
	
	int i;
	
	for(i=0; i<4; i++){
		ret[i] = (x)&255;
		x >>= 8;
	}

}