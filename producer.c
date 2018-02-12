#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include	<errno.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<pthread.h>
#define SIZE 1024

typedef struct {
	pthread_mutex_t lock;
	pthread_cond_t shout;
	int front;
	int rear;
	int count;
	int endOfFile;
	char bytes[1024];
} buffer;


int main(int argc, char const *argv[]) {
	FILE *file = fopen(argv[1], "r");
	if (argc != 2){
		printf("You must enter in a file \n");
	}
	int shmid;
	key_t key = 4132;
	buffer* newBuff;
	if ((shmid = shmget(key, sizeof(newBuff), 0666 | IPC_CREAT | IPC_EXCL)) != -1) {
		newBuff = (buffer*) shmat(shmid, 0, 0);
		printf("successful creation\n");


		newBuff->front = 0;
		newBuff->count = 0;
		newBuff->endOfFile=0;
		pthread_mutexattr_t attr;
		pthread_condattr_t condAttr;

		pthread_mutexattr_init(&attr);
		pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
		pthread_mutex_init(&newBuff->lock, &attr);

		pthread_condattr_init(&condAttr);
		pthread_condattr_setpshared(&condAttr, PTHREAD_PROCESS_SHARED);
		pthread_cond_init(&newBuff->shout, &condAttr);
	} //shared memory creation

	else if ((shmid = shmget(key, 0, 0)) != -1){
		printf("successful attachment\n" );
		newBuff = (buffer*) shmat(shmid, 0, 0);
	}
	else{
		printf("oops\n");
		exit(0);
	}

	pthread_mutex_lock(&newBuff->lock);
	//newBuff->front = 20;
	while (fscanf(file, "%c", &newBuff->bytes[newBuff->rear]) != EOF) //read in file
	{
		newBuff->count++;
		while (newBuff->count == SIZE){ //buffer is full
			pthread_cond_signal(&newBuff->shout);
			pthread_cond_wait(&newBuff->shout, &newBuff->lock);
		}
		//printf("%c\n", newBuff->bytes[newBuff->rear]);
	newBuff->rear = ((newBuff->rear + 1)%sizeof(newBuff->bytes));
	//newBuff->count++;
	}
	newBuff->endOfFile = 1;
	pthread_cond_signal(&newBuff->shout);
	pthread_mutex_unlock(&newBuff->lock);

	shmdt(&newBuff);

	//pthread_mutexattr_destroy(&attr);
	//pthread_condattr_destroy(&condAttr);

	return 0;
}
