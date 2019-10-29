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


unpack( char * packed_file){
	
	int fd;
	header h;
	char buf;

	fd = open(packed_file, O_RDWR);

	while ( read(fd , &buf, 1)==1 && (buf=='D' || buf=='C') ){

		read_header(fd, &h);

		if ( (h.modo & __S_IFDIR) == __S_IFDIR ){
			mkdir(h.name, h.modo);
		}
		if ( (h.modo & __S_IFMT ) == __S_IFLNK ){
			symlink(h.link_path, h.name);
		}
		if ( ( h.modo & __S_IFMT ) == __S_IFIFO ){
			mkfifo(h.name, h.modo);
		}
		if ( (h.modo & __S_IFREG) == __S_IFREG ){
			fd2 = open( h.name , O_RDWR | O_CREAT | O_TRUNC, h.modo );
			loaddata( fd , fd2 , h.size);
			close(fd2);
		}

	}

}



int main(){
	//
}