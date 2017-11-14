#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"


void pass2(LINES*Lines,OBJECT*Object,SYMTAB*Symtab,LOCTAB*Loctab){
	check_address(Lines,Object,Symtab,Loctab);
	mkObjectcode(Lines,Object);
	mkObjectprogram(Loctab,Lines,Object);
}
void check_address(LINES *Lines,OBJECT *Object,SYMTAB*Symtab,LOCTAB*Loctab){
	//피연산자를 읽고 심볼테이블에서 주소 찾고 어드레싱 모드 처리해서 오브젝트코드 주소부분 처리
	int i=0,j=0;
	while(strlen(Lines[i].token1)!=0){
		
		if(strcmp(Lines[i].token2,"RSUB")!=0 && strcmp(Lines[i].token2,"RESB")!=0 && strcmp(Lines[i].token2,"BYTE")!=0 && strcmp(Lines[i].token2,"WORD")!=0 && strcmp(Lines[i].token2,"RESW")!=0 ){
		//////////어드레싱 모드라면(,X가 token2에 붙어져있다면)//////////////////////////////////////
			if(strstr(Lines[i].token2,",X")!=NULL){
				strtok(Lines[i].token2,",");//자르고
				while(strlen(Lines[j].token1)!=0){//라인의 크기만큼 j 반복	
					if(strcmp(Lines[i].token2,Symtab[j].Lable)==0)//피연산자를 심볼테이블에서 찾음
						if(Loctab[j].Loc<32768)//X인덱스가0이라면
							Object[i].address=Loctab[j].Loc+32768;//(x인덱스:1)+(찾은 심볼의 주소)를 오브젝트 주소에  넣음
					
					j++;
					}
				}
			

		////////////어드레싱 모드라면(,X가 token3에 붙어져있다면)///////////////////////////////////
			else if(strstr(Lines[i].token3,",X")!=NULL){
				strtok(Lines[i].token3,",");//자르고
				while(strlen(Lines[j].token1)!=0){//라인의 크기만큼 j 반복	
					if(strcmp(Lines[i].token3,Symtab[j].Lable)==0)//피연산자를 심볼테이블에서 찾음
						if(Loctab[j].Loc<32768)//X인덱스가0이라면
							Object[i].address=Loctab[j].Loc+32768;//(x인덱스:1)+(찾은 심볼의 주소)를 오브젝트 주소에  넣음
					j++;
					//세번째 토큰인 피연산자를 심볼테이블에서 못찾으면 지시자의 피연산자임	
				}
			}
        ////////////어드레싱 모드 아니라면(,X가 token2랑 token3에 붙여져있지않다면)/////////////////
			else if(strstr(Lines[i].token2,",X")==NULL && strlen(Lines[i].token3)==0 ){
				while(strlen(Lines[j].token1)!=0){//라인의 크기만큼 j 반복	
					if(strcmp(Lines[i].token2,Symtab[j].Lable)==0)
					{Object[i].address=Loctab[j].Loc;
					}
					j++;
					}
				}
			else if(strstr(Lines[i].token3,",X")==NULL){
				while(strlen(Lines[j].token1)!=0){//라인의 크기만큼 j 반복	
					if(strcmp(Lines[i].token3,Symtab[j].Lable)==0)
						Object[i].address=Loctab[j].Loc;
					j++;
					}
			}
			
		}
		i++;
		j=0;
		
	}
	
}

void mkObjectcode(LINES*Lines,OBJECT*Object){
///////////////////////////Object.adress 십육진수로 바꾼후,Object.opcode랑  Object.adress 합치기(지시자 처리 주의)
//address부분을 10진수에서 16진수로 바꾸고 strcat 해서 opcode랑 address랑 붙이기
	int m,i=1;
	char tempt[10];
	//char *conver;
	int count0;
	int k;
	
	while(strlen(Lines[i].token1)!=0){
		if(Object[i].opcode){
			if(strcmp(Lines[i].token2,"BYTE")!=0){ // BYTE 제외

				if(strcmp(Lines[i].token1,"RSUB")==0){//RSUB 처리
					strcpy(tempt,"0000");
					strcat(Object[i].opcode,tempt);
					strcpy(Object[i].result,Object[i].opcode);
				}
					

				
				else if(strcmp(Lines[i].token2,"WORD")==0){ //WORD 처리
					//itoa(Object[i].address,tempt,16);
					sprintf(tempt,"%x",Object[i].address);
					count0=4-strlen(tempt);
					for(k=0;k<count0;k++)strcat(Object[i].opcode,"0");
					strcat(Object[i].opcode,tempt);
					strcpy(Object[i].result,Object[i].opcode);
				}
				else if(strcmp(Lines[i].token2,"RSUB")!=0 && strcmp(Lines[i].token2,"WORD")!=0){
					//itoa(Object[i].address,tempt,16);//itoa(숫자, 변환 결과를 담을 버퍼, 진법);
					sprintf(tempt,"%x",Object[i].address);
					//conver=strupr(tempt);//소문자를 대문자로 변환
					for(m=0;m<strlen(tempt);m++)
                        tempt[m] = toupper(tempt[m]);
					strcat(Object[i].opcode,tempt);
					strcpy(Object[i].result,Object[i].opcode);
				}
			}
		}
		if((strchr(Lines[i].token1,'.') !=NULL)|| (strcmp(Lines[i].token2,"RESW")==0) || (strcmp(Lines[i].token2,"RESB")==0) || (strcmp(Lines[i].token2,"START")==0) || (strcmp(Lines[i].token1,"END")==0))
			Object[i].result[0]='\0';//토큰"."과 RESW와 REWB,STRART,END에 NULL로 지우기!!
		i++;
	}

	
}
void mkObjectprogram(LOCTAB*Loctab,LINES*Lines,OBJECT*Object){

	int count;
	int h,k,m,i=0,j=1;
	int l=0;
	char length[10];
	//char*conver;
	//char *conver2;
	char text[70]={NULL};
	char textaddress[10];
	char tempt[10];

	///////////H 라인///////////////////////////
	printf("H");
	
	//프로그램 이름
	printf("%s",Lines[0].token1); 
	count=6-strlen(Lines[0].token1);
	for(k=0;k<count;k++)
		printf(" ");

	//목적프로그램의 시작주소
	count=6-strlen(Lines[0].token3);
	for(k=0;k<count;k++)
		printf("0");
	printf("%s",Lines[0].token3);

	//목적프로그램의 길이
	while(strlen(Lines[i].token1)!=0)
		i++;
	//itoa(Loctab[i-2].Loc-Loctab[1].Loc+Loctab[i-2].Loc_count,length,16);//itoa(숫자, 변환 결과를 담을 버퍼, 진법);
	sprintf(length,"%x",(Loctab[i-2].Loc-Loctab[1].Loc+Loctab[i-2].Loc_count));
	//conver=strupr(length);//소문자를 대문자로 변환
	for(m=0;m<strlen(length);m++)
		length[m] = toupper(length[m]);
	count=6-strlen(length);
	for(k=0;k<count;k++)
		printf("0");
	printf("%s\n",length);

	///////T라인//////////////
	h=1;
	j=1;
	while(strlen(Lines[h].token1)!=0){
		
		if(strchr(Lines[h].token1,'.')==NULL){
			if(j==1){
				//itoa(Loctab[h].Loc,tempt,16);
				sprintf(tempt,"%x",Loctab[h].Loc);
				//conver2=strupr(tempt);
				for(m=0;m<strlen(tempt);m++)
                        tempt[m] = toupper(tempt[m]);
				strcpy(textaddress,tempt);
				strcpy(text,Object[h].result);
				j++;
				}
			
			else
				strcat(text,Object[h].result); 
				j++;
			if(j==12 || (h==i-2)){
				printf("T");
				//목적코드 시작주소/
				count=6-strlen(textaddress);
				for(k=0;k<count;k++)
					printf("0");
				printf("%s",textaddress);

				//목적코드 길이 
				
				//itoa(strlen(text)/2,length,16);//itoa(숫자, 변환 결과를 담을 버퍼, 진법);
				sprintf(length,"%x",strlen(text)/2);
				//conver=strupr(length);//소문자를 대문자로 변환
				for(m=0;m<strlen(length);m++)
                        length[m] = toupper(length[m]);
				count=2-strlen(length);
				for(k=0;k<count;k++)
				printf("0");
				
				printf("%s",length);

				//목적코드
				printf("%s\n",text);
				j=1;
				
			
			}
		}
	h++;
	};
	//////////////E라인////////////////////////
	printf("E");
	//목적프로그램의 시작주소
	count=6-strlen(Lines[0].token3);
	for(k=0;k<count;k++)
		printf("0");
	printf("%s\n",Lines[0].token3);
	
			

	
	
}