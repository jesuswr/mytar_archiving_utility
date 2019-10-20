#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "tar.h"
#include "utilities.h"


void pack_dir(int fd, header *, char * ruta){
	
	DIR* dirp;
	struct dirent* de;
	header h;

	dirp = opendir( ruta );
	while( de = readdir( dirp ) ){
		get_header( make_path(ruta, (de->name)), &h);
		store_header( &h, fd);
		if( (de->type & S_ISDIR) == S_ISDIR) pack_dir(fd, &h, make_path(ruta, (de->name)) );
		if( (de->type & S_ISREG) == S_ISREG) save_data(fd, make_path(ruta, (de->name)));
		/*printf("%s %u\n",de->d_name ,de->d_type);*/
	}
}

int main(){
	header h;
	int fd;
	get_header("carpetica", &h);
	fd = open("mytar", O_RDWR |  O_CREAT |  O_TRUNC);
	store_header( &h, fd);

	pack_dir(fd, &h, "carpetica");

}