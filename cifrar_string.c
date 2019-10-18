/*
	funcion para cifrar strings.
*/

#include "tar.h"

char rotar_char(char a, int desp){
	int i;

	for(i=0;i<desp;i++) a = (char) ((a<<1) | (((1<<7)&a)!=0) )&(255);

	return a;
}

void cifrar_string(char * s, int len, int desp){
	int i;
	for(i=0; i<len; i++) s[i] = rotar_char(s[i], desp);
}