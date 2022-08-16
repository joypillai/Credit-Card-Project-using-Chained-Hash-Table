/*##################################################
Joy Pillai
emp ID2112822
GN22CMT001
###################################################*/

#include<stdio.h>	
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/stat.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
void cmpf(FILE*, FILE*);
extern int errno;
int main(int argc, char *argv[])
{
	int fd1,fd,i=0,j=0;
	char buff[400];
	char buffline[400];
	ssize_t readbytes;
	int ret;
	char buf[10];

	key_t k = ftok ("shmk", 80);
	if((fd1 = open("data.txt",O_CREAT | O_RDWR | O_APPEND, S_IRWXU)) < 0){ 
                fprintf(stderr,"Error in opening file!");
                exit(1);
        }

	printf ("Key obtained is %x\n", k);
	if (k == -1)
	{
		printf ("Errno = %d\n", errno);
		exit (1);
	}
	
	int shmid = shmget (k, 0, 0);

	printf ("Shm identifier is %x\n", shmid);
	if (shmid == -1)
	{
		printf ("Errno = %d\n", errno);
		exit (1);
	}

	void *attach_addr = shmat (shmid, 0, 0);

	printf ("Attached at %x\n", attach_addr);
	if ((int) attach_addr == -1)
	{
		printf ("Errno = %d\n", errno);
		exit (1);
	}
	struct sembuf sembuf_array[2];
        int semid = semget (ftok ("semk", 16), 0, 0); 

        if (semid == -1) { printf ("semget errno = %d\n", errno); exit (1); }
	
	while((readbytes = read(fd1, buff,sizeof(buff))) > 0 ){

		i=0;

		for(i=0;i<readbytes;i++){
			if(buff[i]=='\n'){
				//waiting for s0
				memset (&sembuf_array, 0, sizeof (sembuf_array));
				sembuf_array[0].sem_num = 0;
				sembuf_array[0].sem_op = -1; 
				ret = semop (semid, sembuf_array, 1);    
				if (ret == -1) 
				{ printf ("semop errno = %d\n", errno); exit (1); }

				buffline[j]=buff[i];
				strcpy (attach_addr,buffline);
				j=0;
				printf("%s\n",buffline);
				sleep(0.30);
				memset(buffline,0,400);

				//signal for s1
				memset (&sembuf_array, 0, sizeof (sembuf_array));
				sembuf_array[0].sem_num = 1;
				sembuf_array[0].sem_op = 1;
				ret = semop (semid, sembuf_array, 1);
				if (ret == -1)
				{ printf ("semop errno = %d\n", errno); exit (1); }
				sleep(0.10);

			}
			else{
				buffline[j]=buff[i];
				j++;
			}
		}
		memset(buff,0,400);
	}
	fd=open("consumerpid",O_RDWR);
	read(fd,buf,sizeof(buf));
	int pid=atoi(buf);
	kill(pid,15);
	shmdt(attach_addr);
	shmctl(shmid,IPC_RMID,NULL);
	semctl(semid,1,IPC_RMID);
	semctl(semid,2,IPC_RMID);
	printf("\n");	
	printf("Deleting shared memory and semaphores from Kernel!\n");	
	printf("\n");	
	
	FILE *f1 = fopen("data.txt", "r");
	FILE *f2 = fopen("ReceivedData.txt", "r");

	if (f1 == NULL || f2 == NULL)
	{
		printf("Error : Files not open");
		exit(0);
	}

	cmpf(f1, f2);

	fclose(f1);
	fclose(f2);

	return 0;
}
// function to compare 2 file elements
void cmpf(FILE *f1, FILE *f2)
{
	char ch1=getc(f1);
	char ch2=getc(f2);
	int error=0,pos=0,line=1;
	while (ch1 != EOF && ch2 != EOF){
		pos++;
		if (ch1 == '\n' && ch2 == '\n'){
			line++;
			pos = 0;
		}

		if (ch1 != ch2){
			error++;
			printf("Line Number : %d \tError"
					" Position : %d \n", line, pos);
		}
		ch1=getc(f1);
		ch2=getc(f2);
	}
	printf("Comparing Both the files...........\n");
	printf("Errors found : %d\n", error);
	if(error==0){
		printf("Both files Match and Contents copied properly! \n");
	}
}
