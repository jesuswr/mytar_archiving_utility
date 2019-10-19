#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "tar.h"
#include "utilities.h"


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

	if ( ( sb.st_mode & __S_IFMT ) == __S_IFLNK )
	{	
		c = ( char * )malloc(400);
		l = 0;
		l += readlink( path_and_name , c , 399 );
		c[l] = '\0';
		c2 = ( char * )malloc( strlen(c) );
		strcpy(c2,c);
		h->link_size = l;
		h->link_path = c2;
		free(c);
	}
	return 0;
}

char * header_to_string(header * h){
	//
	char * ret = NULL;
	int size = 28;
	size += h->name_size;
	size += h->link_size;
	ret = (char*) malloc(sizeof(char)*size);
	int_to_char(h->modo, ret);
	int_to_char(h->uid, (ret + 4));
	int_to_char(h->gid, (ret + 8));
	int_to_char(h->size, (ret + 12));
	int_to_char(h->num_blocks, (ret + 16));
	int_to_char(h->name_size, (ret + 20));
	int_to_char(h->link_size, (ret + 24));
	sprintf((ret + 28), "%s", h->name);
	sprintf((ret + 28 + h->name_size), "%s", h->link_path);

	return ret;
}