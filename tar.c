/*
*	File:			tar.c
*	Author: 		Jesus Wahrman 15-11540 , Neil Villamizar 15-11523
*	Description:	file that contains the implementation of the mytar 
*					functions
*	Date:			29 / 10 / 19
*/

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

/*
*	Function : loaddata
*	-----------------------------------------------------------
*		reads size bytes from fd1 and writes them in fd2, using
*		helper functions, if c > 0 it decodes de data using another helper
*
*		fd1 : file descriptor of file to read from
*		fd2 : file descriptor of file to write from
*		size : ammount of bytes to copy
*		c : amount of bits to rotate
*
*		returns 0 in case of success or -1 in failure
*/
int loaddata(int fd1 , int fd2 , int size, int c){
	char *buff;
	int e;
	buff = (char*)malloc(size);
	e = read_aux(fd1, buff, size);
	if ( e < 0 ) return -1;
	if(c) decode_string(buff, size, DESP);
	write_aux(fd2, size, buff);
	free(buff);
	return 0;
}

/*
*	Function : get_header
*	-----------------------------------------------------------
*		function that gets the stat of the given file, and
*		extracts all the information needed to the header structure
*
*		path_and_name : path with the name of the file
*		h : pointer to a header, where the information will be stored
*
*		returns 0 in case of success or < 0 in failure
*/
int get_header( char* path_and_name , header *h ){
	
	struct stat sb;
	int x, y, l;
	char * c;
	char * c2;

	x = lstat( path_and_name , &sb );

	if ( x < 0 ) return -1;

	h->modo = sb.st_mode;
	h->uid = sb.st_uid;
	h->gid = sb.st_gid;
	h->size = sb.st_size;
	h->num_blocks = sb.st_blocks;
	h->name_size = strlen( (char *) path_and_name );

	c = ( char * ) malloc( h->name_size );
	if ( c == NULL ){
		return -2;
	}
	strcpy((char *) c , (char *) path_and_name );

	h->name = c;

	if ( ( sb.st_mode & __S_IFMT ) == __S_IFLNK ){	
		
		c = ( char * )malloc(400);
		if ( c == NULL ){
			return -2;
		}
		l = 0;
		l += readlink( path_and_name , c , 399 );
		c[l] = '\0';
		c2 = ( char * )malloc( strlen( (char *) c) );
		if ( c2 == NULL ){
			return -2;
		}
		strcpy((char *) c2, (char *) c);
		h->link_size = l;
		h->link_path = c2;
		free(c);
	}
	else{
		h->link_size = 0;
	}

	return 0;
}

/*
*	Function : read_header
*	-----------------------------------------------------------
*		function that reads from a tar file, the information that has to
*		be stored in the header using helper functions. If c > 0 it decodes
*		the string 
*
*		fd : file descriptor of the file to read the data from
*		h : pointer to a header, where the information will be stored
*		c : amount of bits to rotate
*
*		returns 0 in case of success or < 0 in failure
*/
int read_header( int fd , header* h, int c){
	char buf[4];
	char *buf2, *buf3;
	int e;

	e = read_aux( fd , buf , 4 );
	if ( e < 0 ) return -1;
	if(c) decode_string(buf, 4, DESP);
	h->modo = str_to_int( buf );
	
	e = read_aux( fd , buf , 4 );
	if ( e < 0 ) return -1;
	if(c) decode_string(buf, 4, DESP);
	h->uid = str_to_int( buf );

	e = read_aux( fd , buf , 4 );
	if ( e < 0 ) return -1;
	if(c) decode_string(buf, 4, DESP);
	h->gid = str_to_int( buf );
	
	e = read_aux( fd , buf , 4 );
	if ( e < 0 ) return -1;
	if(c) decode_string(buf, 4, DESP);
	h->size = str_to_int( buf );

	e = read_aux( fd , buf , 4 );
	if ( e < 0 ) return -1;
	if(c) decode_string(buf, 4, DESP);
	h->num_blocks = str_to_int( buf );

	e = read_aux( fd , buf , 4 );
	if ( e < 0 ) return -1;
	if(c) decode_string(buf, 4, DESP);
	h->name_size = str_to_int( buf );

	e = read_aux( fd , buf , 4 );
	if ( e < 0 ) return -1;
	if(c) decode_string(buf, 4, DESP);
	h->link_size = str_to_int( buf );


	buf2 = (char*)malloc( h->name_size );
	if ( buf2 == NULL) return -2;

	e = read_aux( fd , buf2 , h->name_size );
	if ( e < 0 ) return -1;
	if(c) decode_string(buf2, h->name_size, DESP);
	h->name = buf2;
	
	if( h->link_size > 0 ){
		buf3 = (char*)malloc( h->link_size );
		if ( buf3 == NULL) return -2;
		e = read_aux( fd , buf3 , h->link_size);
		if ( e < 0 ) return -1;
		if(c) decode_string(buf3, h->link_size, DESP);
		h->link_path = buf3;
	}

	return 0;
}

/*
*	Function : header_to_string
*	-----------------------------------------------------------
*		function that stores the files of a header in a char array,
*		using helper functions
*
*		h : pointer to a header, that contains the information to be saved
*
*		returns a pointer to the char array in success of NULL in failure
*/
char * header_to_string(header * h){
	
	char * ret = NULL;
	char * aux = NULL;
	int size = 29;

	size += h->name_size;
	size += h->link_size;

	ret = (char*) malloc(sizeof(char)*size);
	if ( ret == NULL ) return NULL;
	aux = ret + 1;

	ret[0] = 68;
	int_to_char(h->modo, aux);
	int_to_char(h->uid, (aux + 4));
	int_to_char(h->gid, (aux + 8));
	int_to_char(h->size, (aux + 12));
	int_to_char(h->num_blocks, (aux + 16));
	int_to_char(h->name_size, (aux + 20));
	int_to_char(h->link_size, (aux + 24));
	sprintf((aux + 28), "%s", (char *) h->name);
	if ( h->link_size > 0 ) 
		sprintf((aux + 28 + h->name_size), "%s", (char *) h->link_path);

	return ret;
}

/*
*	Function : store_header
*	-----------------------------------------------------------
*		function that stores the content of a header in a file, using
*		the header_to_string function, if c > 0 it encodes the string
*
*		h : pointer to a header, that contains the information
*		fd : file descriptor of the file where the information will be saved
*		c : amount of bits to rotate
*
*		returns 0 in case of success or < 0 in failure
*/
int store_header(header * h, int fd, int c){
	char * buf = header_to_string(h);
	int e;
	if ( buf == NULL ) return -1;
	if(c){ 
		encode_string(buf, 29 + h->name_size + h->link_size, DESP);
		buf[0] = 'C';
	}
	e = write_aux(fd, 29 + h->name_size + h->link_size, buf);
	free(buf);
	if ( e < 0 ) return -2;
	return 0;
}

/*
*	Function : save_data
*	-----------------------------------------------------------
*		function that stores the content of a file in another file, 
*		using helper functions. If c > 0 it encodes the content before
*		saving it
*
*		fd : file descriptor of the file where the data will be saved
*		path : path of the file to read the data from
*		c : amount of bits to rotate
*
*		returns 0 in case of success or < 0 in failure
*/
int save_data( int fd , char* path, int c){
	int a, l, e;
	char buff[400];

	a = open( path , O_RDONLY );
	if ( a < 0 ) return -1;
	l = 1;

	while( l > 0 ){

		l = read( a , buff , 400 );
		if ( l < 0 ) return -2;
		if(c) encode_string(buff, l, DESP);
		e = write_aux( fd , l , buff );
		if ( e < 0 ) return -3;
	}
	return 0;
}

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
int pack( int flag_mask, char** argv , int argc, char * pack_file, 
			char * v_output_file, int desp ){ 
	
	int fd;
	header h;
	int i, e;

	fd_v_output = 1;
	DESP = 0;

	if(flag_mask & __F_IFZ) DESP = desp;

	if((flag_mask & __F_IFV) == __F_IFV){
		if(v_output_file != NULL) {
			fd_v_output = open(v_output_file, O_RDWR | O_CREAT | O_TRUNC,  
								S_IRWXU | S_IRWXG | S_IROTH );
			if ( fd_v_output < 0 ) {
				printf("Error opening the wanted output file. The output will go to the terminal.\n");
				fd_v_output = 1;
			}
		}
		e = write_aux(fd_v_output, 34, "Starting packing...\nCreating file ");
		if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
		e = write_aux(fd_v_output, strlen(pack_file), pack_file);
		if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
		e = write_aux(fd_v_output, 5, " ...\n");
		if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
	}

	fd = open ( pack_file , O_RDWR | O_CREAT | O_TRUNC , 
		S_IRWXU | S_IRWXG | S_IROTH );
	if ( fd < 0 ){
		e = write_aux( fd_v_output , 44 , "Error creating the tar with the given name.\n" );
		if ( e < 0 ) printf("Error creating the tar with the given name.\n");
		return -1;
	}

	for ( i = 0 ; i < argc ; i ++){

		e = get_header( argv[i] , &h );
		if ( e < 0 ){
			e = write_aux( fd_v_output , 40 , "Error using lstat to get the file info.\n");
			if ( e < 0 ) printf("Error using lstat to get the file info\n");
			continue;
		}

		if ((flag_mask & __F_IFN) && ((( h.modo & __S_IFMT ) == __S_IFLNK) || 
			(( h.modo & __S_IFMT ) == __S_IFIFO))) continue;

		e = store_header( &h , fd, DESP);
		if ( e < 0 ){
			e = write_aux( fd_v_output , 40 , "Error storing the file info in the tar.\n");
			if ( e < 0 ) printf("Error storing the file info in the tar.\n");
			continue;
		}

		if((flag_mask & __F_IFV) == __F_IFV){
			e = write_aux(fd_v_output, 8, "Packing ");
			if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
			e = write_aux(fd_v_output, strlen(h.name), h.name);
			if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
			e = write_aux(fd_v_output, 1, "\n");
			if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");	
		}

		if ( ( h.modo & __S_IFMT ) == __S_IFLNK) continue;
		if ( ( h.modo & __S_IFMT ) == __S_IFDIR ) 
			pack_dir(flag_mask, fd , argv[i] , pack_file);
		if ( ( h.modo & __S_IFMT ) == __S_IFREG ) 
			save_data( fd , argv[i], DESP);


		free( h.name );
		if ( h.link_size > 0 ) free( h.link_path );
	}

	close( fd );
	return 0;
}

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
void pack_dir(int flag_mask, int fd, char * path, char * pack_file){
  	DIR* dirp;
  	struct dirent* de;
  	header h;
  	int e;

  	dirp = opendir( path );
 	if ( dirp == NULL ){
		e = write_aux( fd_v_output , 27 , "Error opening a directory.\n");
		if ( e < 0 ) printf("Error opening a directory.\n");
    }
	while( de = readdir( dirp ) ){

		e = get_header( make_path(path, (de->d_name)), &h);
		if ( e < 0 ){
			e = write_aux( fd_v_output , 40 , "Error using lstat to get the file info.\n");
			if ( e < 0 ) printf("Error using lstat to get the file info.\n");
			continue;
		}
	
		if((flag_mask & __F_IFN) && ((( h.modo & __S_IFMT ) == __S_IFLNK) 
			|| (( h.modo & __S_IFMT ) == __S_IFIFO))) continue;

		/* ARREGLAR ESTA LINEA DE ABAJO, LA PARTE DE mytar */
		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0 
			|| strcmp(de->d_name, pack_file)==0) continue; 
	
		e = store_header( &h, fd, DESP);
		if ( e < 0 ){
			e = write_aux( fd_v_output , 40 , "Error storing the file info in the tar.\n");
			if ( e < 0 ) printf("Error storing the file info in the tar.\n");
			continue;
		}

		if((flag_mask & __F_IFV) == __F_IFV){
			e = write_aux(fd_v_output, 8, "Packing ");
			if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
			e = write_aux(fd_v_output, strlen(h.name), h.name);
			if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
			e = write_aux(fd_v_output, 1, "\n");
			if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
		}

		if(( h.modo & __S_IFMT ) == __S_IFLNK) continue;
		if( (h.modo & __S_IFDIR) == __S_IFDIR) 
			pack_dir(flag_mask, fd, make_path(path, (de->d_name)), "." );
		if( (h.modo & __S_IFREG) == __S_IFREG) {
			e = save_data(fd, make_path(path, (de->d_name)), DESP);
			if ( e < 0 ){
				e = write_aux( fd_v_output , 23 , "Error saving the data.\n" );
				if ( e < 0 ) printf("Error saving the data.\n");
			}
		}

		free(h.name);
		if ( h.link_size > 0 ) free(h.link_path);
  	}
  	closedir(  dirp );
  	free(h.name);

}

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
	char * v_output_file, char * unpacking_dir, int desp){
	
	int fd, fd2, e;
	header h;
	char buf;
	DESP = 0;
	fd = open(packed_file, O_RDWR);

	if(flag_mask & __F_IFO) chdir(unpacking_dir);

	if(flag_mask & __F_IFY) DESP = desp;

	fd_v_output = 1;

	if((flag_mask & __F_IFV) == __F_IFV){
		if(v_output_file != NULL) fd_v_output = 
			open(v_output_file, O_RDWR | O_CREAT | O_TRUNC,  
				S_IRWXU | S_IRWXG | S_IROTH );
		if ( fd_v_output < 0 ){
			printf("Error opening the output file. The output will be on the terminal.\n");
			fd_v_output = 1;
		}
		e = write_aux(fd_v_output, 33, "Starting unpacking...\nOpening file ");
		if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
		e = write_aux(fd_v_output, strlen(packed_file), packed_file);
		if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
		e = write_aux(fd_v_output, 1, "\n");
		if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
	}

	if ( fd < 0 ){
		e = write_aux( fd_v_output , 34 , "Error opening the file to unpack.\n");
		if ( e < 0 ){
			printf("Error opening the file to unpack.\n");
		}
		return -1;
	}

	while ( read(fd , &buf , 1)==1 && (buf=='D' || buf=='C') ){

		if((flag_mask & __F_IFY) && buf != 'C'){
			e = write_aux( fd_v_output , 55 , "The tar is not crypted but the decrypt flag was given.\n");
			if ( e < 0 ) printf("The tar is not crypted but the decrypt flag was give.\n");
			return -1;
		}

		if( !(flag_mask & __F_IFY) && buf == 'C' ){
			e = write_aux( fd_v_output , 55 , "The tar is crypted but the decrypt flag was not given.\n");
			if ( e < 0 ) printf("The tar is not crypted but the decrypt flag was give.\n");
			return -1;
		}

		e = read_header(fd, &h, DESP);
		if ( e < 0 ){
			e = write_aux( fd_v_output , 28 , "Error reading the tar file.\n");
			if ( e < 0 ) printf("Error reading the tar file.\n");
		}

		if((flag_mask & __F_IFN) && ((( h.modo & __S_IFMT ) == 
			__S_IFLNK) || (( h.modo & __S_IFMT ) == __S_IFIFO))) continue;

		if((flag_mask & __F_IFV) == __F_IFV){
			e = write_aux(fd_v_output, 10, "Unpacking ");
			if ( e < 0 ) printf("Error writing verbose in the given output file.\n");
			write_aux(fd_v_output, strlen(h.name), h.name);
			if ( e < 0 ) printf("Error writing verbose in the given output file.\n");
			write_aux(fd_v_output, 1, "\n");
			if ( e < 0 ) printf("Error writing verbose in the given output file.\n");
		}


		if ( (h.modo & __S_IFMT) == __S_IFDIR ){
			e = mkdir(h.name, h.modo);
			if ( e < 0 ){
				e = write_aux( fd_v_output , 28 , "Error creating a directory.\n" );
				if ( e < 0 ) printf("Error creating a directory.\n");
			}
		}
		if ( (h.modo & __S_IFMT ) == __S_IFLNK ){
			e = symlink(h.link_path, h.name);
			if ( e < 0 ){
				e = write_aux( fd_v_output , 32 , "Error creating a symbolic link.\n" );
				if ( e < 0 ) printf("Error creating a symbolic link.\n");
			}
		}
		if ( ( h.modo & __S_IFMT ) == __S_IFIFO ){
			e = mkfifo(h.name, h.modo);
			if ( e < 0 ){
				e = write_aux( fd_v_output , 23 , "Error creating a pipe.\n");
				if ( e < 0 ) printf("Error creating a pipe.\n");
			}
		}
		if ( (h.modo & __S_IFMT) == __S_IFREG ){
			fd2 = open( h.name , O_RDWR | O_CREAT | O_TRUNC, h.modo );
			if ( fd2 < 0 ){
				e = write_aux( fd_v_output , 31 , "Error creating a regular file.\n" );
				if ( e < 0 ) printf("Error creating a regular file.\n");
				continue;
			}
			e = loaddata( fd , fd2 , h.size, DESP);
			if ( e < 0 ){
				e = write_aux( fd_v_output , 31 , "Error creating a regular file.\n" );
				if ( e < 0 ) printf("Error creating a regular file.\n");
				continue;
			}
			close(fd2);
		}
		free(h.name);
		if ( h.link_size > 0 ) free(h.link_path);
	}
	close( fd );
	return 0;
}

/*
*	Function : show_content_file
*	-----------------------------------------------------------
*		shows the files in the tar file, it reads the headers and 
*		prints the file name and other information like "ls -l"
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
	char * v_output_file, int desp){
	
	int fd, fd2, i, e;
	header h;
	char buf;
	char * aux , * buff;
	DESP = 0;

	fd_v_output = 1;

	if((flag_mask & __F_IFV) == __F_IFV && (v_output_file != NULL)){
		fd_v_output = 
			open(v_output_file, O_RDWR | O_CREAT | O_TRUNC,  
				S_IRWXU | S_IRWXG | S_IROTH );
		if ( fd_v_output < 0 ){
			printf("Error opening the output file. The output will be in the terminal.\n" );
			fd_v_output = 1;
		}
	}

	if ( flag_mask & __F_IFY ) DESP = desp;
	
	aux =  (char *) malloc(4); 
	if ( aux == NULL ){
		e = write_aux( fd_v_output , 25 , "Error allocating memory.\n");
		if ( e < 0 ) printf("Error allocating memory.\n");
	}

	aux[0] = 'x';
	aux[1] = 'w';
	aux[2] = 'r';

	fd = open(packed_file, O_RDWR);
	if ( fd < 0 ){
		e = write_aux( fd_v_output , 28 , "Error opening the tar file.\n");
		if ( e < 0 ) printf("Error opening the tar file.\n");
		return -1;
	}

	e = write_aux(fd_v_output, 20, "Content of the file ");
	if ( e < 0 ) printf("Error writing in the given output file.\n");
	e = write_aux(fd_v_output, strlen(packed_file), packed_file);
	if ( e < 0 ) printf("Error writing in the given output file.\n");
	e = write_aux(fd_v_output, 2, ":\n");
	if ( e < 0 ) printf("Error writing in the given output file.\n");

	while ( e = read(fd , &buf, 1) && (buf=='D' || buf=='C') ){

		e = read_header(fd, &h, DESP);
		if ( e < 0 ){
			e = write_aux( fd_v_output , 40 , "Error reading the info in the tar file.\n");
			if ( e < 0 )printf("Error reading the info in the tar file.\n");
			continue;
		}

		if((flag_mask & __F_IFN) && ((( h.modo & __S_IFMT ) == 
			__S_IFLNK) || (( h.modo & __S_IFMT ) == __S_IFIFO))) continue;

		if( (h.modo & __S_IFMT) == __S_IFDIR ) write_aux(fd_v_output, 1, "d");
		else if( ( h.modo & __S_IFMT ) == __S_IFLNK ) write_aux(fd_v_output, 1, "l");
		else if( ( h.modo & __S_IFMT ) == __S_IFIFO ) write_aux(fd_v_output, 1, "p");
		else write_aux(fd_v_output, 1, "-");

		for(i=8; i>=0; i--){
			if((h.modo & (1<<i))) write_aux(fd_v_output, 1, & aux[i%3]);
			else write_aux(fd_v_output, 1, "-");
		}

		write_aux(fd_v_output, 1, " ");

		write_aux_int(fd_v_output, h.uid);

		write_aux(fd_v_output, 1, " ");

		write_aux_int(fd_v_output, h.gid);

		write_aux(fd_v_output, 1, " ");

		write_aux_int(fd_v_output, h.size);

		write_aux(fd_v_output, 1, " ");

		write_aux(fd_v_output, strlen(h.name), h.name);

		write_aux(fd_v_output, 1, "\n");

		if ( (h.modo & __S_IFMT) == __S_IFREG ){
			buff = ( char *)malloc(h.size);
			read_aux(fd, buff, h.size);
			free(buff);
		}

		free(h.name);
		if ( h.link_size > 0 ) free(h.link_path);
	}
	close(fd);
	free(aux);
	return 0;

}

/*
*	Function : write_aux_int
*	-----------------------------------------------------------
*		helper function to write ints to the output
*
*		fd : file descriptor of the output file
*		n : number to write
*/
void write_aux_int(int fd, int n){
	char buf[11];
	int size = 0;
	int len = 0;
	int x = n;

	if(n==0){
		write_aux(fd, 1, "0");
	}

	while(x) x/=10, size++;

	len = size;

	while(n){
		buf[size-1] = '0' + n%10;
		size--;
		n/=10;
	}

	write_aux(fd, len, buf);

}

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
	char * v_output_file, char * unpacking_dir, int desp){
	int fd, fd2, e;
	header h;
	char buf, *buf2;
	DESP = 0;

	if(flag_mask & __F_IFO) chdir(unpacking_dir);

	if(flag_mask & __F_IFY) DESP = desp;

	fd_v_output = 1;

	if((flag_mask & __F_IFV) == __F_IFV){
		if(v_output_file != NULL) fd_v_output = 
			open(v_output_file, O_RDWR | O_CREAT | O_TRUNC,  
				S_IRWXU | S_IRWXG | S_IROTH );
		if ( fd_v_output < 0 ){
			printf("Error opening the output file. The output will be on the terminal.\n");
			fd_v_output = 1;
		}
		e = write_aux(fd_v_output, 33, "Starting unpacking...\nOpening file ");
		if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
		e = write_aux(fd_v_output, strlen(packed_file), packed_file);
		if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
		e = write_aux(fd_v_output, 1, "\n");
		if ( e < 0 ) printf("Error trying to write in the wanted file descriptor.\n");
	}

	fd = open(packed_file, O_RDWR);
	if ( fd < 0 ){
		e = write_aux( fd_v_output , 34 , "Error opening the file to unpack.\n");
		if ( e < 0 ){
			printf("Error opening the file to unpack.\n");
		}
		return -1;
	}

	while ( read(fd , &buf , 1)==1 && (buf=='D' || buf=='C') ){

		if((flag_mask & __F_IFY) && buf != 'C'){
			e = write_aux( fd_v_output , 55 , "The tar is not crypted but the decrypt flag was given.\n");
			if ( e < 0 ) printf("The tar is not crypted but the decrypt flag was give.\n");
			return -1;
		}

		if( !(flag_mask & __F_IFY) && buf == 'C' ){
			e = write_aux( fd_v_output , 55 , "The tar is crypted but the decrypt flag was not given.\n");
			if ( e < 0 ) printf("The tar is not crypted but the decrypt flag was give.\n");
			return -1;
		}

		e = read_header(fd, &h, DESP);
		if ( e < 0 ){
			e = write_aux( fd_v_output , 28 , "Error reading the tar file.\n");
			if ( e < 0 ) printf("Error reading the tar file.\n");
		}

		if((flag_mask & __F_IFN) && ((( h.modo & __S_IFMT ) == 
			__S_IFLNK) || (( h.modo & __S_IFMT ) == __S_IFIFO))) continue;

		if((flag_mask & __F_IFV) == __F_IFV){
			e = write_aux(fd_v_output, 10, "Looking ");
			if ( e < 0 ) printf("Error writing verbose in the given output file.\n");
			write_aux(fd_v_output, strlen(h.name), h.name);
			if ( e < 0 ) printf("Error writing verbose in the given output file.\n");
			write_aux(fd_v_output, 1, "\n");
			if ( e < 0 ) printf("Error writing verbose in the given output file.\n");
		}
		if ( str_cmp( h.name , f_name ) == 0 ){	
			if ( (h.modo & __S_IFMT) == __S_IFDIR ){
				e = mkdir(f_name, h.modo);
				if ( e < 0 ){
					e = write_aux( fd_v_output , 28 , "Error creating a directory.\n" );
					if ( e < 0 ) printf("Error creating a directory.\n");
				}
			}
			if ( (h.modo & __S_IFMT ) == __S_IFLNK ){
				e = symlink(h.link_path, f_name);
				if ( e < 0 ){
					e = write_aux( fd_v_output , 32 , "Error creating a symbolic link.\n" );
					if ( e < 0 ) printf("Error creating a symbolic link.\n");
				}
			}
			if ( ( h.modo & __S_IFMT ) == __S_IFIFO ){
				e = mkfifo(f_name, h.modo);
				if ( e < 0 ){
					e = write_aux( fd_v_output , 23 , "Error creating a pipe.\n");
					if ( e < 0 ) printf("Error creating a pipe.\n");
				}
			}
			if ( (h.modo & __S_IFMT) == __S_IFREG ){
				fd2 = open( f_name , O_RDWR | O_CREAT | O_TRUNC, h.modo );
				if ( fd2 < 0 ){
					e = write_aux( fd_v_output , 31 , "Error creating a regular file.\n" );
					if ( e < 0 ) printf("Error creating a regular file.\n");
					continue;
				}
				e = loaddata( fd , fd2 , h.size, DESP);
				if ( e < 0 ){
					e = write_aux( fd_v_output , 31 , "Error creating a regular file.\n" );
					if ( e < 0 ) printf("Error creating a regular file.\n");
					continue;
				}
				close(fd2);
			}
			free(h.name);
			if ( h.link_size > 0 ) free(h.link_path);
			close(fd);
			return 0;
		}
		else
		{
			if ( (h.modo & __S_IFMT) == __S_IFREG ){
				buf2 = (char*)malloc(h.size);
				e = read( fd , buf2 , h.size );
				free(buf2);
				if ( e < 0 ){
					e = write_aux( fd_v_output , 31 , "Error creating a regular file.\n" );
					if ( e < 0 ) printf("Error creating a regular file.\n");
					continue;
				}
			}
		}
	}	
	close( fd );
	e = write_aux( fd_v_output , 42 , "The given file is not in the tar archive.\n");
	if ( e < 0 ) printf("The given file is not in the tar archive.\n");
	return 0;
}