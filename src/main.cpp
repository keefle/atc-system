#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <ctime>

#include "threader.hh"
#include "plane.hh"
#include "sleep.hh"
#include "tower.hh"

#define TICK 1

// Defaults of program falgs
float probability     = 0.5;
int   simulation_time = 60;
int   start_time      = 0;
int   log_after       = 0;

// The following void functions are to be used when creating threads for planes
// and the tower.

void *start_tower(void* t) {
    Tower* tower = (Tower*)t;
    tower->start();
    pthread_exit(NULL);
}

void *thread_arrive_request(void* t) {
    Tower* tower = (Tower*)t;
    Plane* plane = new Plane(ARRIVING, 2, start_time);
    std::string ID = plane->getID();
    tower->request(plane);
    pthread_exit(NULL);
}

void *thread_depart_request(void* t) {
    Tower* tower = (Tower*)t;
    Plane* plane = new Plane(DEPARTING, 2, start_time);
    std::string ID = plane->getID();
    tower->request(plane);
    pthread_exit(NULL);
}

void *thread_emergency_request(void* t) {
    Tower* tower = (Tower*)t;
    Plane* plane = new Plane(EMERGENCY, 2, start_time);
    std::string ID = plane->getID();
    tower->request(plane);
    pthread_exit(NULL);
}


void print_usage() {
    std::cout << "Usage: atc -n <log after n seconds> -s <simulation length> -p <probability>" << std::endl;
    exit(2);
}


int main(int argc, char **argv) {

    // Read and parse flags (arguments) supplied by the user
    int option;
    while ((option = getopt(argc, argv, "p:n:s:")) != -1) {
        switch (option) {
            case 'p':
                probability = atof(optarg);
                break;
            case 'n':
                log_after = atoi(optarg);
                break;
            case 's':
                simulation_time = atoi(optarg);
                break;
            default:
                print_usage();
                abort();
        }
    }


    Threader threader;
    Tower tower;

    start_time = int(std::time(0));
    srand(start_time);

    std::cout << "== SIMULATION STARTED ==" << std::endl;

    // Launch tower on the background.
    threader.run_tower(start_tower, &tower);

    // Add two planes. One departing and one arriving.
    threader.run_plane(thread_depart_request, &tower);
    threader.run_plane(thread_arrive_request, &tower);

    bool timeout = false;
    int seconds_passed = 1;

    // Start main loop
    while(not timeout) {

        // Check which planes are to be created during this TICK
        bool new_arriving_plane  = seconds_passed % TICK == 0 && (rand() % 100) < probability * 100;
        bool new_departing_plane = seconds_passed % TICK == 0 && (rand() % 100) < (100 - probability * 100);
        bool new_emergency_plane = seconds_passed % 40 * TICK == 0;

        if (new_emergency_plane) {
            threader.run_plane(thread_emergency_request, &tower);
        }

        if(new_arriving_plane) {
            threader.run_plane(thread_arrive_request, &tower);
        }

        if(new_departing_plane) {
            threader.run_plane(thread_depart_request, &tower);
        }

        // Start reporting current queues' statuses after the "second" specified by the user.
        if(seconds_passed >= log_after) {
            std::cout << "At sec " << seconds_passed << " ground: " << tower.list_waiting(DEPARTING) << std::endl;
            std::cout << "At sec " << seconds_passed << " air: " << tower.list_waiting(ARRIVING) << tower.list_waiting(EMERGENCY) << std::endl;
        }

        // If the simulation is over mark this as the last iteration
        if(seconds_passed == simulation_time) {
            timeout = true;
            tower.shutdown();
            std::cout << "== SIMULATION FINISHED ==" << std::endl;
            std::cout << "Releasing resources......" << std::endl;
        }
        pthread_sleep(TICK);
        seconds_passed++;
    }

    // Destroy all remaining threads
    // (planes that are still waiting on the tower and the tower will be killed)
    threader.join_tower();

    return 0;
}
