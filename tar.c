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

int loaddata(int fd1 , int fd2 , int size, int c){
	unsigned char *buff;
	buff = (unsigned char*)malloc(size);
	leer_aux(fd1, buff, size);
	if(c) descifrar_string(buff, size, DESP);
	write_aux(fd2, size, buff);
	free(buff);
}

int get_header( char* path_and_name , header *h ){
	
	struct stat sb;
	int x, y, l;
	unsigned char * c;
	unsigned char * c2;

	x = lstat( path_and_name , &sb );

	h->modo = sb.st_mode;
	h->uid = sb.st_uid;
	h->gid = sb.st_gid;
	h->size = sb.st_size;
	h->num_blocks = sb.st_blocks;
	h->name_size = strlen( (char *) path_and_name );

	c = ( unsigned char * ) malloc( h->name_size );
	strcpy((char *) c , (char *) path_and_name );

	h->name = c;

	if ( ( sb.st_mode & __S_IFMT ) == __S_IFLNK ){	
		
		c = ( unsigned char * )malloc(400);
		l = 0;
		l += readlink( path_and_name , c , 399 );
		c[l] = '\0';
		c2 = ( unsigned char * )malloc( strlen( (char *) c) );
		strcpy((char *) c2, (char *) c);
		h->link_size = l;
		h->link_path = c2;
		free(c);
	
	}else{
	
		h->link_size = 0;
	
	}

	return 0;
}

int read_header( int fd , header* h, int c){
	/* RECORDAR LEER LA CASILLA LIBRE ANTES DE LLAMAR LA FUNCION */
	unsigned char buf[4];
	unsigned char *buf2, *buf3;
	int e;

	e = leer_aux( fd , buf , 4 );
	if(c) descifrar_string(buf, 4, DESP);
	h->modo = str_to_int( buf );

	e = leer_aux( fd , buf , 4 );
	if(c) descifrar_string(buf, 4, DESP);
	h->uid = str_to_int( buf );

	e = leer_aux( fd , buf , 4 );
	if(c) descifrar_string(buf, 4, DESP);
	h->gid = str_to_int( buf );
	
	e = leer_aux( fd , buf , 4 );
	if(c) descifrar_string(buf, 4, DESP);
	h->size = str_to_int( buf );

	e = leer_aux( fd , buf , 4 );
	if(c) descifrar_string(buf, 4, DESP);
	h->num_blocks = str_to_int( buf );

	e = leer_aux( fd , buf , 4 );
	if(c) descifrar_string(buf, 4, DESP);
	h->name_size = str_to_int( buf );

	e = leer_aux( fd , buf , 4 );
	if(c) descifrar_string(buf, 4, DESP);
	h->link_size = str_to_int( buf );


	buf2 = (unsigned char*)malloc( h->name_size );

	e = leer_aux( fd , buf2 , h->name_size );
	if(c) descifrar_string(buf2, h->name_size, DESP);
	h->name = buf2;
	if( h->link_size > 0 ){
		buf3 = (unsigned char*)malloc( h->link_size );
		e = leer_aux( fd , buf3 , h->link_size);
		if(c) descifrar_string(buf3, h->link_size, DESP);
		h->link_path = buf3;
	}
	return 0;
}


unsigned char * header_to_string(header * h){
	
	unsigned char * ret = NULL;
	unsigned char * aux = NULL;
	int size = 29;

	size += h->name_size;
	size += h->link_size;

	ret = (unsigned char*) malloc(sizeof(unsigned char)*size);
	aux = ret + 1;

	ret[0] = 68;
	int_to_char(h->modo, aux);
	int_to_char(h->uid, (aux + 4));
	int_to_char(h->gid, (aux + 8));
	int_to_char(h->size, (aux + 12));
	int_to_char(h->num_blocks, (aux + 16));
	int_to_char(h->name_size, (aux + 20));
	int_to_char(h->link_size, (aux + 24));
	sprintf((aux + 28), "%s", (char *) h->name);
	if ( h->link_size > 0 ) sprintf((aux + 28 + h->name_size), "%s", (char *) h->link_path);

	return ret;
}

void store_header(header * h, int fd, int c){
	unsigned char * buf = header_to_string(h);
	if(c){ 
		cifrar_string(buf, 29 + h->name_size + h->link_size, DESP);
		buf[0] = 'C';
	}
	write_aux(fd, 29 + h->name_size + h->link_size, buf);
}

int save_data( int fd , char* path, int c){
	
	int a, l;
	unsigned char buff[400];

	a = open( path , O_RDONLY );
	l = 1;

	while( l > 0 ){

		l = read( a , buff , 400 );
		if(c) cifrar_string(buff, l, DESP);
		write_aux( fd , l , buff );

	}
}

int pack( int flag_mask, char** argv , int argc, char * pack_file, char * v_output_file, int desp ){ /* flag mask, arreglo de strings a empaquetar, nombre del tar, archivo verboso */
	
	int fd;
	header h;
	int i;

	fd_v_output = 1;
	DESP = 0;

	if(flag_mask & __F_IFZ) DESP = desp;

	if((flag_mask & __F_IFV) == __F_IFV){
		if(v_output_file != NULL) fd_v_output = open(v_output_file, O_RDWR | O_CREAT | O_TRUNC,  S_IRWXU | S_IRWXG | S_IROTH );
		write_aux(fd_v_output, 34, "Starting packing...\nCreating file ");
		write_aux(fd_v_output, strlen(pack_file), pack_file);
		write_aux(fd_v_output, 5, " ...\n");
	}

	fd = open ( pack_file , O_RDWR | O_CREAT | O_TRUNC , S_IRWXU | S_IRWXG | S_IROTH );

	for ( i = 2 ; i < argc ; i ++){

		get_header( argv[i] , &h );

		if((flag_mask & __F_IFN) && ((( h.modo & __S_IFMT ) == __S_IFLNK) || (( h.modo & __S_IFMT ) == __S_IFIFO))) continue;

		store_header( &h , fd, DESP);

		if((flag_mask & __F_IFV) == __F_IFV){
			write_aux(fd_v_output, 8, "Packing ");
			write_aux(fd_v_output, strlen(h.name), h.name);
			write_aux(fd_v_output, 1, "\n");
		}

		if ( ( h.modo & __S_IFMT ) == __S_IFLNK) continue;
		if ( ( h.modo & __S_IFMT ) == __S_IFDIR ) pack_dir(flag_mask, fd , argv[i] );
		if ( ( h.modo & __S_IFMT ) == __S_IFREG ) save_data( fd , argv[i], DESP);
	}
	
}

void pack_dir(int flag_mask, int fd, char * ruta){
  
  DIR* dirp;
  struct dirent* de;
  header h;

  dirp = opendir( ruta );
  while( de = readdir( dirp ) ){

    get_header( make_path(ruta, (de->d_name)), &h);
    
    if((flag_mask & __F_IFN) && ((( h.modo & __S_IFMT ) == __S_IFLNK) || (( h.modo & __S_IFMT ) == __S_IFIFO))) continue;

    if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0 || strcmp(de->d_name, "mytar")==0) continue; /* comparar con el nombre del archivo 'mytar'*/
    
    store_header( &h, fd, DESP);

    if((flag_mask & __F_IFV) == __F_IFV){
		write_aux(fd_v_output, 8, "Packing ");
		write_aux(fd_v_output, strlen(h.name), h.name);
		write_aux(fd_v_output, 1, "\n");
	}



    if(( h.modo & __S_IFMT ) == __S_IFLNK) continue;
    if( (h.modo & __S_IFDIR) == __S_IFDIR) pack_dir(flag_mask, fd, make_path(ruta, (de->d_name)) );
    if( (h.modo & __S_IFREG) == __S_IFREG) save_data(fd, make_path(ruta, (de->d_name)), DESP);
    	
  }

}


int unpack(int flag_mask, char * packed_file, char * v_output_file, char * unpacking_dir, int desp){
	
	int fd, fd2;
	header h;
	unsigned char buf;
	DESP = 0;

	fd = open(packed_file, O_RDWR);

	if(flag_mask & __F_IFO) chdir(unpacking_dir);

	if(flag_mask & __F_IFY) DESP = desp;

	fd_v_output = 1;

	if((flag_mask & __F_IFV) == __F_IFV){
		if(v_output_file != NULL) fd_v_output = open(v_output_file, O_RDWR | O_CREAT | O_TRUNC,  S_IRWXU | S_IRWXG | S_IROTH );
		write_aux(fd_v_output, 33, "Starting unpacking...\nOpening file ");
		write_aux(fd_v_output, strlen(packed_file), packed_file);
		write_aux(fd_v_output, 1, "\n");
	}

	while ( read(fd , &buf, 1)==1 && (buf=='D' || buf=='C') ){

		if((flag_mask & __F_IFY) && buf != 'C'){
			/*manejar error*/
		}

		if( !(flag_mask & __F_IFY) && buf == 'C' ){
			/*manejo de error*/
		}

		read_header(fd, &h, DESP);

		if((flag_mask & __F_IFN) && ((( h.modo & __S_IFMT ) == __S_IFLNK) || (( h.modo & __S_IFMT ) == __S_IFIFO))) continue;

		if((flag_mask & __F_IFV) == __F_IFV){
			write_aux(fd_v_output, 10, "Unpacking ");
			write_aux(fd_v_output, strlen(h.name), h.name);
			write_aux(fd_v_output, 1, "\n");
		}


		if ( (h.modo & __S_IFMT) == __S_IFDIR ){
			mkdir(h.name, h.modo);
		}
		if ( (h.modo & __S_IFMT ) == __S_IFLNK ){
			symlink(h.link_path, h.name);
		}
		if ( ( h.modo & __S_IFMT ) == __S_IFIFO ){
			mkfifo(h.name, h.modo);
		}
		if ( (h.modo & __S_IFMT) == __S_IFREG ){
			fd2 = open( h.name , O_RDWR | O_CREAT | O_TRUNC, h.modo );
			loaddata( fd , fd2 , h.size, DESP);
			close(fd2);
		}

	}

}

/* se llama cuando pasan t, tambien deben pasar f*/
int show_content_file(int flag_mask, char * packed_file, char * v_output_file){
	
	int fd, fd2, i;
	header h;
	unsigned char buf;
	unsigned char * aux;
	DESP = 0;

	aux =  (unsigned char *) malloc(4); 
	aux[0] = 'x';
	aux[1] = 'w';
	aux[2] = 'r';

	fd = open(packed_file, O_RDWR);

	fd_v_output = 1;

	if((flag_mask & __F_IFV) == __F_IFV && (v_output_file != NULL)){
		fd_v_output = open(v_output_file, O_RDWR | O_CREAT | O_TRUNC,  S_IRWXU | S_IRWXG | S_IROTH );
	}

	write_aux(fd_v_output, 20, "Content of the file ");
	write_aux(fd_v_output, strlen(packed_file), packed_file);
	write_aux(fd_v_output, 2, ":\n");

	while ( read(fd , &buf, 1)==1 && (buf=='D' || buf=='C') ){

		read_header(fd, &h, DESP);

		if((flag_mask & __F_IFN) && ((( h.modo & __S_IFMT ) == __S_IFLNK) || (( h.modo & __S_IFMT ) == __S_IFIFO))) continue;

		if( (h.modo & __S_IFMT) == __S_IFDIR ) write_aux(fd_v_output, 1, "d");
		else if( ( h.modo & __S_IFMT ) == __S_IFLNK ) write_aux(fd_v_output, 1, "l");
		else if( ( h.modo & __S_IFMT ) == __S_IFIFO ) write_aux(fd_v_output, 1, "p");
		else write_aux(fd_v_output, 1, "-");

		for(i=8; i>=0; i--){
			/*printf("hola\n");*/
			if((h.modo & (1<<i))) write_aux(fd_v_output, 1, & aux[i%3]);
			else write_aux(fd_v_output, 1, "-");
		}

		write_aux(fd_v_output, 1, " ");

		write_aux_int(fd_v_output, h.uid);
		/*printf("%d", h.uid);*/

		write_aux(fd_v_output, 1, " ");

		write_aux_int(fd_v_output, h.gid);

		write_aux(fd_v_output, 1, " ");

		write_aux_int(fd_v_output, h.size);

		write_aux(fd_v_output, 1, " ");

		write_aux(fd_v_output, strlen(h.name), h.name);

		write_aux(fd_v_output, 1, "\n");

		if ( (h.modo & __S_IFMT) == __S_IFREG ){
			leer_aux(fd, NULL, h.size);
		}

	}

}

void write_aux_int(int fd, int n){
	char buf[11];
	int size = 0;
	int len = 0;
	int x = n;

	if(n==0){
		write_aux(fd, 1, "0");
	}

	while(x) x/=10, size++;

	len = size;

	while(n){
		buf[size-1] = '0' + n%10;
		size--;
		n/=10;
	}

	write_aux(fd, len, buf);

}