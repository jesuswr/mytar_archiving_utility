#include <stdio.h>
#include <unistd.h>

#define __F_IFF 1<<1
#define __F_IFC 1<<2
#define __F_IFN 1<<3
#define __F_IFT 1<<4
#define __F_IFO 1<<5
#define __F_IFZ 1<<6
#define __F_IFY 1<<7
#define __F_IFX 1<<8
#define __F_IFS 1<<9
#define __F_IFV 1<<10

main(int argc, char *argv[]) {
   int option;
   // put ':' at the starting of the string so compiler can distinguish between '?' and ':'
   /*while((option = getopt(argc, argv, ":if:lrx")) != -1){ //get option from the getopt() method
      switch(option){
         //For option i, r, l, print that these are options
         case 'i':
         case 'l':
         case 'r':
            printf("Given Option: %c\n", option); // i,l o r queda en option
            break;
         case 'f': //here f is used for some file name (f pide argumento, este queda en optarg)
            printf("Given File: %s\n", optarg);
            break;
         case ':': //pusiste opcion valida que necesita argumento, pero no diste argumento
            printf("option needs a value\n");
            break;
         case '?': //used for some unknown options (opcion no valida)
            printf("unknown option: %c\n", optopt);
            break;
      }
   }*/


   while((option = getopt(argc, argv, ":f:cnto:z:y:x:s:v:")) != -1){ //get option from the getopt() method
      switch(option){
         case 'f':
         case 'c':
         case 'n':
         case 't':
         case 'o':
         case 'z':
         case 'y':
         case 'x':
         case 's':
         case 'v':
         case ':':
         case '?':
         
         
         case 'i':
         case 'l':
         case 'r':
            printf("Given Option: %c\n", option); // i,l o r queda en option
            break;
         case 'f': //here f is used for some file name (f pide argumento, este queda en optarg)
            printf("Given File: %s\n", optarg);
            break;
         case ':': //pusiste opcion valida que necesita argumento, pero no diste argumento
            printf("option needs a value\n");
            break;
         case '?': //used for some unknown options (opcion no valida)
            printf("unknown option: %c\n", optopt);
            break;
      }
   }

   for(; optind < argc; optind++){ //when some extra arguments are passed
      printf("Given extra arguments: %s\n", argv[optind]);
   }
}