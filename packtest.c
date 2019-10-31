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

	/*pack(__F_IFZ, argv, argc, argv[1], NULL, 3);*/
    show_content_file(__F_IFY,"mytar", NULL,3);
	/*unpack(__F_IFY, "mytar", NULL, NULL, 3);*/
	/*single_extract( 0 , argv[1] , argv[2] , NULL , NULL, 0 );*/
}