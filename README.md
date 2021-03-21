# Two-pass-SIC-Assembler
## SIC Assembler on Linux🌱

### 1. Design Issue
#### 1) 어셈블러의 아키텍쳐를 결정한다.
Two-pass SIC Asssembler   

👉**pass 1(SYMBOL 정의)**  
- 프로그램내의 모든 문에 주소를 배정한다.(LOCCTR)  
- 패스 2에서 사용하기 위해 모든 레이블에 배정된 주소 값들을 저장한다.(SYMTAB)  
- 어셈블러 지시자들에 관련된 처리를 부분적으로 행한다.(BYTE, RESW 등에 의하	여 정의되는 데이타 영역의 길이 결정과 같은 주소배정에 영향을 주는 처리를 포함	한다.:함수로 처리)  
  
👉**pass 2(명령어를 번역하고 목적 프로그램 생성)**   
- 명령어를 어셈블한다.(연산자 코드를 번역하고 주소를 조사	함.:OPTAB,SYMTAB탐색)  
- BYTE, WORD 등으로 정의되는 데이타 값을 생성한다.(pass2의 지시자 처리 시 생성)  
- 패스 1동안에 이루어지지 않는 어셈블러 지시자의 처리를 한다.(함수처리-if문 사	용하여 지시자마다 각각의 처리)  
- 목적 프로그램과 어셈블러 리스트를 출력한다.  

#### 2) Micro-Instruction Set Table을 위한 데이터구조를 결정한다.  
- SYMTAB: 배열 구조체  
- LOCCTR: 배열 구조체  
- OPTAB: 배열 구조체  

#### 3) Micro-Instruction Set Table의 검색 알고리즘을 결정한다.  
- OPTAB 탐색: 순차 탐색(for문,while 이용)  
- SYMTAB 탐색: 순차 탐색(단, 어드레싱 모드로 “X"가 붙었을경우 ”X"를 지운다.)  

#### 4) 어셈블리언어상의 Addressing Mode 표기방법을 결정한다.  
- 인덱스 주소지정 방식-플래그 비트 X: 모든 주소처리를 정수로 처리하여 계산  
- 직접주소지정 방식   

#### 5) 어셈블리언어상의 Directive 처리 방법을 결정한다.  
pass1에서 지시자를 확인하여 주소배정 후 지시자 모두 처리. BYTE는 Type이 C	일 경우와 X일 경우를 나누어 목적코드 생성, WORD는 10진수를 16진수로 변환 	후 목적코드 생성, RESB와 RESW는 데이터 생성 없이 메모리 예약.  

#### 6) 어셈블리 프로그램에 대한 최종 산출물인 프로그램 Object Program의 구조를 	결정한다.  
목적 코드를 생성하는 함수를 만든 후, 목적프로그램 생성 함수에 H레코드, T레코	트, E레코드 차례대로 생성한다.   

### 2. data flow diagram  
![12](https://user-images.githubusercontent.com/22677083/111906660-ee230f00-8a94-11eb-9da0-31c02beb20ae.png)

### 3. functions
```C
void pass1(FILE*fp,LINES*Lines,OPTAB*Optab,OBJECT*Object,SYMTAB*Symtab,LOCTAB*Loctab);
//Tokenizing, check_lines, Locctr 함수를 호출합니다.
```
```C
void pass2(LINES*Lines,OBJECT*Object,SYMTAB*Symtab,LOCTAB*Loctab);
//check_address,mkObjectcode,mkObjectprogram 함수를 호출합니다.
```
```C
void Tokenizing(FILE *fp,LINES *Lines);
//어셈블러 텍스트 파일을 읽어 한 줄 당 두개 혹은 세개의 토큰을 			
//Line[줄].token1,Line[줄].token2,Line[줄].token3에 저장합니다.
```
```C
void check_lines(LINES *Lines,OPTAB *Optab,OBJECT *Object,SYMTAB*Symtab);
//1)Lines마다 토큰들을 체크합니다. 
//2)어떤 i번째 줄의 토큰이 Optab[j].name과 같다면 Object[i].opcode에 Optab[j].opcode를 저장하고, 
//3)같지 않다면 지시어인지 판별합니다. 만약 지시어라면 지시어대로 메모리의 크기를 Loctab.[i].Loc_countLines에 저장합니다. 이때, BYTE거나 WORD라면 OPCODE를 생성합니다. 
//4)어떤 토큰이 명령어, 지시어 둘 다 아니라면 , Symtab.Label에 저장합니다.
```
```C
void Locctr(LINES *Lines,LOCTAB *Loctab);
//어떤 i번째 줄이라면, 								
//Loctab[i].Loc=Loctab[i-1]+Loctab[i-1].Loc_count입니다.(단, 각 지시자가 가지는 메모리크기(Loctab[i].Loc_count) 처리는 check_lines함수에서 이미 하였습니다.)
```
```C
void check_address(LINES *Lines,OBJECT *Object,SYMTAB*Symtab,LOCTAB*Loctab);
//피연산자(token2나 token3)를 읽고,
//1)어드레싱 모드일 경우,2)어드레싱 모드가 아닐 경우를 나눠 Symtab에서 주소를 찾아 Object.adress에 저장할 주소를 다르게 처리하였습니다. 
```
```C
void mkObjectcode(LINES*Lines,OBJECT*Object);
//Object.opcode와 Object.address를 strcat하여 목적코드를 생성하였습니다.
```
```C
void mkObjectprogram(LOCTAB*Loctab,LINES*Lines,OBJECT*Object);
//헤더 레코드: Lines의 길이를 재서 프로그램의 길이를 구하였습니다.
//텍스트 레코드: 목적코드 시작주소, 목적코드 길이, 목적코드 를 구하였습니다.
//엔드 레코드 
```
### 4. running  
1)입력 파일이 없을 때.  
![noname01ㄴㄹㄴ](https://user-images.githubusercontent.com/22677083/111906662-f24f2c80-8a94-11eb-958e-054fd1f12542.png)

2)입력 파일(copy.txt)가 있을 때.  
![noname01ㄹㄹ](https://user-images.githubusercontent.com/22677083/111906663-f24f2c80-8a94-11eb-9d75-672633561695.png)
