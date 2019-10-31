#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tar.h"

#define __MAY_HAVE_ARG  (__F_IFF | __F_IFO | __F_IFZ | __F_IFY | __F_IFS | __F_IFV)
#define __MUST_HAVE_ARG (__F_IFF | __F_IFO | __F_IFZ | __F_IFY | __F_IFS)

int valid_name(char * str){
   
   if(str[0] == '-') return 0;

   return 1;
}

int check_conflict(int mask){
   
   if((mask & __F_IFT) && (mask & __F_IFC)) return 1;
   if((mask & __F_IFO) && (mask & __F_IFC)) return 1;
   if((mask & __F_IFO) && (mask & __F_IFT)) return 1;
   if((mask & __F_IFZ) && (mask & __F_IFT)) return 1;
   if((mask & __F_IFZ) && (mask & __F_IFO)) return 1;
   if((mask & __F_IFY) && (mask & __F_IFC)) return 1;
   if((mask & __F_IFY) && (mask & __F_IFZ)) return 1;
   if((mask & __F_IFX) && (mask & __F_IFC)) return 1;
   if((mask & __F_IFX) && (mask & __F_IFT)) return 1;
   if((mask & __F_IFX) && (mask & __F_IFZ)) return 1;
   if((mask & __F_IFS) && (mask & __F_IFC)) return 1;
   if((mask & __F_IFS) && (mask & __F_IFN)) return 1;
   if((mask & __F_IFS) && (mask & __F_IFT)) return 1;
   if((mask & __F_IFS) && (mask & __F_IFO)) return 1;
   if((mask & __F_IFS) && (mask & __F_IFZ)) return 1;
   if((mask & __F_IFS) && (mask & __F_IFX)) return 1;
   if((mask & __F_IFV) && (mask & __F_IFT)) return 1;

   if((mask & __F_IFX) && !(mask & __F_IFF)) return 1;
   if((mask & __F_IFT) && !(mask & __F_IFF)) return 1;
   if((mask & __F_IFS) && !(mask & __F_IFF)) return 1;
   if((mask & __F_IFO) && !(mask & __F_IFX)) return 1;
   if((mask & __F_IFZ) && !(mask & __F_IFC)) return 1;
   if((mask & __F_IFY) && !(mask & __F_IFX) && !(mask & __F_IFT) && !(mask & __F_IFS)) return 1;
   if((mask & __F_IFN) && !(mask & __F_IFX) && !(mask & __F_IFT) && !(mask & __F_IFC)) return 1;

   return 0;
}

int main(int argc, char *argv[]) {
   
   int FLAG_MASK = 0, FLAG_MASK_AUX;
   int i, j, len, args_number=0;
   int is_flag[26];
   char c;
   char * flag_args[26];
   char ** other_args;

   other_args = (char **) malloc(sizeof(char*)*argc);

   for(i=0; i<26; i++) is_flag[i] = 0, flag_args[i] = NULL;

   is_flag['f'-'a'] = 1;
   is_flag['c'-'a'] = 2;
   is_flag['n'-'a'] = 3;
   is_flag['t'-'a'] = 4;
   is_flag['o'-'a'] = 5;
   is_flag['z'-'a'] = 6;
   is_flag['y'-'a'] = 7;
   is_flag['x'-'a'] = 8;
   is_flag['s'-'a'] = 9;
   is_flag['v'-'a'] = 10;

   for(i=1; i<argc; i++){
      //printf("jeje %d\n", i);
      
      if(argv[i][0] == '-'){
         
         len = strlen(argv[i]);
         FLAG_MASK_AUX = 0;

         for(j = 1; j < len; j++){
            
            c = argv[i][j];
            
            if(c>'z' || c<'c' || !is_flag[c - 'a'] ) {printf("pelaste mano 1\n"); return -1;}
            if(FLAG_MASK & (1<<is_flag[c - 'a']) ) {printf("pelaste mano 2\n"); return -1;}
            
            FLAG_MASK_AUX |= (1<<is_flag[c - 'a']);

            if((FLAG_MASK_AUX & __MUST_HAVE_ARG)!=0 && j<len-1 ) {printf("pelaste mano 3\n"); return -1;}

         }

         if(FLAG_MASK_AUX & __MAY_HAVE_ARG){
            
            if(i+1 >= argc) {printf("pelaste mano 4\n"); return -1;}
            if(!valid_name(argv[i+1])) {printf("pelaste mano 5\n"); return -1;}
            
            c = argv[i][len-1];
            flag_args[c - 'a'] = (char *) malloc(strlen(argv[i+1])+1);
            strcpy( flag_args[c - 'a'], argv[i+1] ); 
            i++;

         }

         FLAG_MASK |= FLAG_MASK_AUX;

      }else{
         
         if(!valid_name(argv[i])) {printf("pelaste mano 6\n"); return -1;}
         other_args[args_number] = (char *) malloc(strlen(argv[i])+1);
         strcpy( other_args[args_number], argv[i] );
         args_number++;

      }

   }

   if(check_conflict(FLAG_MASK)) {printf("pelaste mano 7\n"); return -1;}
   if(__F_IFC && args_number==0) {printf("pelaste mano 8\n"); return -1;}

   printf("Brujaa lo lograstee!!\n");

   return 0;
}