/*
*	File:			tar.h
*	Author: 		Jesus Wahrman 15-11540 , Neil Villamizar 15-11523
*	Description:	file that contains the signatures of the mytar functions
*	Date:			29 / 10 / 19
*/

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

/*
*	Function : loaddata
*	-----------------------------------------------------------
*		Copies size bytes from fd1 to fd2
*
*		fd1 : file descriptor of file to read from
*		fd2 : file descriptor of file to write from
*		size : ammount of bytes to copy
*		c : ammount of bits to rotate
*
*		returns 0 in case of success or -1 in failure
*/
int loaddata(int fd1 , int fd2 , int size, int c);

/*
*	Function : get_header
*	-----------------------------------------------------------
*		loads important file information into a header
*
*		path_and_name : path with the name of the file
*		h : pointer to a header, where the information will be stored
*
*		returns 0 in case of success or < 0 in failure
*/
int get_header( char* path_and_name , header *h );

/*
*	Function : read_header
*	-----------------------------------------------------------
*		function that reads from a tar file, the information that has to
*		be stored in the header
*
*		fd : file descriptor of the file to read the data from
*		h : pointer to a header, where the information will be stored
*		c : amount of bits to rotate
*
*		returns 0 in case of success or < 0 in failure
*/
int read_header( int fd , header* h, int c);

/*
*	Function : header_to_string
*	-----------------------------------------------------------
*		function that stores the files of a header in a char array
*
*		h : pointer to a header, that contains the information to be saved
*
*		returns a pointer to the char array in success of NULL in failure
*/
char * header_to_string(header * h);

/*
*	Function : store_header
*	-----------------------------------------------------------
*		function that stores the content of a header in a file, 
*		if c > 0 it encodes the string
*
*		h : pointer to a header, that contains the information
*		fd : file descriptor of the file where the information will be saved
*		c : amount of bits to rotate
*
*		returns 0 in case of success or < 0 in failure
*/
int store_header(header * h, int fd, int c);

/*
*	Function : save_data
*	-----------------------------------------------------------
*		function that stores the content of a file in another file . If c > 0 
*		it encodes the content before
*		saving it
*
*		fd : file descriptor of the file where the data will be saved
*		path : path of the file to read the data from
*		c : amount of bits to rotate
*
*		returns 0 in case of success or < 0 in failure
*/
int save_data( int fd , char* path, int c);

/*
*	Function : pack
*	-----------------------------------------------------------
*		function that packs the given files in a tar file, with
*		the especified flags
*
*		flag_mask : int that represents what flags are on
*		argv : array of strings that represent the files to be packed
*		argc : number of files in argv
*		pack_file : name of the tar file
*		v_output_file : name of the file to save the output in,
*						if there's no name it will go to the terminal
*		desp : amount of bits to rotate the data bits
*
*		returns 0 in case of success or < 0 in failure
*/
int pack( int flag_mask, char** argv , int argc, 
	char * pack_file, char * v_output_file, int desp );

/*
*	Function : pack_dir
*	-----------------------------------------------------------
*		function that packs the given files in a tar file, with
*		the especified flags
*
*		flag_mask : int that represents what flags are on
*		fd : file descriptor of the tar file to save the data in
*		path : path of the current dir that we are saving
*/
void pack_dir(int flag_mask, int fd, char * ruta);

/*
*	Function : unpack
*	-----------------------------------------------------------
*		function that unpacks the given files in a tar file, with
*		the especified flags
*
*		flag_mask : int that represents what flags are on
*		pack_file : name of the tar file
*		v_output_file : name of the file to save the output in,
*						if there's no name it will go to the terminal
*		unpacking_dir : directory where the files will be created
*		desp : amount of bits to rotate the data bits
*
*		returns 0 in case of success or < 0 in failure
*/
int unpack(int flag_mask, char * packed_file, 
	char * v_output_file, char * unpacking_dir, int desp);

/*
*	Function : show_content_file
*	-----------------------------------------------------------
*		shows the files in the tar file
*
*		flag_mask : int that represents what flags are on
*		pack_file : name of the tar file
*		v_output_file : name of the file to save the output in,
*						if there's no name it will go to the terminal
*		desp : amount of bits to rotate the data bits
*
*		returns 0 in case of success or < 0 in failure
*/
int show_content_file(int flag_mask, char * packed_file, 
	char * v_output_file, int desp);

/*
*	Function : write_aux_int
*	-----------------------------------------------------------
*		helper function to write ints to the output
*
*		fd : file descriptor of the output file
*		n : number to write
*/
void write_aux_int(int fd, int n);

/*
*	Function : single_extract
*	-----------------------------------------------------------
*		function that looks for a file with a specific name in the tar, and
*		if it finds it, it extracts the file from the tar
*
*		flag_mask : int that represents what flags are on
*		f_name : name of the file to look
*		pack_file : name of the tar file
*		v_output_file : name of the file to save the output in,
*						if there's no name it will go to the terminal
*		unpacking_dir : directory where the files will be created
*		desp : amount of bits to rotate the data bits
*
*		returns 0 in case of success or < 0 in failure
*/
int single_extract(int flag_mask, char* f_name, char * packed_file, 
	char * v_output_file, char * unpacking_dir, int desp);

#endif