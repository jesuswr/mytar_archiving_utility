#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include "tar.h"



int main(int argc, char *argv[]) {
   int option;

   int FLAG_MASK = 0;
   char *tar_name, *unpacking_dir, *v_output_file, *file_to_search;
   while((option = getopt(argc, argv, ":f:cnto:z:y:xs:v:")) != -1){ 
      switch(option){
         case 'f':
            FLAG_MASK |= __F_IFF;
            tar_name = (char*)malloc(strlen(optarg));
            strcpy( tar_name , optarg );
            printf("entro en f %s\n", tar_name);
            continue;
         case 'c':
            FLAG_MASK |= __F_IFC;
            printf("entro en c\n");
            continue;
         case 'n':
            FLAG_MASK |= __F_IFN;
            printf("entro en n\n");
            continue;
         case 't':
            FLAG_MASK |= __F_IFT;
            printf("entro en t\n");
            continue;
         case 'o':
            FLAG_MASK |= __F_IFO;
            unpacking_dir = (char*)malloc(strlen(optarg));
            strcpy( unpacking_dir , optarg );
            printf("entro en o %s\n", unpacking_dir);
            continue;
         case 'z':
            FLAG_MASK |= __F_IFZ;
            DESP = atoi( optarg );
            printf("entro en z %d\n", DESP);
            continue;
         case 'y':
            FLAG_MASK |= __F_IFY;
            DESP = atoi( optarg );
            printf("entro en y %d\n", DESP);
            continue;
         case 'x':
            FLAG_MASK |= __F_IFX;
            printf("entro en x \n");
            continue;
         case 's':
            FLAG_MASK |= __F_IFS;
            file_to_search = (char*)malloc(strlen(optarg));
            strcpy( file_to_search , optarg );
            printf("entro en s %s\n", file_to_search);
            continue;
         case 'v':
            FLAG_MASK |= __F_IFV;
            v_output_file = (char*)malloc(strlen(optarg));            
            strcpy( v_output_file , optarg );
            printf("entro en v %s\n", v_output_file);
            continue;
         case ':':
            if ( optopt == 'v'){
               FLAG_MASK |= __F_IFV;            
               v_output_file = NULL;
               printf("No file to save verbose was given. Output will be on the terminal.\n");
            }
            else if ( optopt == 'f' ){
               FLAG_MASK |= __F_IFF;
               printf("No tar name specified. mytar will be used.\n");
               tar_name = "mytar";
            }
            else if ( optopt == 'o'){
               FLAG_MASK |= __F_IFO;
               unpacking_dir = NULL;
               printf("No directory to unpack the files was given. We will use the current one.\n");
            }
            else if ( optopt == 'y' || optopt == 'z'){
               printf("Didn't pass the number of bits to rotate.\n");
               return -1;
            }
            continue;
         case '?':
            printf("Error parsing the command.\n");
            return -1;
      }
   }

   for(; optind < argc; optind++){ 
      printf("Given extra arguments: %s\n", argv[optind]);
   }
}