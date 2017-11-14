#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define LABLE 0


typedef struct LINES
{
	char token1[10];
	char token2[10];
	char token3[10];
}LINES;
	
typedef struct SYMTAB
{
	char Lable[10];
	char Location[10];
}SYMTAB;
typedef struct OPTAB{
	char name[10];
	char opcode[10];
}OPTAB;
typedef struct OBJECT{
	char opcode[10];
	int address;
	char result[10];
}OBJECT;
typedef struct LOCTAB{
	int Loc;
	int Loc_count;
}LOCTAB;

void Tokenizing(FILE *fp,LINES *Lines);
void check_lines(LINES *Lines,OPTAB *Optab,OBJECT *Object,SYMTAB*Symtab,LOCTAB*Loctab);
void check_address(LINES *Lines,OBJECT *Object,SYMTAB*Symtab,LOCTAB*Loctab);

void Locctr(LINES *Lines,LOCTAB *Loctab);
void mkObjectcode(LINES*Lines,OBJECT*Object);
void mkObjectprogram(LOCTAB*Loctab,LINES*Lines,OBJECT*Object);

void pass1(FILE*fp,LINES*Lines,OPTAB*Optab,OBJECT*Object,SYMTAB*Symtab,LOCTAB*Loctab);

void pass2(LINES*Lines,OBJECT*Object,SYMTAB*Symtab,LOCTAB*Loctab);

 