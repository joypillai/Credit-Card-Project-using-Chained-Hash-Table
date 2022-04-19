#include <stdio.h>
#include "node.h"
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
struct node* retrive(char *proof,char *address,char *fullName,char *email,unsigned long long int cardNo,char *bank,char *namePrint,int dob,int cardIssue,int cardExp,int cvv);
int hashfun(unsigned long long int cardNo){
        return cardNo%1021;    //nearest prime to half of the hashtable
}
void insert_chaining(struct node *HT[],struct node *person1){
        int index=hashfun(person1->cardNo);
        struct node **H=&HT[index];
        struct node *prev=NULL;
        struct node *curr=*H;
        if(*H==NULL)
                *H=person1;
        else{
                while(curr&&(curr->cardNo<person1->cardNo)){
                        prev=curr;
                        curr=curr->next;
                }
                if(prev==NULL){
                        person1->next=curr;
                        *H=person1;
                }
                else{
                        prev->next=person1;
                        person1->next=curr;
                }
        }
}
void search(struct node *HT[],unsigned long long key)
{
       // printf("In search");
	int flag=0;
        struct node *curr=HT[hashfun(key)];

        while(curr!=NULL)
        {
                if(key==curr->cardNo){
                        printf("Full Name     : %s\n",curr->fullName);
                        printf("Address       : %s\n",curr->address);
                        printf("Email         : %s\n",curr->email);
                        printf("Bank          : %s\n",curr->bank);
                        printf("Card Number   : %lld\n",curr->cardNo);
                        printf("Date of Birth : %d\n",curr->dob);
			flag=1;
                }
                curr=curr->next;
        }
if(flag==0)
printf("Entered card NUmber is not registered\n");
}


int main()
{
        struct node *HT[1021];
        for(int i=0;i<1021;i++){
                HT[i]=NULL;
        }
        char buff[200];
	char proof[30];
        char address[30];
        char fullName[30];
        char email[30];
        unsigned long long int cardNo;
        char bank[30];
        char namePrint[30];
        int dob;
        int cardno;
        int cardExp;
        int cardIssue;
        int cvv;
        FILE * fp;
        int fd1;
        fp = fopen ("data.txt", "r");
        fd1=open("storedata",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU);
        if(fd1<0){
                perror("Error in opening data file\n");
                }
        while(1){
                        if(feof(fp)){
                                break;
                        }

                        fscanf(fp,"%s ",proof);
                        fscanf(fp,"%s ",address);
                        fscanf(fp,"%s ",fullName);
                        fscanf(fp,"%s ",email);
                        fscanf(fp,"%llu ",&cardNo);
                        fscanf(fp,"%s ",bank);
                        fscanf(fp,"%s ",namePrint);
                        fscanf(fp,"%d ",&dob);
                        fscanf(fp,"%d ",&cardExp);
                        fscanf(fp,"%d ",&cardIssue);
                        fscanf(fp,"%d ",&cvv);
struct node *person1;
person1=retrive(proof,address,fullName,email,cardNo,bank,namePrint,dob,cardIssue,cardExp,cvv);



                        insert_chaining(HT,person1);
                        char buff[200];
                        sprintf(buff,"%s %s %s %s %llu %s %s %d %d %d %d\n",proof,address,fullName,email,cardNo,bank,namePrint,dob,cardExp,cardIssue,cvv);
                        if(write(fd1,buff,strlen(buff))!=strlen(buff)){
                                perror("Error in writing to database file\n");
                        }
        }
                while(1){
                        int choice;
                        printf("Enter your choice\n 1>Insert\n 2>search\n 3>Display\n 4>Quit\n");
                        scanf("%d",&choice);

                        switch(choice){
                                case 1:printf("Enter your proof\n");
                                       scanf("%s",proof);
                                       printf("Enter your address\n");
                                       scanf("%s",address);
                                       printf("Enter your fullname\n");
                                       scanf("%s",fullName);
                                       printf("Enter your email\n");
                                       scanf("%s",email);
                                       printf("Enter your cardNo \n");
                                       scanf("%llu",&cardNo);
                                       printf("Enter your bank\n");
                                       scanf("%s",bank);
                                       printf("Enter your namePrint\n");
                                       scanf("%s",namePrint);
                                       printf("Enter your cardIssue\n");
                                       scanf("%d",&cardIssue);
                                       printf("Enter your cardExpiry\n");
                                       scanf("%d",&cardExp);
                                       printf("Enter your cvv\n");
                                       scanf("%d",&cvv);
				       struct node *person1;
				       person1=retrive(proof,address,fullName,email,cardNo,bank,namePrint,dob,cardIssue,cardExp,cvv);
                                       insert_chaining(HT,person1);
                                       fd1=open("storedata",O_APPEND|O_RDWR);
                                       sprintf(buff,"%s %s %s %s %llu %s %s %d %d %d %d\n",proof,address,fullName,email,cardNo,bank,namePrint,dob,cardExp,cardIssue,cvv);
                                       if(write(fd1,buff,strlen(buff))!=strlen(buff)){
                                               perror("Error in writing new record\n");
                                       }
                                       fd1=open("data.txt",O_APPEND|O_RDWR);
                                       if(write(fd1,buff,strlen(buff))!=strlen(buff)){
                                               perror("Error in writing\n");
                                       }
                                       break;
                                case 2:printf("Enter the cardNumber\n");
                                       scanf("%llu",&cardNo);
                                       search(HT,cardNo);
                                       break;
                                case 3:fd1=open("storedata",O_RDONLY);
                                       if(fd1<0){
                                               perror("Error in opening the file\n");
                                       }
                                       ssize_t readbytes;
                                       while((readbytes=read(fd1,buff,sizeof(buff)))>0){
                                               write(1,buff,readbytes);

                                       }
                                       break;
                                case 4:exit(1);
                                       break;
                        }
                }
}
struct node* retrive(char *proof,char *address,char *fullName,char *email,unsigned long long int cardNo,char *bank,char *namePrint,int dob,int cardIssue,int cardExp,int cvv){
struct node *person1=malloc(sizeof(struct node));
                                       if(person1==NULL)
                                               fprintf(stderr,"Error in creating structure memory\n");
                                       person1->proof=malloc(sizeof(strlen(proof)+1));
                                       if(person1->proof==NULL)
                                               fprintf(stderr,"Error in allocating memory to proof feild\n");
                                       person1->address=malloc(sizeof(strlen(address)+1));
                                       if(person1->address==NULL)
                                               fprintf(stderr,"Error in allocating memory to address feild\n");
                                       person1->fullName=malloc(sizeof(strlen(fullName)+1));
                                       if(person1->fullName==NULL)
                                               fprintf(stderr,"Error in allocating memory to fullName feild\n");

                                       person1->email=malloc(sizeof(strlen(email)+1));
                                       if(person1->email==NULL)
                                               fprintf(stderr,"Error in allocating memory to email feild\n");

                                       person1->bank=malloc(sizeof(strlen(bank)+1));
                                       if(person1->bank==NULL)
                                               fprintf(stderr,"Error in allocating memory to bank field\n");

                                       person1->namePrint=malloc(sizeof(strlen(namePrint)+1));

                                       if(person1->namePrint==NULL)
                                               fprintf(stderr,"Error in allocating memory to nameprint feild\n");


                                       strcpy(person1->proof,proof);
                                       strcpy(person1->address,address);
                                       strcpy(person1->fullName,fullName);
                                       strcpy(person1->email,email);
                                       person1->cardNo=cardNo;
                                       strcpy(person1->bank,bank);
                                       strcpy(person1->namePrint,namePrint);
                                       person1->dob=dob;
                                       person1->cardExp=cardExp;
                                       person1->cardIssue=cardIssue;
                                       person1->cvv=cvv;
return person1;
}


