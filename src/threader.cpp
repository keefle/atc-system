#include "threader.hh"
#include <signal.h>


Threader::Threader() {
}

void Threader::run_plane(void *(*start_routine) (void *), void *arg) {
    pthread_t thread;
    pthread_create(&thread, NULL, start_routine, arg);
    planes.push_back(thread);
}

void Threader::run_tower(void *(*start_routine) (void *), void *arg) {
    pthread_create(&tower, NULL, start_routine, arg);
}


void Threader::join_tower() {
    pthread_join(tower, NULL);
}

