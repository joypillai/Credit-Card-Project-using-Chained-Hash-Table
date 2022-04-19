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

struct Card * tempcd;
struct Full_Name * tempnm;
struct Address * tempad;
struct Bucklist * tempbk;
struct Bucklist * travel;
struct Bucklist * hash_table[TABLE_SIZE];

int main(int argc, char *argv[]){
	int debug=0;
	int i=0,itr; 
	for(i=0;i<1000;i++){
		hash_table[i]= NULL;
	}
	char buff[256];
	char temp[80];
	ssize_t readbytes;
	int fd1,hashval,j=0,pos=0;
	
	if((fd1 = open("/home/joy/Desktop/project/newdat",O_CREAT | O_RDONLY, S_IRWXU)) < 0){
		fprintf(stderr,"Error in opening file!");
		exit(1);
	}
	while((readbytes = read(fd1, buff,sizeof(buff))) > 0 ){		
		for(i=0;i<readbytes;i++){
			if(pos == 0 && j == 0){
				tempcd=malloc(sizeof(struct Card));
				tempnm=malloc(sizeof(struct Full_Name));
				tempad=malloc(sizeof(struct Address));
				tempbk=malloc(sizeof(struct Bucklist));
				if(tempcd==NULL){ fprintf(stderr,"ptr not assigned!"); exit(1);}
				if(tempnm==NULL){ fprintf(stderr,"ptr not assigned!"); exit(1);}
				if(tempad==NULL){ fprintf(stderr,"ptr not assigned!"); exit(1);}
				if(tempbk==NULL){ fprintf(stderr,"ptr not assigned!"); exit(1);}
				tempcd->Name=tempnm;
				tempcd->Addr=tempad;
				tempbk->next=NULL;
				tempbk->ListCard=tempcd;
			}
			if(buff[i] == 58 || buff[i] == '\n'){
				if(pos == 0){	
					strcpy(tempcd->Name->First_Name,temp);
					printf("name: %s\n",tempcd->Name->First_Name);
					memset(temp,0,80);
					pos = 1;
					j=0;
				}
				else if(pos == 1 ){	
					strcpy(tempcd->Name->Middle_Name,temp);
					printf("midname: %s\n",tempcd->Name->Middle_Name);
					memset(temp,0,80);
					pos = 2;
					j=0;
				}
				else if(pos == 2 ){         
					strcpy(tempcd->Name->Last_Name,temp);
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
					strcpy(tempcd->Identification,temp);
					printf("ID: %s\n",tempcd->Identification);
					memset(temp,0,80);
					pos = 5;
					j=0;
				}
				else if(pos == 5 ){         
					printf("Addr lineprev: %s\n",temp);
					itr=0;	
					while(itr < 32 && itr<strlen(temp)){
						tempcd->Addr->Line[itr]=temp[itr];
						itr++;
					}
					printf("Addr Line: %s\n",tempcd->Addr->Line);	
					memset(temp,0,80);
					pos = 6;
					j=0;
				}
				else if(pos == 6 ){
					printf("The city is %s\n",temp);	         
					strcpy(tempcd->Addr->City,temp);
					printf("After city\n");	
					memset(temp,0,80);
					pos = 7;
					j=0;
				}
				else if(pos == 7 ){ 
					printf("The state is %s\n",temp);	        
					strcpy(tempcd->Addr->State,temp);
					memset(temp,0,80);
					pos = 8;
					j=0;
				}
				else if(pos == 8 ){         
					tempcd->Addr->Pin=atoi(temp);
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
					printf("hashval: %d\n",hashval);
					printf("tempcd->CVV = %s\n",tempcd->CVV);		
					if(hash_table[hashval] == NULL){
						hash_table[hashval]=tempbk;
					}
					else {
						travel=hash_table[hashval];
						while(travel->next != NULL){
							travel = travel->next;
						}
						travel->next=tempbk;
					}
					printf("debug\n");	
				}
			}
			else{
				temp[j]=buff[i];
				j++;	
			}
		}
		memset(buff,0,sizeof(buff));
	}
	for(i=0;i<1000;i++){
		travel = hash_table[i];
		while(travel != NULL){
			//printf("ID: %llu\n",travel->ListCard->Card_Num);
			//printf("Name: %s\n",travel->ListCard->Name->First_Name);
			travel = travel->next;
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


