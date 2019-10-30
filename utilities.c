/*
*	File:			utilities.h
*	Description:	file that contains the implementation of some useful
*					functions used in tar
*	Date:			29 / 10 / 19
*/

#include "tar.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/*
*	Function : int_to_char
*	-----------------------------------------------------------
*		stores the first byte of the int to the first position of the
*		array, then right shifts the int bits by 8 and repeats the
*		process now with the second position of the array, then for
*		the third and finally for the fourth
*
*		x : int to store
*		ret : pointer to the array of chars
*/

void int_to_char(int x, char * ret){
	int i;
	for(i=0; i<4; i++){
		ret[i] = (char) (x)&255;
		x >>= 8;
	}
}

/*
*	Function : write_aux
*	-----------------------------------------------------------
*		makes calls to syscall read until all len is read or an 
*		error occurs
*
*		fd : file descriptor of the file to write
*		len : ammount of chars to write from buf
*		buf : array of chars to write from
*
*		returns 0 in case of success or -1 in case of failure		
*/
int write_aux(int fd, int len, char * buf){
	int e , len2 = 0;
	while(len2 < len){
		e = write(fd, buf + len2, len-len2);
		if ( e <= 0 ) return -1;
		len2 += e;
	}
	return 0;
}
/*
*	Function : rotate_char
*	-----------------------------------------------------------
*		uses bit magic to rotate the bits easily to the left
*
*		a : char to rotate the bits from
*		desp : number of times to rotate
*
*		returns an char with the rotated bits
*/
char rotate_char(char a, int desp){
	int i;
	a = ( a << desp ) | ( a >> 8 - desp );
	return a;
}

/*
*	Function : encode_string
*	-----------------------------------------------------------
*		makes calls to rotate_char for all the chars in the string,
*		uses 8 - ( desp % 8 ) because its the same rotate to the left
*		desp times than rotate to the right 8 - ( desp % 8 )
*
*		s : array of chars to rotate
*		len : size of the array
*		desp : number of times to rotate	
*/
void encode_string(char * s, int len, int desp){
	int i;
	for(i=0; i<len; i++) s[i] = rotate_char(s[i], 8 -(desp%8));
}

/*
*	Function : decode_string
*	-----------------------------------------------------------
*		makes calls to rotate_char for all the chars in the string
*
*		s : array of chars to rotate
*		len : size of the array
*		desp : number of times to rotate	
*/

void decode_string(char * s, int len, int desp){
	int i;
	for(i=0; i<len; i++) s[i] = rotate_char(s[i], desp%8);
}


/*
*	Function : make_path
*	-----------------------------------------------------------
*		given two arrays of chars, uses string.h basic functions to
*		create a new string with the format "path/name"
*
*		path : pointer to the path name
*		name : pointer to the file name

*		returns an array of chars of the form path/name	
*/
char* make_path( char* path , char* name ){
	char *ret = 
		(char *)malloc( strlen(path) + strlen(name) + 1);
	if ( strlen(path) == 0 ){
		strcpy( ret , name );	
	}
	else{
		strcpy( ret , path );
		ret[strlen(path)] = '/';
		strcpy(ret + strlen(path)+1, name);
	}
	return ret;
}

/*
*	Function : read_aux
*	-----------------------------------------------------------
*		makes calls to syscall read untile l chars are read and 
*		stored in buf or an error occurs
*
*		fd : file descriptor of the file to read from
*		len : ammount of chars to read 
*		buf : array of chars to write 
*
*		returns 0 in case of success or -1 in case of failure		
*/
int read_aux( int fd , char* buf , int l ){
	int l2, e;
	l2 = 0;
	while ( l2 < l ){
		e = read( fd , buf + l2 , l - l2 );
		if ( e <= 0 ) return -1;
		l2 = l2 + e;
	}
	return 0;
}

/*
*	Function : str_to_int
*	-----------------------------------------------------------
*		saves the bits from each character in the int, to do so
*		it saves the bits from the fourth char, then left shifts 8 bits
*		and saves the bits from the third, then left shifts 8 bits and
*		repeats for the second and first char
*
*		c : pointer to the array of chars
*
*		returns an int that has the bits of the array
*/
int str_to_int( char* c  ){
	int x, i;
	x = 0;
	for( i = 3 ; i >= 0 ; i-- ){
		x = x << 8;
		x = x | (unsigned char)(c[i]);
	}
	return x;
}