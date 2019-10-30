#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "tar.h"
#include "utilities.h"
#include <dirent.h>


int main(int argc, char ** argv){

	/*pack(0, argv, argc, argv[1], NULL, 3);*/
    /*show_content_file(0,"mytar", NULL);*/
	unpack(0, "mytar", NULL, NULL, 0);
}