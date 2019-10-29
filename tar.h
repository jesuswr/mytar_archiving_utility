#ifndef _TAR_H
#define _TAR_H

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

int get_header( char* path_and_name , header *h );

int read_header( int fd , header* h );

char * header_to_string(header * h);

void store_header(header * h, int fd);

int save_data( int fd , char* path );

int pack( char** argv , int argc );

void pack_dir(int fd, char * ruta);

#endif