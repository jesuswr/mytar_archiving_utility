#ifndef _UTILITIES_H
#define _UTILITIES_H

void int_to_char(int x, unsigned char * ret);

void write_aux(int fd, int len, unsigned char * buf);

unsigned char rotar_char(unsigned char a, int desp);

void cifrar_string(unsigned char * s, int len, int desp);

void descifrar_string(unsigned char * s, int len, int desp);

unsigned char* make_path( unsigned char* path , unsigned char* name );

int leer_aux( int fd , unsigned char* buf , int l );

int str_to_int( unsigned char* c  );

#endif