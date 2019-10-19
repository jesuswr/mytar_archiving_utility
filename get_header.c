#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int modo;
	int uid;
	int gid;
	int size;
	int num_blocks;
	int name_size;
	int link_size;
	char* name;
	char* link_path;
} header;


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



}




int main( int argc , char ** argv ){
	header h;
	get_header( argv[1] , &h );
	printf("MODO: %o\n", h.modo);
	printf("USUARIO: %d\n", h.uid);
	printf("GRUPO: %d\n", h.gid);
	printf("TAMANO: %d\n", h.size);
	printf("NUM BLOCKS: %d\n", h.num_blocks);
	printf("TAMANO NOMBRE: %d\n", h.name_size);
	if ( (h.modo & __S_IFMT) == __S_IFLNK) printf("TAMANO NOMBRE LINK: %d\n", h.link_size);
	printf("NOMBRE: %s\n", h.name);
	if ( (h.modo & __S_IFMT) == __S_IFLNK) printf("LINK: %s\n", h.link_path);

	if ( (h.modo & __S_IFMT) == __S_IFLNK) printf("es un link\n");
	if ( (h.modo & __S_IFMT) == __S_IFDIR) printf("es un dir\n");
	if ( (h.modo & __S_IFMT) == __S_IFREG) printf("es un reg\n");
	if ( (h.modo & __S_IFMT) == __S_IFIFO) printf("es un fifo\n");
	

}