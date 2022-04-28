#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>	
#include"card.h"

#define TABLE_SIZE 1000

int hashfn(unsigned long long );
unsigned long long atoull(char []);
void userinterface();
void dispdate(char []);
void showall();
void hashlookup();
void appenddata();

struct Card * tempcd;
struct Card * travel;
struct Card * hash_table[TABLE_SIZE];

int main(int argc, char *argv[]){
	int debug=0;
	int i=0,itr=0,sel=0; 
	for(i=0;i<1000;i++){
		hash_table[i]= NULL;
	}
	char buff[256];
	char temp[80];
	char dat[8];
	char idef[16];
	ssize_t readbytes;
	int fd1,hashval,j=0,pos=0;
	
	if((fd1 = open("data.txt",O_CREAT | O_RDWR | O_APPEND, S_IRWXU)) < 0){
		fprintf(stderr,"Error in opening file!");
		exit(1);
	}
	while((readbytes = read(fd1, buff,sizeof(buff))) > 0 ){		
		for(i=0;i<readbytes;i++){
			if(pos == 0 && j == 0){
				tempcd=malloc(sizeof(struct Card));
				if(tempcd==NULL){ fprintf(stderr,"ptr not assigned!"); exit(1);}
				tempcd->next=NULL;
			}
			if(buff[i] == 58 || buff[i] == '\n'){
				if(pos == 0){	
					strcpy(tempcd->First_Name,temp);
					memset(temp,0,80);
					pos = 1;
					j=0;
				}
				else if(pos == 1 ){	
					strcpy(tempcd->Middle_Name,temp);
					memset(temp,0,80);
					pos = 2;
					j=0;
				}
				else if(pos == 2 ){         
					strcpy(tempcd->Last_Name,temp);
					memset(temp,0,80);
					pos = 3;
					j=0;
				}
				else if(pos == 3 ){         
					strcpy(tempcd->DOB,temp);
					memset(temp,0,80);
					pos = 4;
					j=0;
				}
				else if(pos == 4 ){     
					itr=0;    
					while(itr < 16 && itr<strlen(temp)){
                                                tempcd->Identification[itr]=temp[itr];
                                                itr++;
                                        }
					memset(temp,0,80);
					pos = 5;
					j=0;
				}
				else if(pos == 5 ){         
					itr=0;	
					while(itr < 32 && itr<strlen(temp)){
						tempcd->Line[itr]=temp[itr];
						itr++;
					}
					memset(temp,0,80);
					pos = 6;
					j=0;
				}
				else if(pos == 6 ){
					strcpy(tempcd->City,temp);
					memset(temp,0,80);
					pos = 7;
					j=0;
				}
				else if(pos == 7 ){ 
					strcpy(tempcd->State,temp);
					memset(temp,0,80);
					pos = 8;
					j=0;
				}
				else if(pos == 8 ){         
					tempcd->Pin=atoi(temp);
					memset(temp,0,80);
					pos = 9;
					j=0;
				}
				else if(pos == 9 ){         
					strcpy(tempcd->Email,temp);
					memset(temp,0,80);
					pos = 10;
					j=0;
				}
				else if(pos == 10 ){         
					tempcd->Card_Num=atoull(temp);
					memset(temp,0,80);
					pos = 11;
					j=0;
				}
				else if(pos == 11 ){         
					strcpy(tempcd->Card_Exp,temp);
					memset(temp,0,80);
					pos = 12;
					j=0;
				}
				else if(pos == 12 ){         
					strcpy(tempcd->Card_Issue,temp);
					memset(temp,0,80);
					pos = 13;
					j=0;
				}
				else if(pos == 13 ){         
					strcpy(tempcd->CVV,temp);
					memset(temp,0,80);
					pos = 0;
					j=0;
					hashval=hashfn(tempcd->Card_Num);
					if(hash_table[hashval] == NULL){
						hash_table[hashval]=tempcd;
					}
					else {
						travel=hash_table[hashval];
						while(travel->next != NULL){
							travel = travel->next;
						}
						travel->next=tempcd;
					}
				}
			}
			else{
				temp[j]=buff[i];
				j++;	
			}
		}
		memset(buff,0,sizeof(buff));
	}
	printf("Existing File Uploaded....\n");
	while(1){
		userinterface();
		scanf("%d",&sel);
		if(sel == 1){
			showall();
		}
		else if(sel == 2){
			hashlookup();;
		}
		else if(sel == 3){
			appenddata();
			hashval=hashfn(tempcd->Card_Num);
			if(hash_table[hashval] == NULL){
				hash_table[hashval]=tempcd;
			}
			else {
				travel=hash_table[hashval];
				while(travel->next != NULL){
					travel = travel->next;
				}
				travel->next=tempcd;
			}

			itr=0;
			while(itr<8){
				dat[itr]=tempcd->DOB[itr];
				itr++;
			}
			itr=0;
                        while(itr<16){
                                idef[itr]=tempcd->Identification[itr];
				itr++;
                        }
			sprintf(buff,"%s:%s:%s:%s:%s:%s:%s:%s:%d:%s:%llu:%s:%s:%s\n",tempcd->First_Name,tempcd->Middle_Name,tempcd->Last_Name,dat,idef,tempcd->Line,tempcd->City,tempcd->State,tempcd->Pin,tempcd->Email,tempcd->Card_Num,tempcd->Card_Issue,tempcd->Card_Exp,tempcd->CVV);
			write(fd1,buff,strlen(buff));

		}
		else if(sel == 4){
			exit(1);	
		}
		else{
			printf("\nERROR: INVALID INPUT\n");	
		}
	}

	close(fd1);
	return 0; 
}	

int hashfn(unsigned long long x){

	return (int)(((x%10000)*5+(((x/10000)%10000)*3)+(((x/100000000)%10000)))%1000);
}

unsigned long long atoull(char str[]){

	int i=0;
	unsigned long long n=0;
	for(i=0;i<strlen(str);i++){
		n*=10;
		n+=(unsigned long long)(str[i]-48);
	}
	return n;
}

void userinterface(){

	printf("#####Select Operation on the file#####\n");
	printf("\n");
	printf("1.Display all content in the file\n2.Display details of Specific Credit Card Number\n");
	printf("3.Add a new Card Member\n4Exit Program\n");
	printf("\n");
}

void showall(){
	int i,itr;
	
	for(i=0;i<1000;i++){
		travel = hash_table[i];
		printf("\nHash Table %d\n\n",i);    
		while(travel != NULL){
			printf("\n");	
			printf("##############################################################\n");	
			printf("\n");	
			printf("Credit Card Number: %llu\n",travel->Card_Num);
			printf("Customer Name: %s %s %s\n",travel->First_Name,travel->Middle_Name,travel->Last_Name);
			printf("Date Of Birth: ");
			dispdate(travel->DOB);
			printf("\n");	
			printf("Identification Number: ");
			itr=0;
			while(itr < 16){
				printf("%c",tempcd->Identification[itr]);
				itr++;
			}
			printf("\n");	
			printf("Address: %s, %s, %s, %d.\n",travel->Line,travel->City,travel->State,travel->Pin);
			printf("Email: %s\n",travel->Email);
			printf("Issue Date: ");
			dispdate(travel->Card_Issue);
			printf("\n");	
			printf("Expiry Date: ");
			dispdate(travel->Card_Exp);
			printf("\n");	
			printf("CVV: %s\n",travel->CVV);
			printf("\n");	
			printf("##############################################################\n");
			printf("\n");		
			travel = travel->next;
		}    
	}
}

void dispdate(char str[]){
	int i;
	for(i=0;i<8;i++){
		if(i==2 || i==4){
			printf("/");	
		}
		printf("%c",str[i]);	
	}
}

void appenddata(){
	int itr;
	char buff[512];
	tempcd=malloc(sizeof(struct Card));
	if(tempcd==NULL){ fprintf(stderr,"ptr not assigned!"); exit(1);}
	tempcd->next=NULL;
	getchar();
	printf("Enter Customer details!\n");
	printf("\n");		
	printf("Enter First name:\n");
	scanf("%[^\n]s",tempcd->First_Name);
	getchar();
	printf("Enter Middle name:\n");
	scanf("%[^\n]s",tempcd->Middle_Name);
	getchar();
	printf("Enter Last name:\n");
	scanf("%[^\n]s",tempcd->Last_Name);
	getchar();
	printf("Enter DOB in DDMMYYYY format only!\n");
	itr=0;
	while(itr < 8){
                scanf("%c",&tempcd->DOB[itr]);
                itr++;
        }
	getchar();
	printf("Enter Government Identification Number:\n");
	itr=0;
	while(itr < 16){
		scanf("%c",&tempcd->Identification[itr]);
		itr++;
	}
	getchar();
	printf("Enter Address line 1:\n");
	scanf("%[^\n]s",tempcd->Line);
	getchar();
	printf("Enter City:\n");
	scanf("%[^\n]s",tempcd->City);
	getchar();
	printf("Enter State:\n");
	scanf("%[^\n]s",tempcd->State);
	getchar();
	printf("Enter Pin:\n");
	scanf("%d",&tempcd->Pin);
	getchar();
	printf("Enter Email:\n");
	scanf("%[^\n]s",tempcd->Email);
	printf("Enter Credit Card Number:\n");
	scanf("%llu",&tempcd->Card_Num);
	getchar();
	printf("Enter Credit Card Issue date in DDMMYYYY format:\n");
	scanf("%[^\n]s",tempcd->Card_Issue);
	getchar();
	printf("Enter Credit Card Expiry Date in DDMMYYYY format:\n");
	scanf("%[^\n]s",tempcd->Card_Exp);
	getchar();
	printf("Enter 3 digit CVV:\n");
	scanf("%[^\n]s",tempcd->CVV);
	getchar();
}

void hashlookup(){
	int hashval,itr;
	unsigned long long n;
	printf("Enter a Credit Card Number you want to Lookup\n");
	scanf("%llu",&n);
	hashval=hashfn(n);
	travel=hash_table[hashval];
	while(travel!=NULL){
		if(travel->Card_Num == n){
			printf("\n");
			printf("##############################################################\n");
			printf("\n");
			printf("Credit Card Number: %llu\n",travel->Card_Num);
			printf("Customer Name: %s %s %s\n",travel->First_Name,travel->Middle_Name,travel->Last_Name);
			printf("Date Of Birth: ");
			dispdate(travel->DOB);
			printf("\n");
			printf("Identification Number: ");
                        itr=0;
                        while(itr < 16){
                                printf("%c",tempcd->Identification[itr]);
                                itr++;
                        }
			printf("\n");	
			printf("Address: %s, %s, %s, %d.\n",travel->Line,travel->City,travel->State,travel->Pin);
			printf("Email: %s\n",travel->Email);
			printf("Issue Date: ");
			dispdate(travel->Card_Issue);
			printf("\n");
			printf("Expiry Date: ");
			dispdate(travel->Card_Exp);
			printf("\n");
			printf("CVV: %s\n",travel->CVV);
			printf("\n");
			printf("##############################################################\n");
			printf("\n");
			return;
		}
		travel=travel->next;
	}
	printf("\n");	
	printf("Entered Credit Card Number Not Found\n");
	printf("\n");	
	return;
}
