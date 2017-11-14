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
	//�ǿ����ڸ� �а� �ɺ����̺��� �ּ� ã�� ��巹�� ��� ó���ؼ� ������Ʈ�ڵ� �ּҺκ� ó��
	int i=0,j=0;
	while(strlen(Lines[i].token1)!=0){
		
		if(strcmp(Lines[i].token2,"RSUB")!=0 && strcmp(Lines[i].token2,"RESB")!=0 && strcmp(Lines[i].token2,"BYTE")!=0 && strcmp(Lines[i].token2,"WORD")!=0 && strcmp(Lines[i].token2,"RESW")!=0 ){
		//////////��巹�� �����(,X�� token2�� �پ����ִٸ�)//////////////////////////////////////
			if(strstr(Lines[i].token2,",X")!=NULL){
				strtok(Lines[i].token2,",");//�ڸ���
				while(strlen(Lines[j].token1)!=0){//������ ũ�⸸ŭ j �ݺ�	
					if(strcmp(Lines[i].token2,Symtab[j].Lable)==0)//�ǿ����ڸ� �ɺ����̺��� ã��
						if(Loctab[j].Loc<32768)//X�ε�����0�̶��
							Object[i].address=Loctab[j].Loc+32768;//(x�ε���:1)+(ã�� �ɺ��� �ּ�)�� ������Ʈ �ּҿ�  ����
					
					j++;
					}
				}
			

		////////////��巹�� �����(,X�� token3�� �پ����ִٸ�)///////////////////////////////////
			else if(strstr(Lines[i].token3,",X")!=NULL){
				strtok(Lines[i].token3,",");//�ڸ���
				while(strlen(Lines[j].token1)!=0){//������ ũ�⸸ŭ j �ݺ�	
					if(strcmp(Lines[i].token3,Symtab[j].Lable)==0)//�ǿ����ڸ� �ɺ����̺��� ã��
						if(Loctab[j].Loc<32768)//X�ε�����0�̶��
							Object[i].address=Loctab[j].Loc+32768;//(x�ε���:1)+(ã�� �ɺ��� �ּ�)�� ������Ʈ �ּҿ�  ����
					j++;
					//����° ��ū�� �ǿ����ڸ� �ɺ����̺��� ��ã���� �������� �ǿ�������	
				}
			}
        ////////////��巹�� ��� �ƴ϶��(,X�� token2�� token3�� �ٿ��������ʴٸ�)/////////////////
			else if(strstr(Lines[i].token2,",X")==NULL && strlen(Lines[i].token3)==0 ){
				while(strlen(Lines[j].token1)!=0){//������ ũ�⸸ŭ j �ݺ�	
					if(strcmp(Lines[i].token2,Symtab[j].Lable)==0)
					{Object[i].address=Loctab[j].Loc;
					}
					j++;
					}
				}
			else if(strstr(Lines[i].token3,",X")==NULL){
				while(strlen(Lines[j].token1)!=0){//������ ũ�⸸ŭ j �ݺ�	
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
///////////////////////////Object.adress ���������� �ٲ���,Object.opcode��  Object.adress ��ġ��(������ ó�� ����)
//address�κ��� 10�������� 16������ �ٲٰ� strcat �ؼ� opcode�� address�� ���̱�
	int m,i=1;
	char tempt[10];
	//char *conver;
	int count0;
	int k;
	
	while(strlen(Lines[i].token1)!=0){
		if(Object[i].opcode){
			if(strcmp(Lines[i].token2,"BYTE")!=0){ // BYTE ����

				if(strcmp(Lines[i].token1,"RSUB")==0){//RSUB ó��
					strcpy(tempt,"0000");
					strcat(Object[i].opcode,tempt);
					strcpy(Object[i].result,Object[i].opcode);
				}
					

				
				else if(strcmp(Lines[i].token2,"WORD")==0){ //WORD ó��
					//itoa(Object[i].address,tempt,16);
					sprintf(tempt,"%x",Object[i].address);
					count0=4-strlen(tempt);
					for(k=0;k<count0;k++)strcat(Object[i].opcode,"0");
					strcat(Object[i].opcode,tempt);
					strcpy(Object[i].result,Object[i].opcode);
				}
				else if(strcmp(Lines[i].token2,"RSUB")!=0 && strcmp(Lines[i].token2,"WORD")!=0){
					//itoa(Object[i].address,tempt,16);//itoa(����, ��ȯ ����� ���� ����, ����);
					sprintf(tempt,"%x",Object[i].address);
					//conver=strupr(tempt);//�ҹ��ڸ� �빮�ڷ� ��ȯ
					for(m=0;m<strlen(tempt);m++)
                        tempt[m] = toupper(tempt[m]);
					strcat(Object[i].opcode,tempt);
					strcpy(Object[i].result,Object[i].opcode);
				}
			}
		}
		if((strchr(Lines[i].token1,'.') !=NULL)|| (strcmp(Lines[i].token2,"RESW")==0) || (strcmp(Lines[i].token2,"RESB")==0) || (strcmp(Lines[i].token2,"START")==0) || (strcmp(Lines[i].token1,"END")==0))
			Object[i].result[0]='\0';//��ū"."�� RESW�� REWB,STRART,END�� NULL�� �����!!
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

	///////////H ����///////////////////////////
	printf("H");
	
	//���α׷� �̸�
	printf("%s",Lines[0].token1); 
	count=6-strlen(Lines[0].token1);
	for(k=0;k<count;k++)
		printf(" ");

	//�������α׷��� �����ּ�
	count=6-strlen(Lines[0].token3);
	for(k=0;k<count;k++)
		printf("0");
	printf("%s",Lines[0].token3);

	//�������α׷��� ����
	while(strlen(Lines[i].token1)!=0)
		i++;
	//itoa(Loctab[i-2].Loc-Loctab[1].Loc+Loctab[i-2].Loc_count,length,16);//itoa(����, ��ȯ ����� ���� ����, ����);
	sprintf(length,"%x",(Loctab[i-2].Loc-Loctab[1].Loc+Loctab[i-2].Loc_count));
	//conver=strupr(length);//�ҹ��ڸ� �빮�ڷ� ��ȯ
	for(m=0;m<strlen(length);m++)
		length[m] = toupper(length[m]);
	count=6-strlen(length);
	for(k=0;k<count;k++)
		printf("0");
	printf("%s\n",length);

	///////T����//////////////
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
				//�����ڵ� �����ּ�/
				count=6-strlen(textaddress);
				for(k=0;k<count;k++)
					printf("0");
				printf("%s",textaddress);

				//�����ڵ� ���� 
				
				//itoa(strlen(text)/2,length,16);//itoa(����, ��ȯ ����� ���� ����, ����);
				sprintf(length,"%x",strlen(text)/2);
				//conver=strupr(length);//�ҹ��ڸ� �빮�ڷ� ��ȯ
				for(m=0;m<strlen(length);m++)
                        length[m] = toupper(length[m]);
				count=2-strlen(length);
				for(k=0;k<count;k++)
				printf("0");
				
				printf("%s",length);

				//�����ڵ�
				printf("%s\n",text);
				j=1;
				
			
			}
		}
	h++;
	};
	//////////////E����////////////////////////
	printf("E");
	//�������α׷��� �����ּ�
	count=6-strlen(Lines[0].token3);
	for(k=0;k<count;k++)
		printf("0");
	printf("%s\n",Lines[0].token3);
	
			

	
	
}