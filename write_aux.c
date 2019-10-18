/*
	funcion auxiliar write.
*/

#include "tar.h"
#include <unistd.h>


void write_aux(int fd, int len, char * buf){

	int len2 = 0;

	while(len2 < len){
		len2 += write(fd, buf + len2, len-len2);
	}

}

/*
int main( int argc , char** argv ){

	char str[5];
	str[0]=str[1]=str[2]=str[3]=str[4] = 'A';
	write_aux(1,5,str);
}
*/
