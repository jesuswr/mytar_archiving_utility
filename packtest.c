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


int main(int argc, unsigned char ** argv){

	/*pack(__F_IFZ | __F_IFN, argv, argc, argv[1], NULL, 3);*/
    /*show_content_file(0,"mytar", NULL);*/
	unpack(__F_IFY, "mytar", NULL, NULL, 3);
}