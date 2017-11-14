
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"

void pass1(FILE*fp,LINES*Lines,OPTAB*Optab,OBJECT*Object,SYMTAB*Symtab,LOCTAB*Loctab){
	Tokenizing(fp,Lines);
	check_lines(Lines,Optab,Object,Symtab,Loctab);
	Locctr(Lines,Loctab);
}
void Tokenizing(FILE *fp,LINES *Lines)
{

	char buffer[1000];
	char *p;
	int i=0;
	

	while(fgets(buffer,1000,fp)) 
       {
		printf("%s",buffer);
	    p=strtok(buffer," ");
        if(p)
		{
			
			//printf("1.%s\n",p);
			if(strstr(p,"\n")!=NULL)
				strtok(p,"\n");
			strcpy((Lines[i]).token1,p); //p의 내용을  Lines[i].token1에 저장
			
		} 
       while(p!=NULL){
		    p=strtok(NULL," ");
		    if(p){
				//printf("2.%s\n",p);
				if(strstr(p,"\n")!=NULL)
				strtok(p,"\n");
				strcpy(Lines[i].token2,p); 
			  }
			p=strtok(NULL," ");//p의 내용을  Lines[i].token2에 저장
			
			if(p){
				//if(strlen(p)==0)printf("3.%s\n",p);
				if(strstr(p,"\n")!=NULL)
				strtok(p,"\n");
				strcpy(Lines[i].token3,p); 
			  }//p의 내용을  Lines[i].token3에 저장
		}
	   
  	i++;

           }
	
	

 }


void check_lines(LINES *Lines,OPTAB *Optab,OBJECT *Object,SYMTAB*Symtab,LOCTAB*Loctab){
	//Lines별로 검사해서 명령어면  Object에 opcode저장 하고 지시어면 Loc에 지시자 크기별로 저장하고 둘다 아니면 SYMTAB에 저장하기
	
	int m,i=0,j=0,count=0;;
	
	
	while(strlen(Lines[i].token1)!=0){//첫번째 토큰이 null일때 까지 i++해서 반복(Lines 전부 훓기)

		if(strchr(Lines[i].token1,'.'))//첫번째 토큰 검사해서 Loc_count초기화
			Loctab[i].Loc_count=0;
		else
			Loctab[i].Loc_count=3; 
		
		while(strlen(Optab[j].name)!=0){//optab name이 null일 때까지 j++해서 반복(optab 훓기)
			
				
			if(strcmp(Lines[i].token1,Optab[j].name) ==0){//token1이 연산자 이면
				strcpy(Object[i].opcode,Optab[j].opcode);//그 연산자 opcode 그 lines의 object opcode부분에 저장하기
				count++;//연산자임을 표시!
				}
			j++;
			}
		if(count==0 && (strchr(Lines[i].token1,'.') ==0))//token1이 연산자가 아니면
			strcpy(Symtab[i].Lable,Lines[i].token1);//token1을 symtab에 넣기
	
	count=0;		
	j=0;
	i++;	
		}

	j=0;
	count=0;
	i=0;
	
	while(strlen(Lines[i].token1)!=0){//모든 라인훓기
		
		while(strlen(Optab[j].name)!=0){//optab훓기

			if(strcmp(Lines[i].token2,Optab[j].name)==0){//token2가 연산자 이면
				strcpy(Object[i].opcode,Optab[j].opcode);//그 연산자 opcode 그 lines의 object opcode부분에 저장하기
			    
				}
			j++;
			}
	
	/*//////////////////////////지시자처리////////////////////////////////
		/////지시자 BYTE는 Object.result에 바로저장 
		//////지시자 WORD는 token3의 정수를 Object[i].address에 저장 하고 opcode는 "00"으로 저장(나중에 한번에 처리)
		//////지시자 RESW,REWB는 Object.result null초기값 그대로 내두고 주소계산*/
		if(strcmp(Lines[i].token2,"BYTE")==0){
			char *token;
			char check_type[5];
			char check_length[10];
			//char *conver;
			

			token=strtok(Lines[i].token3,"'");
			strcpy(check_type,token);
			token=strtok(NULL,"'");
			strcpy(check_length,token);

			if(strcmp(check_type,"X")==0){ //BYTE X''일때
				Loctab[i].Loc_count=1;    //주소계산 1저장
				strcpy(Object[i].result,check_length); //Object[i].result에 ''사이의 스트링 저장
			}
			else if(strcmp(check_type,"C")==0){  //BYTE C''일때
				int count2=0;
				char objectcode[10]={0};
				while(check_length[count2]!=0) //''사이의 스트링 검사
				{
					char AtoH[5];  
					sprintf(AtoH,"%x",(int)check_length[count2++]);  //스트링의 한글자씩 아스키코드로 바꿔서
					strcat(objectcode,AtoH);          //char objectcode에 계속 붙이기
				}
				//conver=strupr(objectcode); //소문자는 대문자로!
				for(m=0;m<strlen(objectcode);m++)
                        objectcode[m] = toupper(objectcode[m]);

				strcpy(Object[i].result,objectcode);   //char objectcode는 Object[i].result가 된다
				Loctab[i].Loc_count=count2;  //주소계산 에int count 저장
			}
		}
		if(strcmp(Lines[i].token2,"RESB")==0){
				Loctab[i].Loc_count=atoi(Lines[i].token3); //token3의 정수 크기만큼 주소계산
				
		}
		else if(strcmp(Lines[i].token2,"WORD")==0){   
				Loctab[i].Loc_count=3; //token3의 정수를 object address에 넣기
				strcpy(Object[i].opcode,"00");
				Object[i].address=atoi(Lines[i].token3);
				
		}
		else if (strcmp(Lines[i].token2,"RESW")==0){
				Loctab[i].Loc_count=(atoi(Lines[i].token3))*3;//token3의 정수 크기*3 만큼 주소계산
				
		}
		

	j=0;
	i++;
	}
	
	
}
void Locctr(LINES *Lines,LOCTAB *Loctab){//첫째줄 세번째토큰을 십진수로 바꾸고 계속 3을 더해서 주소 만듦

	int i=0;

	//puts(Lines[0].token3);//는 시작주소
	Loctab[1].Loc=(int)strtol(Lines[0].token3,NULL,16);//Loctab[1]부터 주소카운팅 시작 
	//printf("0->%d\n",Loctab[0].Loc);
	for(i=1;strlen(Lines[i].token1)!=0;i++){
		Loctab[i+1].Loc=Loctab[i].Loc+Loctab[i].Loc_count;
		//printf("%d->%d\n",i,Loctab[i].Loc);
	}
}
