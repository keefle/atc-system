#ifndef THREADER_HH
#define THREADER_HH
#include <pthread.h>
#include <vector>

class Threader {
    private:
        std::vector<pthread_t> threads;

    public:
        Threader();
        void run_thread(void *(*start_routine) (void *), void *arg);
        void cancel_all();
};

#endif
