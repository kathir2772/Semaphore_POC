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


int main(void)
{
	struct  res *stuff;

	struct sembuf v;

	key_t shm_key, sem_key;
	int shm_id;
	int sem_id;

	shm_key = ftok("text.txt",'h');
	sem_key = ftok("text.txt",'b');
	shm_id = shmget(shm_key, sizeof(struct res) , IPC_CREAT | 0600);

	sem_id = semget( sem_key, 1 , IPC_CREAT | 0600);


short temp,temp1;


	stuff = shmat ( shm_id , NULL, 0);

	printf("in child%d__%d\n",shm_key,stuff->a);

	semctl(sem_id, 0 ,SETVAL,2);
	while(stuff->a<500)
	{
		//***LOCK***
                temp=v.sem_op;

		v.sem_num = 0;
		v.sem_op = 2;
		v.sem_flg = SEM_UNDO;
		semop(sem_id ,&v, 1);
                temp1=v.sem_op;

		stuff->a +=10;
		//*****UNLOCK******
		v.sem_num = 0;
		v.sem_op = -2;
		v.sem_flg = SEM_UNDO;
		semop(sem_id ,&v, 1);
                fprintf(stderr,"P2: B4 Lock:%d; Aft Lock:%d; Aft Unlock:%d\n",temp,temp1,v.sem_op);

		printf("in child__%d\n",stuff->a);
	}
	semctl(sem_id ,0,IPC_RMID,0);
}	

