#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "incl/clogger.h"

#define NUM_THREADS 500

typedef struct thread_data_t {
        int tid;
        int sleep_time;
} thread_data_t;

void *thread_func(void *arg)
{
        int i = 10;
        char msg[25];
        thread_data_t *t_data = (thread_data_t *) arg;

        while (i--) {
                sprintf(msg, "This is thread %d", t_data->tid);
                clogger_log(msg, ERR);
        }

        pthread_exit(NULL);
}


int main(void)
{
        /* The next two lines are just to make the debugger stop */
        //int debugI;
        //scanf("%d", &debugI);

        int i, rc;
        pthread_t thread[NUM_THREADS];
        thread_data_t thread_data[NUM_THREADS];

        /* Initialize clogger's switches with values from `runtime.conf` */
        clogger_init();
        clogger_log("Starting threads...", INFO);

        /* Spawn threads */
        for (i = 0; i < NUM_THREADS; ++i) {
                thread_data[i].tid = i;
                if ((rc = pthread_create(&thread[i], NULL, thread_func, &thread_data[i]))) {
                        printf("Could not create thread %d\n", i);
                }

        }

        /* Wait for all threads to complete */
        for (i = 0; i < NUM_THREADS; ++i) {
                pthread_join(thread[i], NULL);
        }

        clogger_log("All threads have joined.", FATAL);

        /* Shutdown clogger */
        clogger_terminate();

        return 0;
}

