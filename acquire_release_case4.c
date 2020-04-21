#include <stdio.h>
#include <time.h>
#include <pthread.h>

int num_readers = 0;
int num_writers = 0;

pthread_mutex_t acquire_release_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t write_lock = PTHREAD_MUTEX_INITIALIZER;


void acquire_shared_lock() {
     
int has_lock = 0;
     while (! has_lock) {
       pthread_mutex_lock(&acquire_release_lock);
       if (num_readers == 0 && num_writers == 0) {
         num_readers++;
         pthread_mutex_lock(&write_lock);
         has_lock = 1;
       }
       pthread_mutex_unlock(&acquire_release_lock);
       
     }

}

void release_shared_lock() {
  
int has_lock = 0;
     while (! has_lock) {
       pthread_mutex_lock(&acquire_release_lock);
       if (num_readers == 1 && num_writers == 0) {
         num_readers--;
         pthread_mutex_unlock(&write_lock);
         has_lock = 1;
       }
       pthread_mutex_unlock(&acquire_release_lock);
      
     }


}

void acquire_exclusive_lock() {

int has_lock = 0;
     while (! has_lock) {
       pthread_mutex_lock(&acquire_release_lock);
       if (num_readers == 0 && num_writers == 0) {
         num_writers++;
         pthread_mutex_lock(&write_lock);
         has_lock = 1;
       }
       pthread_mutex_unlock(&acquire_release_lock);
       if (! has_lock) {  // delay before trying again for the lock
         struct timespec ten_milliseconds = {0, 10000000};
         nanosleep(&ten_milliseconds, NULL);
       }
     }

}

void release_exclusive_lock() {

int has_lock = 0;
     while (! has_lock) {
       pthread_mutex_lock(&acquire_release_lock);
       if (num_readers == 0 && num_writers == 1) {
         num_writers--;
         pthread_mutex_unlock(&write_lock);
         has_lock = 1;
       }
       pthread_mutex_unlock(&acquire_release_lock);
       
     }


}
