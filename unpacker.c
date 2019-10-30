#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include "tar.h"
#include "utilities.h"

int leer_aux( int fd , unsigned char* buf , int l )
{
	int l2;
	l2 = 0;
	while ( l2 < l )
	{
		l2 = l2 + read( fd , buf + l2 , l - l2 );
	}
	return 0;
}

int str_to_int(unsigned char* c  )
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

void int_to_char(int x, unsigned char * ret){
	int i;
	for(i=0; i<4; i++){
		ret[i] = (x)&255;
		x >>= 8;
	}
}

void write_aux(int fd, int len, unsigned char * buf){
	int len2 = 0;
	while(len2 < len){
		len2 += write(fd, buf + len2, len-len2);
	}
}


int loaddata(int fd1 , int fd2 , int size){
	unsigned char *buff;
	buff = (unsigned char*)malloc(size);
	leer_aux(fd1, buff, size);
	write_aux(fd2, size, buff);
	free(buff);
}

int read_header( int fd , header* h ){
	/* RECORDAR LEER LA CASILLA LIBRE ANTES DE LLAMAR LA FUNCION */
	

	unsigned char buf[4];
	unsigned char *buf2, *buf3;
	int e;

	e = leer_aux( fd , buf , 4 );
	h->modo = str_to_int( buf );

	e = leer_aux( fd , buf , 4 );
	h->uid = str_to_int( buf );

	e = leer_aux( fd , buf , 4 );
	h->gid = str_to_int( buf );
	
	e = leer_aux( fd , buf , 4 );
	h->size = str_to_int( buf );

	e = leer_aux( fd , buf , 4 );
	h->num_blocks = str_to_int( buf );

	e = leer_aux( fd , buf , 4 );
	h->name_size = str_to_int( buf );

	e = leer_aux( fd , buf , 4 );
	h->link_size = str_to_int( buf );

	buf2 = (unsigned char*)malloc( h->name_size );
	e = leer_aux( fd , buf2 , h->name_size );

	h->name = buf2;
	if( h->link_size > 0 ){
		buf3 = (unsigned char*)malloc( h->link_size );
		e = leer_aux( fd , buf3 , h->link_size);
		h->link_path = buf3;
	}

	return 0;
}

int unpack( unsigned char * packed_file){
	
	int fd, fd2;
	header h;
	unsigned char buf;

	fd = open(packed_file, O_RDWR);

	while ( read(fd , &buf, 1)==1 && (buf=='D' || buf=='C') ){


		read_header(fd, &h);


		if ( (h.modo & __S_IFMT) == __S_IFDIR ){
			mkdir(h.name, h.modo);
		}
		if ( (h.modo & __S_IFMT ) == __S_IFLNK ){
			symlink(h.link_path, h.name);
		}
		if ( ( h.modo & __S_IFMT ) == __S_IFIFO ){
			mkfifo(h.name, h.modo);
		}
		if ( (h.modo & __S_IFMT) == __S_IFREG ){
			fd2 = open( h.name , O_RDWR | O_CREAT | O_TRUNC, h.modo );
			loaddata( fd , fd2 , h.size);
			close(fd2);
		}

	}

}


int main(){

	unpack("dirEquis/mytar");

}