#include <pthread.h>
#include <cassert>

#include "tower.hh"
#include "plane.hh"

void test_construct() {
    Tower t;
}

void *start_tower(void* t) {
    Tower* tower = (Tower*)t;
    tower->start();
    pthread_exit(NULL);
}

void test_request_and_start() {
    pthread_t tower_thread;
    Tower t;
    pthread_create(&tower_thread, NULL, start_tower, (void *)(&t));

    Plane* plane1 = new Plane(ARRIVING);
    Plane* plane2 = new Plane(DEPARTING);
    Plane* plane3 = new Plane(ARRIVING);

    t.request(plane1);
    assert(t.size_waiting() == 0);
    assert(t.size_waiting(ARRIVING) == 0);
    assert(t.size_waiting(DEPARTING) == 0);

    t.request(plane2);
    assert(t.size_waiting() == 0);
    assert(t.size_waiting(ARRIVING) == 0);
    assert(t.size_waiting(DEPARTING) == 0);

    t.request(plane3);
    assert(t.size_waiting() == 0);
    assert(t.size_waiting(ARRIVING) == 0);
    assert(t.size_waiting(DEPARTING) == 0);

    pthread_cancel(tower_thread);
}


int main() {
    test_construct();
    test_request_and_start();
}
