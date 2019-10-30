#ifndef _TAR_H
#define _TAR_H

#define __F_IFF 1<<1
#define __F_IFC 1<<2
#define __F_IFN 1<<3
#define __F_IFT 1<<4
#define __F_IFO 1<<5
#define __F_IFZ 1<<6
#define __F_IFY 1<<7
#define __F_IFX 1<<8
#define __F_IFS 1<<9
#define __F_IFV 1<<10

int fd_v_output;
int DESP;

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

int loaddata(int fd1 , int fd2 , int size, int c);

int get_header( char* path_and_name , header *h );

int read_header( int fd , header* h, int c);

char * header_to_string(header * h);

int store_header(header * h, int fd, int c);

int save_data( int fd , char* path, int c);

int pack( int flag_mask, char** argv , int argc, char * pack_file, char * v_output_file, int desp );

void pack_dir(int flag_mask, int fd, char * ruta);

int unpack(int flag_mask, char * packed_file, char * v_output_file, char * unpacking_dir, int desp);

int show_content_file(int flag_mask, char * packed_file, char * v_output_file);

void write_aux_int(int fd, int n);

#endif