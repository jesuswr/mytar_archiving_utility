/*
  funcion para obtener un string que represente un header

  struct header:

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
*/

#include "tar.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

char * int_to_char(int x, char * ret){
	
	int i;
	
	for(i=0; i<4; i++){
		ret[i] = (x)&255;
		x >>= 8;
	}

}

char * header_to_string(header * h){
	//
	char * ret = NULL;
	int size = 28;
	size += h->name_size;
	size += h->link_size;
	ret = (char*) malloc(sizeof(char)*size);
	int_to_char(h->modo, ret);
	int_to_char(h->uid, (ret + 4));
	int_to_char(h->gid, (ret + 8));
	int_to_char(h->size, (ret + 12));
	int_to_char(h->num_blocks, (ret + 16));
	int_to_char(h->name_size, (ret + 20));
	int_to_char(h->link_size, (ret + 24));
	sprintf((ret + 28), "%s", h->name);
	sprintf((ret + 28 + h->name_size), "%s", h->link_path);

	return ret;
}


int main(){
	//no se como probar esto jeje
	header h;
	int i;
	char * hs;
	h.modo = 1145258561;
	h.uid = 1145258561;
	h.gid = 1145258561;
	h.size = 1145258561;
	h.num_blocks = 1145258561;
	h.name_size = 5;
	h.link_size = 0;
	h.name = "holaa";
	h.link_path = "";
	hs = header_to_string(&h);
	for(i = 0; i<33; i++){
		printf("hs[%d]: %c\n", i, hs[i]);
	}
}