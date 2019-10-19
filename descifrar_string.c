/*
	funcion para descifrar strings.
*/

#include "tar.h"
#include <stdio.h>
#include <string.h>

char rotar_char_der(char a, int desp){
	int i;

	a = ( a >> desp ) | ( a << 8 - desp );

	return a;
}

void descifrar_string(char * s, int len, int desp){
	int i;
	for(i=0; i<len; i++) s[i] = rotar_char_der(s[i], desp);
}

int main( int argc , char** argv)
{
	int i,j;
	int n = atoi( argv[2 ]);
	int d = atoi( argv[3]);
	printf("%s\n",argv[1]);
	for( i = 0 ; i < n ; i++)
	{
		for( j = 7 ; j >= 0 ; j--)
		{
			if ( argv[1][i] & (1 << j ) ) printf("1");
			else printf("0");
		}
		printf("  ");
	}
	printf("\n");
	descifrar_string( argv[1] , n , d );
	printf("%s\n",argv[1]);
	for( i = 0 ; i < n ; i++)
	{
		for( j = 7 ; j >= 0 ; j--)
		{
			if ( argv[1][i] & (1 << j ) ) printf("1");
			else printf("0");
		}
		printf("  ");
	}
	printf("\n");

}