#ifndef TOWER_HH
#define TOWER_HH

#include <fstream>
#include <functional>
#include <pthread.h>
#include <string>

#include "plane.hh"
#include "waitlist.hh"


class Tower {
    private:
        wait_list wl;

        std::ofstream log_file;

        pthread_mutex_t checking_turn;
        pthread_cond_t next_plane_announced;
        pthread_cond_t left_runway;
        pthread_cond_t newly_arrived;

        std::string next_plane_id = "none";

        bool timeout = false;

        // The following memeber variables are used to remove starvation from the
        // system by limiting the number of consectutive uses of each queue
        int consecutive_arriving  = 0;
        int consecutive_departing = 0;
        bool arriving_turn = true;


    private:
        // process_request chooses which plane is next to use the runway, reports
        // if a boolean indicating if a new plane was chosen or not. (in case
        // there are no planes in the queue this will return false)
        bool process_request();

    public:
        Tower();

        // Request adds a plane to the waiting queue and lets the plane know when its the
        // plane's turn to use the runway
        void request(Plane *plane);


        // Start runs the tower managing planes in the waiting list and access to the
        // runway
        void start();

        // Quaries mostly for logging and testing
        int size_waiting();
        int size_waiting(PlaneStatus status);
        std::string list_waiting(PlaneStatus status);

        void shutdown();

    private:
};

#endif
