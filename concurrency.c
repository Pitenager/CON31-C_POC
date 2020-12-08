/**************************************************************/
/*     CON31-C. Do not destroy a mutex while it is locked    */
/*               AUTHOR: PABLO RUIZ ENCINAS                 */
/***********************************************************/

#include <stdatomic.h>
#include <stddef.h>
#include <threads.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

mtx_t lock;
/* Atomic so multiple threads can increment safely */
atomic_int completed = ATOMIC_VAR_INIT(0);
enum { max_threads = 5 };


int do_work_non_compliant(void *arg) {
  int *i = (int *)arg;
  
  if (*i == 0) { /* Creation thread */
    if (thrd_success != mtx_init(&lock, mtx_plain)) {
      /* Handle error */
      fprintf(stderr,"Error while initializing the mutex\n");
      exit(1);
    }
    atomic_store(&completed, 1);
  } else if (*i < max_threads - 1) { /* Worker thread */
    if (thrd_success != mtx_lock(&lock)) {
      /* Handle error */
      fprintf(stderr,"Error while locking the mutex\n");
      exit(1);
    }
    /* Access data protected by the lock */
    atomic_fetch_add(&completed, 1);
    if (thrd_success != mtx_unlock(&lock)) {
      /* Handle error */
      fprintf(stderr,"Error while unlocking the mutex\n");
      exit(1);
    }
  } else { /* Destruction thread */
    mtx_destroy(&lock);
  }
  return 0;
}
  
int do_work_compliant(void *dummy) {
  if (thrd_success != mtx_lock(&lock)) {
    /* Handle error */
      fprintf(stderr,"Error while locking the mutex\n");
      exit(1);
  }
  /* Access data protected by the lock */
  atomic_fetch_add(&completed, 1);
  if (thrd_success != mtx_unlock(&lock)) {
      /* Handle error */
      fprintf(stderr,"Error while unlocking the mutex\n");
      exit(1);
    }
 
  return 0;
}

int main(int argc, char *argv[]) {
  
  int argument;

  if (argc == 2) {
    argument = atoi(argv[1]);
  } 
  else {
    fprintf(stderr,"You must provide at least 1 argument\n");
    exit(1);
  }
  if (argument == 0) {
    thrd_t threads[max_threads];
    
    for (size_t i = 0; i < max_threads; i++) {
      if (thrd_success != thrd_create(&threads[i], do_work_non_compliant, &i)) {
        /* Handle error */
        fprintf(stderr,"Error while creatings threads\n");
        exit(1);
      }
      fprintf(stdout,"Valor de variable atomica tras lanzar %d threads: %d\n",(int)i+1,completed);
    }
    for (size_t i = 0; i < max_threads; i++) {
      if (thrd_success != thrd_join(threads[i], 0)) {
        /* Handle error */
        fprintf(stderr,"Error while joining threads\n");
        exit(1);
      }
      fprintf(stdout,"Valor de variable atomica tras hacer join del thread %d: %d\n",(int)i,completed);
    }
  }

  else if (argument == 1) {
    thrd_t threads[max_threads];
    
    if (thrd_success != mtx_init(&lock, mtx_plain)) {
      /* Handle error */
      fprintf(stderr,"Error while initializing mutex\n");
      exit(1);
    }
    for (size_t i = 0; i < max_threads; i++) {
      if (thrd_success != thrd_create(&threads[i], do_work_compliant, NULL)) {
        /* Handle error */
        fprintf(stderr,"Error while creatings threads\n");
        exit(1);
      }
      fprintf(stdout,"Valor de variable atomica tras lanzar %d threads: %d\n",(int)i+1,completed);
    }
    for (size_t i = 0; i < max_threads; i++) {
      if (thrd_success != thrd_join(threads[i], 0)) {
        /* Handle error */
        fprintf(stderr,"Error while joining threads\n");
        exit(1);
      }
      fprintf(stdout,"Valor de variable atomica tras hacer join del thread %d: %d\n",(int)i,completed);
    }
  
    mtx_destroy(&lock);
    }

  else {
    fprintf(stderr,"Wrong parameter, choose between 0 and 1\n");
    exit(1);
  }
  
  return 0;
}
