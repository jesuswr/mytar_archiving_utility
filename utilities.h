/*
*	File:			utilities.h
*	Description:	file that contains the signature of some useful
*					functions used in tar
*	Date:			29 / 10 / 19
*/

#ifndef _UTILITIES_H
#define _UTILITIES_H

/*
*	Function : int_to_char
*	-----------------------------------------------------------
*		stores the bytes of the int in an char array of size 4
*
*		x : int to store
*		ret : pointer to the array of unsigned chars
*/
void int_to_char(int x, unsigned char * ret);

/*
*	Function : write_aux
*	-----------------------------------------------------------
*		helper function to write the content of an array of chars
*		to the file of the given file descriptor
*
*		fd : file descriptor of the file to write
*		len : ammount of chars to write from buf
*		buf : array of unsigned chars to write from
*
*		returns 0 in case of success or -1 in case of failure		
*/
int write_aux(int fd, int len, unsigned char * buf);

/*
*	Function : rotate_char
*	-----------------------------------------------------------
*		rotates the bits of a char to the right the given number of times
*
*		a : unsigned char to rotate the bits from
*		desp : number of times to rotate
*
*		returns an unsigned char with the rotated bits
*/
unsigned char rotate_char(unsigned char a, int desp);

/*
*	Function : encode_string
*	-----------------------------------------------------------
*		rotates the bits of all the chars in the given array to the right
*
*		s : array of chars to rotate
*		len : size of the array
*		desp : number of times to rotate	
*/
void encode_string(unsigned char * s, int len, int desp);

/*
*	Function : decode_string
*	-----------------------------------------------------------
*		rotates the bits of all the chars in the given array to the left
*
*		s : array of chars to rotate
*		len : size of the array
*		desp : number of times to rotate	
*/
void decode_string(unsigned char * s, int len, int desp);


/*
*	Function : make_path
*	-----------------------------------------------------------
*		given two arrays of chars, creates a path of the form "path/name"
*
*		path : pointer to the path name
*		name : pointer to the file name

*		returns an array of chars of the form path/name	
*/
unsigned char* make_path( unsigned char* path , unsigned char* name );

/*
*	Function : read_aux
*	-----------------------------------------------------------
*		helper function to read the content of a file to an
*		array of unsigned chars
*
*		fd : file descriptor of the file to read from
*		len : ammount of chars to read 
*		buf : array of unsigned chars to write 
*
*		returns 0 in case of success or -1 in case of failure		
*/
int read_aux( int fd , unsigned char* buf , int l );

/*
*	Function : str_to_int
*	-----------------------------------------------------------
*		stores the bits of the chars in the array to an int
*
*		c : pointer to the array of chars
*
*		returns an int that has the bits of the array
*/
int str_to_int( unsigned char* c  );

#endif