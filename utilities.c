#include "tar.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


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
	for(i=0; i<len; i++) s[i] = rotar_char(s[i], desp%8);
}

void descifrar_string(char * s, int len, int desp){
	int i;
	for(i=0; i<len; i++) s[i] = rotar_char(s[i], 8-(desp%8));
}

char* make_path( char* path , char* name )
{
	char *ret = (char *)malloc( strlen(path) + strlen(name) + 1);
	if ( strlen(path) == 0 )
	{
		strcpy( ret , name );	
	}
	else
	{
		strcpy( ret , path );
		ret[strlen(path)] = '/';
		strcat( ret, name);
	}
	return ret;
}

int leer_aux( int fd , char* buf , int l )
{
	int l2;
	l2 = 0;
	while ( l2 < l )
	{
		l2 = l2 + read( fd , buf + l2 , l - l2 );
	}
	return 0;
}

int str_to_int( char* c  )
{
	int x, i;
	x = 0;
	for( i = 3 ; i >= 0 ; i-- )
	{
		x = x << 8;
		x = x | c[i];
	}
	return x;
}