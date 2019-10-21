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

	pack(argv, argc);

}