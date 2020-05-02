#include <cassert>
#include <iostream>
#include "waitlist.hh"
#include "plane.hh"

void test_add() {
    wait_list wl;
    Plane* plane1 = new Plane(ARRIVING);
    Plane* plane2 = new Plane(DEPARTING);
    Plane* plane3 = new Plane(DEPARTING);
    Plane* plane4 = new Plane(EMERGENCY);

    wl.add(plane1);
    wl.add(plane2);
    wl.add(plane3);
    wl.add(plane4);

    assert(wl.size_waiting(ARRIVING) == 1);
    assert(wl.size_waiting(DEPARTING) == 2);
    assert(wl.size_waiting(EMERGENCY) == 1);

    delete plane1;
    delete plane2;
    delete plane3;
    delete plane4;
}

void test_is_front() {
    wait_list wl;
    Plane* plane1 = new Plane(ARRIVING);
    Plane* plane2 = new Plane(DEPARTING);
    Plane* plane3 = new Plane(DEPARTING);
    Plane* plane4 = new Plane(EMERGENCY);

    wl.add(plane1);
    wl.add(plane2);
    wl.add(plane3);
    wl.add(plane4);

    assert(wl.is_front(plane1));
    assert(wl.is_front(plane2));
    assert(wl.is_front(plane4));

    delete plane1;
    delete plane2;
    delete plane3;
    delete plane4;
}

void test_front() {
    wait_list wl;
    Plane* plane1 = new Plane(ARRIVING);
    Plane* plane2 = new Plane(DEPARTING);
    Plane* plane3 = new Plane(DEPARTING);
    Plane* plane4 = new Plane(EMERGENCY);

    wl.add(plane1);
    wl.add(plane2);
    wl.add(plane3);
    wl.add(plane4);

    assert(wl.front(ARRIVING) == plane1);
    assert(wl.front(DEPARTING) == plane2);
    assert(wl.front(EMERGENCY) == plane4);

    delete plane1;
    delete plane2;
    delete plane3;
    delete plane4;
}

void test_waiting() {
    wait_list wl;
    Plane* plane1 = new Plane(ARRIVING);
    Plane* plane2 = new Plane(DEPARTING);
    Plane* plane3 = new Plane(DEPARTING);
    Plane* plane4 = new Plane(EMERGENCY);

    wl.add(plane1);
    assert(wl.size_waiting() == 1);
    assert(wl.size_waiting(ARRIVING) == 1);
    assert(wl.size_waiting(DEPARTING) == 0);

    wl.add(plane2);
    assert(wl.size_waiting() == 2);
    assert(wl.size_waiting(ARRIVING) == 1);
    assert(wl.size_waiting(DEPARTING) == 1);

    wl.add(plane3);
    assert(wl.size_waiting() == 3);
    assert(wl.size_waiting(ARRIVING) == 1);
    assert(wl.size_waiting(DEPARTING) == 2);

    wl.add(plane4);
    assert(wl.size_waiting() == 4);
    assert(wl.size_waiting(ARRIVING) == 1);
    assert(wl.size_waiting(DEPARTING) == 2);
    assert(wl.size_waiting(EMERGENCY) == 1);

    delete plane1;
    delete plane2;
    delete plane3;
    delete plane4;
}

void test_pop() {
    wait_list wl;
    Plane* plane1 = new Plane(ARRIVING);
    Plane* plane2 = new Plane(DEPARTING);
    Plane* plane3 = new Plane(DEPARTING);
    Plane* plane4 = new Plane(EMERGENCY);

    wl.add(plane1);
    wl.add(plane2);
    wl.add(plane3);
    wl.add(plane4);

    wl.pop(ARRIVING);
    assert(wl.size_waiting(ARRIVING) == 0);
    assert(wl.size_waiting() == 3);

    wl.pop(DEPARTING);
    assert(wl.size_waiting(DEPARTING) == 1);
    assert(wl.size_waiting() == 2);

    wl.pop(DEPARTING);
    assert(wl.size_waiting(DEPARTING) == 0);
    assert(wl.size_waiting() == 1);

    wl.pop(EMERGENCY);
    assert(wl.size_waiting(EMERGENCY) == 0);
    assert(wl.size_waiting() == 0);

    delete plane1;
    delete plane2;
    delete plane3;
    delete plane4;
}

void test_list_waiting() {
    wait_list wl;
    Plane* plane1 = new Plane(ARRIVING);
    Plane* plane2 = new Plane(DEPARTING);
    Plane* plane3 = new Plane(DEPARTING);
    Plane* plane4 = new Plane(EMERGENCY);

    wl.add(plane1);
    wl.add(plane2);
    wl.add(plane3);
    wl.add(plane4);

    std::string list_arrive = wl.list_waiting(ARRIVING);
    std::string list_depart = wl.list_waiting(DEPARTING);
    std::string list_emergency = wl.list_waiting(EMERGENCY);

    assert(list_arrive.find(plane1->getPrettyID()) != std::string::npos);
    assert(list_depart.find(plane2->getPrettyID()) != std::string::npos);
    assert(list_depart.find(plane3->getPrettyID()) != std::string::npos);
    assert(list_emergency.find(plane4->getPrettyID()) != std::string::npos);


    delete plane1;
    delete plane2;
    delete plane3;
    delete plane4;
}

int main() {
    test_add();
    test_is_front();
    test_front();
    test_waiting();
    test_pop();
    test_list_waiting();
    return 0;
}
