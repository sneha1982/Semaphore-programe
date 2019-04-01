#include<stdio.h>                                      /* Input/Output */
#include<pthread.h>                                    /* POSIX Threads */
#include<semaphore.h>                                  /* Semaphore*/
#include<stdlib.h>                                     /* Stander Libarary*/
#include<string.h>                                     /*string */


void *writer(void *);
void *reader(void *);

sem_t mutex; 
sem_t db; 
int data = 0, rc = 0; 

int main()
{
	int i, b;
	pthread_t rtid[5], wtid[5];
	
	sem_init(&mutex, 0, 1);
	sem_init(&db, 0, 1);

	for (i = 0; i <= 2; i++)
	{
		pthread_create(&wtid[i], NULL, writer, (void *)i);
		pthread_create(&rtid[i], NULL, reader, (void *)i);
	}

	for (i = 0; i <=2; i++)
	{
		pthread_join(rtid[i], NULL);
	}

	for (i = 0; i <=2; i++)
	{
		pthread_join(wtid[i], NULL);
	}
    sem_destroy(&db); 
    sem_destroy(&mutex);
	return 0;
}

void *reader(void *arg)
{
	while (1) {                                                           /* repeat forever */
		sem_wait(&mutex);                                                 /* get exclusive access to 'rc' */
		rc = rc + 1;                                                      /* one reader more now */
		if (rc == 1)                                                      /* if this is the first reader ... */
			sem_wait(&db); 
		sem_post(&mutex);                                                 /* release exclusive access to 'rc' */
		printf("Data read by the reader rc = %d; data = %d\n", rc, data); /* access the data */
		//sleep(1);
		sem_wait(&mutex);                                                 /* get exclusive access to 'rc' */
		rc = rc - 1;                                                      /* one reader fewer now */
		if (rc == 0)                                                      /* if this is the last reader ... */
			sem_post(&db);
		sem_post(&mutex);
		printf("Data used by the reader rc = %d; data = %d\n", rc, data); /* release exclusive access to 'rc' */
                                                                          /* noncritical region */
	}
}

void *writer(void *arg)
{
	while (1) {                                                           /* repeat forever */
	    printf("Data access by the writer rc = %d data= %d\n",rc, data);  /* noncritical region */
		sem_wait(&db);                                                    /* get exclusive access */
		data++;   
		printf("Data update by the writer rc = %d data= %d\n",rc, data ); /* update the data */
		
		sem_post(&db);                                                    /* release exclusive access */
	}
}
