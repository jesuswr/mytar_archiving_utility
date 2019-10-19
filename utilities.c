#include "tar.h"
#include <unistd.h>


void int_to_char(int x, char * ret){
	int i;
	for(i=0; i<4; i++){
		ret[i] = (x)&255;
		x >>= 8;
	}
}

void write_aux(int fd, int len, char * buf){
	int len2 = 0;
	while(len2 < len){
		len2 += write(fd, buf + len2, len-len2);
	}
}

char rotar_char(char a, int desp){
	int i;
	a = ( a << desp ) | ( a >> 8 - desp );
	return a;
}

void cifrar_string(char * s, int len, int desp){
	int i;
	for(i=0; i<len; i++) s[i] = rotar_char(s[i], desp);
}

void descifrar_string(char * s, int len, int desp){
	int i;
	for(i=0; i<len; i++) s[i] = rotar_char(s[i], 8-desp);
}