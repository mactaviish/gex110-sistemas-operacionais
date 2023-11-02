#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int left(int id);
int right(int id);
void *philosopher(void *data);
void *take_forks(int id);
void *put_forks(int id);
void test(int id);

int state[N];
sem_t mutex;
sem_t semaphore[N];

int main(void) {
  int i;
  pthread_t tids[N];

  sem_init(&mutex, 0, 1);
  for(i = 0; i < N; i++) {
    sem_init(&semaphore[i], 0, 0);
    state[i] = THINKING;
    int *j = malloc(sizeof(int));
    *j = i;
    printf("\nCreating philosopher %d\n", *j);
    pthread_create(&tids[i], NULL, philosopher, (void *)j);
  }

  for(i = 0; i < N; i++) {
    pthread_join(tids[i], NULL);
    printf("Thread id %ld returned\n", tids[i]);
  }

  return(1);
}

void *philosopher(void *data) {
  int id = *((int *) data);

  while(1) {
    printf("\nPhilosopher %d is thinking\n", id);
    sleep(2);
    printf("\nPhilosopher %d is hungry\n", id);
    take_forks(id);
    sleep(2);
    printf("\nPhilosopher %d is eating\n", id);
    put_forks(id);
  }
  pthread_exit(NULL);
}

int left(int id) {
  return((id + N - 1) % N);
}

int right(int id) {
  return((id + 1) % N);
}

void *take_forks(int id) {
  sem_wait(&mutex);
  state[id] = HUNGRY;
  test(id);
  sem_post(&mutex);
  sem_wait(&semaphore[id]);
}

void *put_forks(int id) {
  sem_wait(&mutex);
  state[id] = THINKING;
  test(left(id));
  test(right(id));
  sem_post(&mutex);
}

void test(int id) {
  if((state[id] == HUNGRY) && (state[left(id)] != EATING) && (state[right(id)]) != EATING) {
    state[id] = EATING;
    sem_post(&semaphore[id]);
  }
}
