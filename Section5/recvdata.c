/*##################################################
Joy Pillai
emp ID2112822
GN22CMT001
###################################################*/

#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/sem.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
extern int errno;
union semun
{
  int val;
  struct semid_ds *buf;
  ushort *array;
};

int main(int argc, char *argv[])
{
	int fw1;
	char *recvbuff;
	recvbuff=malloc(sizeof(char)*400);
	if(recvbuff==NULL){ fprintf(stderr,"ptr not assigned!"); exit(1);}
	unsigned short sem_values[2] = { 1, 0 };
  	union semun ctlun;

	if((fw1 = open("ReceivedData.txt",O_CREAT | O_RDWR | O_APPEND, S_IRWXU)) < 0){ 
                fprintf(stderr,"Error in opening file!");
                exit(1);
        }
	ftruncate(fw1,0);
	key_t k = ftok ("shmk", 80);

	printf ("Key obtained is %x\n", k);
	if (k == -1)
	{
		printf ("Errno = %d\n", errno);
		exit (1);
	}

	int shmid = shmget (k, 400, IPC_CREAT | IPC_EXCL | S_IRWXU);
	printf ("Shm identifier is %x\n", shmid);
	if (shmid == -1)
	{
		printf ("Errno = %d\n", errno);
		exit (1);
	}

	void *attach_addr = shmat (shmid, 0, SHM_RDONLY);

	printf ("Attached at %x\n", attach_addr);
	if ((int) attach_addr == -1)
	{
		printf ("Errno = %d\n", errno);
		exit (1);
	}
	//int semid = semget (ftok ("semk", 16), 0, 0);
	int semid = semget (ftok ("semk", 16), 2,  
			IPC_CREAT | IPC_EXCL | S_IRWXU); 
	if (semid == -1) { printf ("semget errno = %d\n", errno); exit (1); }
	struct sembuf sembuf_array[2];
	int i;

	ctlun.array = sem_values;

	int ret = semctl (semid, 0, SETALL, ctlun);
	printf ("setctl returned %d\n", ret);
	if (ret == -1) 
	{   
		printf ("semctl errno = %d\n", errno);
		exit (1);
	}
	int fd;
	fd=open("consumerpid",O_CREAT|O_TRUNC,S_IRWXU);
	int pid=getpid();
	char buff_pid[10];
	sprintf(buff_pid,"%d",pid);
	fd=open("consumerpid",O_RDWR);
	write(fd,buff_pid,sizeof(buff_pid));
	while(1)
	{   
		//waiting for s1
		memset (&sembuf_array, 0, sizeof (sembuf_array));
		sembuf_array[0].sem_num = 1;
		sembuf_array[0].sem_op = -1;
		int ret = semop (semid, sembuf_array, 1);
		if (ret == -1) { printf ("semop errno = %d\n", errno); exit (1); }

		printf ("%s\n", attach_addr);
		strcpy(recvbuff,attach_addr);
		write(fw1,recvbuff,strlen(recvbuff));
		sleep(0.10);

		//signal to s0
		memset (&sembuf_array, 0, sizeof (sembuf_array));
		sembuf_array[0].sem_num = 0;
		sembuf_array[0].sem_op = 1;
		ret = semop (semid, sembuf_array, 1);
		if (ret == -1) { printf ("semop errno = %d\n", errno); exit (1); }
	}

}
