#ifndef _UTILITIES_H
#define _UTILITIES_H

void int_to_char(int x, char * ret);

void write_aux(int fd, int len, char * buf);

char rotar_char(char a, int desp);

void cifrar_string(char * s, int len, int desp);

void descifrar_string(char * s, int len, int desp);

char* make_path( char* path , char* name );


#endif