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

int loaddata(int fd1 , int fd2 , int size){
	char *buff;
	buff = (char*)malloc(size);
	leer_aux(fd1, buff, size);
	write_aux(fd2, size, buff);
}

int get_header( char* path_and_name , header *h ){
	
	struct stat sb;
	int x, y, l;
	char * c;
	char * c2;

	x = lstat( path_and_name , &sb );

	h->modo = sb.st_mode;
	h->uid = sb.st_uid;
	h->gid = sb.st_gid;
	h->size = sb.st_size;
	h->num_blocks = sb.st_blocks;
	h->name_size = strlen( path_and_name );

	c = ( char * )malloc( h->name_size );
	strcpy( c , path_and_name );

	h->name = c;

	if ( ( sb.st_mode & __S_IFMT ) == __S_IFLNK ){	
		
		c = ( char * )malloc(400);
		l = 0;
		l += readlink( path_and_name , c , 399 );
		c[l] = '\0';
		c2 = ( char * )malloc( strlen(c) );
		strcpy(c2,c);
		h->link_size = l;
		h->link_path = c2;
		free(c);
	
	}else{
	
		h->link_size = 0;
	
	}

	return 0;
}

int read_header( int fd , header* h ){
	/* RECORDAR LEER LA CASILLA LIBRE ANTES DE LLAMAR LA FUNCION */
	char buf[4];
	char *buf2, *buf3;
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

	buf2 = (char*)malloc( h->size );
	e = leer_aux( fd , buf2 , h->name_size );
	h->name = buf2;
	if( h->link_size > 0 ){
		buf3 = (char*)malloc( h->link_size );
		e = leer_aux( fd , buf3 , h->link_size);
		h->link_path = buf3;
	}
	return 0;
}


char * header_to_string(header * h){
	
	char * ret = NULL;
	char * aux = NULL;
	int size = 29;

	size += h->name_size;
	size += h->link_size;

	ret = (char*) malloc(sizeof(char)*size);
	aux = ret + 1;

	ret[0] = 68;
	int_to_char(h->modo, aux);
	int_to_char(h->uid, (aux + 4));
	int_to_char(h->gid, (aux + 8));
	int_to_char(h->size, (aux + 12));
	int_to_char(h->num_blocks, (aux + 16));
	int_to_char(h->name_size, (aux + 20));
	int_to_char(h->link_size, (aux + 24));
	sprintf((aux + 28), "%s", h->name);
	if ( h->link_size > 0 ) sprintf((aux + 28 + h->name_size), "%s", h->link_path);

	return ret;
}

void store_header(header * h, int fd){
	write_aux(fd, 29 + h->name_size + h->link_size, header_to_string(h));
}

int save_data( int fd , char* path ){
	
	int a, l;
	char buff[400];

	a = open( path , O_RDONLY );
	l = 1;

	while( l > 0 ){

		l = read( a , buff , 400 );
		write_aux( fd , l , buff );
	}
}

int pack( char** argv , int argc ){
	
	int fd;
	header h;
	int i;

	fd = open ( argv[1] , O_RDWR | O_CREAT | O_TRUNC , S_IRWXU | S_IRWXG | S_IROTH );

	for ( i = 2 ; i < argc ; i ++){

		get_header( argv[i] , &h );
		store_header( &h , fd );
		if ( ( h.modo & __S_IFMT ) == __S_IFLNK) continue;
		if ( ( h.modo & __S_IFMT ) == __S_IFDIR ) pack_dir( fd , argv[i] );
		if ( ( h.modo & __S_IFMT ) == __S_IFREG ) save_data( fd , argv[i] );
	}
	
}

void pack_dir(int fd, char * ruta){
  
  DIR* dirp;
  struct dirent* de;
  header h;

  dirp = opendir( ruta );
  while( de = readdir( dirp ) ){
    
    get_header( make_path(ruta, (de->d_name)), &h);
    
    if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0 ) continue;
    
    store_header( &h, fd);

    if(( h.modo & __S_IFMT ) == __S_IFLNK) continue;
    if( (h.modo & __S_IFDIR) == __S_IFDIR) pack_dir(fd, make_path(ruta, (de->d_name)) );
    if( (h.modo & __S_IFREG) == __S_IFREG) save_data(fd, make_path(ruta, (de->d_name)));
    	
  }

}