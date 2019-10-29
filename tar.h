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
	unsigned char* name;
	unsigned char* link_path;
} header;

int loaddata(int fd1 , int fd2 , int size);

int get_header( unsigned char* path_and_name , header *h );

int read_header( int fd , header* h );

unsigned char * header_to_string(header * h);

void store_header(header * h, int fd);

int save_data( int fd , unsigned char* path );

int pack( unsigned char** argv , int argc );

void pack_dir(int fd, unsigned char * ruta);

#endif