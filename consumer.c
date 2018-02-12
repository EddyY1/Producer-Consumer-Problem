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
	FILE *file = fopen(argv[1], "w");
	if (argc != 2){
		printf("You must enter in a file name\n");
		exit(0);
	}
	int shmid;
	key_t key = 4132;
	buffer* newBuff;
	//shared memory creation along with intialization stuff
	if ((shmid = shmget(key, sizeof(newBuff), 0666 | IPC_CREAT | IPC_EXCL)) != -1) {
		newBuff = (buffer*) shmat(shmid, 0, 0);
		printf("successful creation\n");
		newBuff->front = 0;
		newBuff->count = 0;
		newBuff->endOfFile = 0;
		pthread_mutexattr_t attr;
		pthread_condattr_t condAttr;

		pthread_mutexattr_init(&attr);
		pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
		pthread_mutex_init(&newBuff->lock, &attr);

		pthread_condattr_init(&condAttr);
		pthread_condattr_setpshared(&condAttr, PTHREAD_PROCESS_SHARED);
		pthread_cond_init(&newBuff->shout, &condAttr);
	}
	//finding shared memory already created
	else if ((shmid = shmget(key, 0, 0)) != -1){
		printf("successful attachment\n" );
		newBuff = (buffer*) shmat(shmid, 0, 0);
	}
	else{ //uh oh
		printf("oops\n");
		exit(0);
	}
	pthread_mutex_lock(&newBuff->lock);
	//do this while the producer hasn't reached EOF and the buffer is empty
	while ((newBuff->endOfFile == 0) || (newBuff->count != 0))
	{

		//while buff is empty
		while (newBuff->count == 0){
			pthread_cond_signal(&newBuff->shout);
			pthread_cond_wait(&newBuff->shout, &newBuff->lock);
		}
		//take two bytes at a time and input them in reverse order
			fprintf(file, "%c%c", newBuff->bytes[newBuff->front + 1], newBuff->bytes[newBuff->front]);
			newBuff->front = ((newBuff->front + 2)%sizeof(newBuff->bytes));
			newBuff->count -= 2;
		//if the byte size is uneven just put last byte in normally
		if (newBuff->count == 1){
			fprintf(file, "%c", newBuff->bytes[newBuff->front]);
			newBuff->front = ((newBuff->front + 1)%sizeof(newBuff->bytes));
			newBuff->count --;
		}
}
	pthread_mutex_unlock(&newBuff->lock);
	shmdt(&newBuff);

	//pthread_mutexattr_destroy(&attr);
	//pthread_condattr_destroy(&condAttr);*/

	return 0;
}
