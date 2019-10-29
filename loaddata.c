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

loaddata(int fd1 , int fd2 , int size){
	
	int read_count = 0;
	char buff[size];

	leer_aux(fd1, buff, size);
	write_aux(fd2, size, buff);
	
}

int main(){
	//
}