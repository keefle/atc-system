#ifndef PLANE_HH
#define PLANE_HH
#include <string>
#include <pthread.h>

enum PlaneStatus {ARRIVING, DEPARTING, EMERGENCY};

class Plane {

    public:
        static pthread_mutex_t arriving_id_lock;
        static pthread_mutex_t departing_id_lock;

    private:
        static unsigned int arriving_id_counter;
        static unsigned int departing_id_counter;

        std::string id;
        PlaneStatus status;
        int request_time = 0;

        int request_time_epoch = 0;
        int runway_time_epoch = 0;

        // Time for plane to takeoff or land in seconds
        int operation_time = 0;

    private:
        void genID();

    public:
        Plane(PlaneStatus _status);
        Plane(PlaneStatus _status, int _operation_time, int _start_time_epoch);
        PlaneStatus getStatus();
        std::string getPrettyStatus();
        std::string getID();
        std::string log();

        // Provides Shorter version of ID
        std::string getPrettyID();
        void notify_tower();
        void perform_action();
        void takeoff();
        void land();
};


#endif
