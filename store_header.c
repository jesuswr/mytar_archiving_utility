

#include <stdio.h>
#include <unistd.h>
#include "tar.h"
#include "utilities.h"

void store_header(header * h, int fd){
	write_aux(fd, 28 + h->name_size + h->link_size, header_to_string(h));
}

int main(){
	//
}