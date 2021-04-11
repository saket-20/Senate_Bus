#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<semaphore.h>
#include<unistd.h>

int waiting=0;
pthread_mutex_t mutex;
sem_t bus;

sem_t boarded;
sem_t mut;
void *busfn()
{
        while(1){
        //sem_wait(&mutex);

        pthread_mutex_lock(&mutex);

        int n;
        if(waiting<=50) n=waiting;

        else if(50<waiting) n=50;


        int i;

        for(i=0;i<n;i++)

        {
                sem_post(&bus);

                sem_wait(&boarded);
        }
        if(waiting-50>=0) waiting=waiting-50;

        else if(0>waiting-50) waiting=0;
        sem_wait(&mut);

        printf("Bus is departing. number of riders waiting:%d \n",waiting);


        sem_post(&mut);

        pthread_mutex_unlock(&mutex);

        //depart();
        sleep(1);}
}
void *ridersfn()
{
        while(1){
        //sem_wait(&mutex);

        pthread_mutex_lock(&mutex);
        waiting+=1;
        sem_wait(&mut);

        printf("number of riders waiting:%d\n",waiting);
        sem_post(&mut);
        pthread_mutex_unlock(&mutex);
        sleep(1);

        sem_wait(&bus);

        //board();
        sem_wait(&mut);
        printf("bus has arrived. number of riders waiting: %d \n",waiting--);
        sem_post(&mut);
        sem_post(&boarded);
        sleep(1);}

}
int main()
{
waiting=0;

pthread_mutex_init(&mutex,NULL);

sem_init(&bus,0,0);

sem_init(&boarded,0,1);

sem_init(&mut,0,1);

pthread_t bus_thread,riders_thread[100];
int p;
pthread_create(&bus_thread,NULL,busfn,NULL);

for(p=0;p<50;p++)
{
        pthread_create(&riders_thread[p],NULL,ridersfn,NULL);
}
pthread_join(bus_thread,NULL);
for(p=0;p<50;p++)
{
        pthread_join(riders_thread[p],NULL);
}
return 0;
}
