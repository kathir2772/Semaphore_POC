#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/ipc.h>

struct res
{
	int a; 
	int b;
};

struct  res *stuff;
struct sembuf v;
int shm_id;
int sem_id;
key_t shm_key,sem_key;

int main(void)
{


	shm_key = ftok("text.txt",'h');
	sem_key = ftok("text.txt",'b');
	shm_id = shmget(shm_key, sizeof(struct res) , IPC_CREAT | 0600);
	sem_id = semget( sem_key, 1 , IPC_CREAT | 0600);

	v.sem_num = 0;
	v.sem_op = -1;
	v.sem_flg = SEM_UNDO;
	stuff = shmat ( shm_id , NULL, 0);

	stuff->b = 20;
	stuff->a = 10;

	semctl(sem_id, 0 ,SETVAL,1);
short temp,temp1;
	while(stuff->a<500)
		//while(stuff->a)
	{
		printf("process_1 acquired lock\n");
		//	sleep();
		/***************LoCK*/
		temp=v.sem_op;
		v.sem_num = 0;
		v.sem_op = -2;
		v.sem_flg = SEM_UNDO;
		semop(sem_id ,&v, 1);
		printf("in process_1  %d__%d\n",stuff->a , stuff->b);
		temp1=v.sem_op;
		v.sem_num = 0;
		v.sem_op = 2;
		v.sem_flg = SEM_UNDO;
		semop(sem_id ,&v, 1);
		fprintf(stderr,"P1: B4 Lock:%d; Aft Lock:%d; Aft Unlock:%d\n",temp,temp1,v.sem_op);
		//	stuff->b = 20;
		/***************UnLocK*/
		//	printf("process_1 released lock\n");
	}



}	
