#ifndef THREADER_HH
#define THREADER_HH
#include <pthread.h>
#include <vector>

class Threader {
    private:
        pthread_t tower;
        std::vector<pthread_t> planes;

    public:
        Threader();
        void run_tower(void *(*start_routine) (void *), void *arg);
        void run_plane(void *(*start_routine) (void *), void *arg);
        void join_tower();
};

#endif
