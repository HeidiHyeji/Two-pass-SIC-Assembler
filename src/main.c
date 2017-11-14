#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"




int main(void)
{
 
    FILE *fp;
   LINES Lines[1000]={NULL};
   OPTAB Optab[]= {{"ADD","18"},
	{"ADDF","58"},{"ADDR","90"},{"AND","40"},{"CLEAR","B4"},{"COMP","28"},
	{"COMPF","88"},{"COMPR","A0"},{"DIV","24"},{"DIVF","64"},{"DIVR","9C"},
	{"FIX","C4"},{"FLOAT","C0"},{"HIO","F4"},{"J","3C"},{"JEQ","30"},
	{"JGT","34"},{"JLT","38"},{"JSUB","48"},{"LDA","00"},{"LDB","68"},
	{"LDCH","50"},{"LDF","70"},{"LDL","08"},{"LDS","6c"},{"LDT","74"},
	{"LDX","04"},{"LPS","D0"},{"MUL","20"},{"MULF","60"},{"MULR","98"},
	{"NORM","C8"},{"OR","44"},{"RD","D8"},{"RWO","AC"},{"RSUB","4C"},
	{"SHIFTL","A4"},{"SHIFTR","A8"},{"SIO","F0"},{"SSK","EC"},{"STA","0C"},
	{"STB","78"},{"STCH","54"},{"STF","80"},{"STI","D4"},{"STL","14"},
	{"STS","7C"},{"STSW","E8"},{"STT","84"},{"STX","10"},{"SUB","1C"},
	{"SUBF","5C"},{"SUBR","94"},{"SVC","B0"},{"TD","E0"},{"TIO","F8"},
	{"TIX","2C"},{"TIXR","B8"},{"WD","DC"},{"LISTEND","00"}};
   OBJECT Object[1000]={NULL};
   SYMTAB Symtab[1000]={NULL};
   LOCTAB Loctab[1000]={0};

   char fname[100];
   char buffer[100];

   int i;
   
   printf("Enter the name of file you wish to see\n");
   scanf("%s",fname);

   if((fp=fopen(fname,"r"))==NULL){
	   printf("No exist input file!\n");
	   exit(1);
   }

   printf("---------------------This is the input Assembler program .\n");
   pass1(fp,Lines,Optab,Object,Symtab,Loctab);

   printf("\n\n---------------------This is Opject program for the input Assembler.\n");
   pass2(Lines,Object,Symtab,Loctab);

 return 0;
}


