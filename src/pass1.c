
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
			strcpy((Lines[i]).token1,p); //p�� ������  Lines[i].token1�� ����
			
		} 
       while(p!=NULL){
		    p=strtok(NULL," ");
		    if(p){
				//printf("2.%s\n",p);
				if(strstr(p,"\n")!=NULL)
				strtok(p,"\n");
				strcpy(Lines[i].token2,p); 
			  }
			p=strtok(NULL," ");//p�� ������  Lines[i].token2�� ����
			
			if(p){
				//if(strlen(p)==0)printf("3.%s\n",p);
				if(strstr(p,"\n")!=NULL)
				strtok(p,"\n");
				strcpy(Lines[i].token3,p); 
			  }//p�� ������  Lines[i].token3�� ����
		}
	   
  	i++;

           }
	
	

 }


void check_lines(LINES *Lines,OPTAB *Optab,OBJECT *Object,SYMTAB*Symtab,LOCTAB*Loctab){
	//Lines���� �˻��ؼ� ��ɾ��  Object�� opcode���� �ϰ� ���þ�� Loc�� ������ ũ�⺰�� �����ϰ� �Ѵ� �ƴϸ� SYMTAB�� �����ϱ�
	
	int m,i=0,j=0,count=0;;
	
	
	while(strlen(Lines[i].token1)!=0){//ù��° ��ū�� null�϶� ���� i++�ؼ� �ݺ�(Lines ���� �f��)

		if(strchr(Lines[i].token1,'.'))//ù��° ��ū �˻��ؼ� Loc_count�ʱ�ȭ
			Loctab[i].Loc_count=0;
		else
			Loctab[i].Loc_count=3; 
		
		while(strlen(Optab[j].name)!=0){//optab name�� null�� ������ j++�ؼ� �ݺ�(optab �f��)
			
				
			if(strcmp(Lines[i].token1,Optab[j].name) ==0){//token1�� ������ �̸�
				strcpy(Object[i].opcode,Optab[j].opcode);//�� ������ opcode �� lines�� object opcode�κп� �����ϱ�
				count++;//���������� ǥ��!
				}
			j++;
			}
		if(count==0 && (strchr(Lines[i].token1,'.') ==0))//token1�� �����ڰ� �ƴϸ�
			strcpy(Symtab[i].Lable,Lines[i].token1);//token1�� symtab�� �ֱ�
	
	count=0;		
	j=0;
	i++;	
		}

	j=0;
	count=0;
	i=0;
	
	while(strlen(Lines[i].token1)!=0){//��� �����f��
		
		while(strlen(Optab[j].name)!=0){//optab�f��

			if(strcmp(Lines[i].token2,Optab[j].name)==0){//token2�� ������ �̸�
				strcpy(Object[i].opcode,Optab[j].opcode);//�� ������ opcode �� lines�� object opcode�κп� �����ϱ�
			    
				}
			j++;
			}
	
	/*//////////////////////////������ó��////////////////////////////////
		/////������ BYTE�� Object.result�� �ٷ����� 
		//////������ WORD�� token3�� ������ Object[i].address�� ���� �ϰ� opcode�� "00"���� ����(���߿� �ѹ��� ó��)
		//////������ RESW,REWB�� Object.result null�ʱⰪ �״�� ���ΰ� �ּҰ��*/
		if(strcmp(Lines[i].token2,"BYTE")==0){
			char *token;
			char check_type[5];
			char check_length[10];
			//char *conver;
			

			token=strtok(Lines[i].token3,"'");
			strcpy(check_type,token);
			token=strtok(NULL,"'");
			strcpy(check_length,token);

			if(strcmp(check_type,"X")==0){ //BYTE X''�϶�
				Loctab[i].Loc_count=1;    //�ּҰ�� 1����
				strcpy(Object[i].result,check_length); //Object[i].result�� ''������ ��Ʈ�� ����
			}
			else if(strcmp(check_type,"C")==0){  //BYTE C''�϶�
				int count2=0;
				char objectcode[10]={0};
				while(check_length[count2]!=0) //''������ ��Ʈ�� �˻�
				{
					char AtoH[5];  
					sprintf(AtoH,"%x",(int)check_length[count2++]);  //��Ʈ���� �ѱ��ھ� �ƽ�Ű�ڵ�� �ٲ㼭
					strcat(objectcode,AtoH);          //char objectcode�� ��� ���̱�
				}
				//conver=strupr(objectcode); //�ҹ��ڴ� �빮�ڷ�!
				for(m=0;m<strlen(objectcode);m++)
                        objectcode[m] = toupper(objectcode[m]);

				strcpy(Object[i].result,objectcode);   //char objectcode�� Object[i].result�� �ȴ�
				Loctab[i].Loc_count=count2;  //�ּҰ�� ��int count ����
			}
		}
		if(strcmp(Lines[i].token2,"RESB")==0){
				Loctab[i].Loc_count=atoi(Lines[i].token3); //token3�� ���� ũ�⸸ŭ �ּҰ��
				
		}
		else if(strcmp(Lines[i].token2,"WORD")==0){   
				Loctab[i].Loc_count=3; //token3�� ������ object address�� �ֱ�
				strcpy(Object[i].opcode,"00");
				Object[i].address=atoi(Lines[i].token3);
				
		}
		else if (strcmp(Lines[i].token2,"RESW")==0){
				Loctab[i].Loc_count=(atoi(Lines[i].token3))*3;//token3�� ���� ũ��*3 ��ŭ �ּҰ��
				
		}
		

	j=0;
	i++;
	}
	
	
}
void Locctr(LINES *Lines,LOCTAB *Loctab){//ù°�� ����°��ū�� �������� �ٲٰ� ��� 3�� ���ؼ� �ּ� ����

	int i=0;

	//puts(Lines[0].token3);//�� �����ּ�
	Loctab[1].Loc=(int)strtol(Lines[0].token3,NULL,16);//Loctab[1]���� �ּ�ī���� ���� 
	//printf("0->%d\n",Loctab[0].Loc);
	for(i=1;strlen(Lines[i].token1)!=0;i++){
		Loctab[i+1].Loc=Loctab[i].Loc+Loctab[i].Loc_count;
		//printf("%d->%d\n",i,Loctab[i].Loc);
	}
}
