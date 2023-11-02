#include <stdio.h>
#include <pthread.h>

void *my_thread(void *data);
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

#define N 3
#define MAX 10

int global = 0;

int main(void) {
  pthread_t tids[N];
  int i;

  for(i = 0; i < N; i++) {
    pthread_create(&tids[i], NULL, my_thread, NULL);
  }

  for(i = 0; i < N; i++) {
    pthread_join(tids[i], NULL);
    printf("Thread ID %ld returned.\n", tids[i]);
  }

  return(1);
}

void *my_thread(void *data) {
  pthread_mutex_lock(&count_mutex);
  while(global < MAX) {
    global++;
    printf("Thread ID %ld: global is now %d.\n", pthread_self(), global);
    sleep(1);
  }
  global = 0;
  pthread_mutex_unlock(&count_mutex);
  pthread_exit(NULL);
}
