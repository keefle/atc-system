#ifndef WAITLIST_HH
#define WAITLIST_HH

#include "plane.hh"
#include <queue>
#include <string>
#include <pthread.h>

class wait_list {
    private:
        std::queue<Plane*> arrive_wait_list;
        std::queue<Plane*> depart_wait_list;
        std::queue<Plane*> emergency_wait_list;
        pthread_mutex_t lock;

    public:
        wait_list();
        bool is_front(Plane *plane);

        void add(Plane *plane);
        Plane* front(PlaneStatus status);

        void pop(PlaneStatus plane);

        int size_waiting();
        int size_waiting(PlaneStatus status);
        std::string list_waiting(PlaneStatus status);
};

#endif
