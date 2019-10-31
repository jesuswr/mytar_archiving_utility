/*
*  File:       tar.c
*  Author:     Jesus Wahrman 15-11540 , Neil Villamizar 15-11523
*  Description:   file that contains the implementation of the mytar 
*              function main
*  Date:       29 / 10 / 19
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tar.h"

#define __MAY_HAVE_ARG  (__F_IFF | __F_IFO | __F_IFZ | __F_IFY | __F_IFS | __F_IFV)
#define __MUST_HAVE_ARG (__F_IFF | __F_IFO | __F_IFZ | __F_IFY | __F_IFS)

/*
*  Function : valid_name
*  ----------------------
*  check if the string str is valid as a name
*  of a file to pack with  mytar
*  str: string to check
*/
int valid_name(char * str){
   
   if(str[0] == '-') return 0;

   return 1;
}

/*
*  Function : check_conflict
*  ----------------------
*  check if the flags pased to mytar
*  contains inconsistencies ( flags that can't be 
*  in the same command )
*
*  mask:  bit mask with the flags
*/
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

/*
*  Function : main
*  ----------------------
*  main function of mytar utility
*  
*/
int main(int argc, char *argv[]) {
   
   int FLAG_MASK = 0, FLAG_MASK_AUX;
   int i, j, len, args_number=0, desp=0;
   int is_flag[26];
   char c;
   char * flag_args[26];
   char ** other_args;

   other_args = (char **) malloc(sizeof(char*)*argc);
   if(other_args == NULL){
      printf("Error allocating memory.\n");
      return -1;
   } 

   for(i=0; i<26; i++) is_flag[i] = 0, flag_args[i] = NULL;

   /* to check if a given char is a valid flag*/
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

   /* iterate over the arguments from command line to parse them */
   for(i=1; i<argc; i++){
      
      if(argv[i][0] == '-'){
         
         len = strlen(argv[i]);
         FLAG_MASK_AUX = 0;

         for(j = 1; j < len; j++){
            
            c = argv[i][j];
            
            if(c>'z' || c<'c' || !is_flag[c - 'a'] ) {
               printf("Invalid format\n"); 
               return -1;
            }
            if(FLAG_MASK & (1<<is_flag[c - 'a']) ) {
               printf("Invalid format\n"); 
               return -1;
            }
            
            FLAG_MASK_AUX |= (1<<is_flag[c - 'a']);

            if((FLAG_MASK_AUX & __MUST_HAVE_ARG)!=0 && j<len-1 ) {
               printf("Invalid format\n"); 
               return -1;
            }

         }

         if(FLAG_MASK_AUX & __MAY_HAVE_ARG){
            
            if(i+1 >= argc) {
               printf("Invalid format\n"); 
               return -1;
            }
            if(!valid_name(argv[i+1])) {
               printf("Invalid format\n"); 
               return -1;
            }
            
            c = argv[i][len-1];
            flag_args[c - 'a'] = (char *) malloc(strlen(argv[i+1])+1);
            if(flag_args[c - 'a'] == NULL){
               printf("Error allocating memory.\n");
               return -1;
            }
            strcpy( flag_args[c - 'a'], argv[i+1] ); 
            i++;

         }

         FLAG_MASK |= FLAG_MASK_AUX;

      }else{
         
         if(!valid_name(argv[i])) {
            printf("Invalid format\n"); 
            return -1;
         }
         other_args[args_number] = (char *) malloc(strlen(argv[i])+1);
         if(other_args[args_number] == NULL){
            printf("Error allocating memory.\n");
            return -1;
         } 
         strcpy( other_args[args_number], argv[i] );
         args_number++;

      }

   }

   if(check_conflict(FLAG_MASK)) {
      printf("Invalid format\n");
      return -1;
   }
   if((__F_IFC & FLAG_MASK) && args_number==0) {
      printf("Invalid format\n");
       return -1;
   }

   if(flag_args['f'-'a'] == NULL){
      flag_args['f'-'a'] = (char *) malloc(9);
      if(flag_args['f'-'a'] == NULL){
         printf("Error allocating memory.\n");
         return -1;
      } 
      strcpy(flag_args['f'-'a'], "mytar.mt");
   }

   /* call the corresponding functions given the flags */
   if( __F_IFC & FLAG_MASK ){
      if(flag_args['z'-'a'] != NULL) desp = atoi(flag_args['z'-'a']);
      pack(FLAG_MASK, other_args, args_number, flag_args['f'-'a'], flag_args['v'-'a'], desp);
   }
   if(__F_IFX & FLAG_MASK){
      if(flag_args['y'-'a'] != NULL) desp = atoi(flag_args['y'-'a']);
      unpack(FLAG_MASK, flag_args['f'-'a'], flag_args['v'-'a'], flag_args['o'-'a'], desp);
   }
   if(__F_IFT & FLAG_MASK){
      show_content_file(FLAG_MASK, flag_args['f'-'a'], flag_args['v'-'a'], desp);
   }
   if(__F_IFS & FLAG_MASK){
      if(flag_args['y'-'a'] != NULL) desp = atoi(flag_args['y'-'a']);
      single_extract(FLAG_MASK, flag_args['s'-'a'], flag_args['f'-'a'], flag_args['v'-'a'], flag_args['o'-'a'], desp);
   }

   return 0;
}