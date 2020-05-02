#include "threader.hh"
#include <signal.h>
#include <pthread.h>


Threader::Threader() {
}

void Threader::run_thread(void *(*start_routine) (void *), void *arg) {
    pthread_t thread;
    pthread_create(&thread, NULL, start_routine, arg);
    threads.push_back(thread);
}

void Threader::cancel_all() {
    for (pthread_t thread : threads) {
        if(pthread_kill(thread, 0) == 0) {
            pthread_cancel(thread);
        }
    }
}

