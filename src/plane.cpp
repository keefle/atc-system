#include "plane.hh"
#include "sleep.hh"
#include <sstream>
#include <ctime>

unsigned int Plane::arriving_id_counter = 0;
unsigned int Plane::departing_id_counter = 1;
pthread_mutex_t Plane::arriving_id_lock;
pthread_mutex_t Plane::departing_id_lock;

int _ = pthread_mutex_init(&Plane::arriving_id_lock, NULL);
int __ = pthread_mutex_init(&Plane::departing_id_lock, NULL);

Plane::Plane(PlaneStatus _status, int _operation_time, int start_time_epoch) {
    status = _status;
    request_time_epoch = int(std::time(0));
    request_time = request_time_epoch - start_time_epoch;
    operation_time = _operation_time;
    genID();
}

Plane::Plane(PlaneStatus _status) {
    status = _status;
    genID();
}

PlaneStatus Plane::getStatus() {
    return status;
};

std::string Plane::getPrettyStatus() {
    switch(status) {
        case ARRIVING:
            return "L";
        case DEPARTING:
            return "D";
        case EMERGENCY:
            return "(E)L";
        default:
            throw "error: uknown status cannot return pretty format";
    }
};

std::string Plane::getID() {
    return id;
};

std::string Plane::getPrettyID() {
    return id.substr(0, 7);
};

void Plane::notify_tower() {
    return;
};

void Plane::perform_action() {
    switch(status) {
        case ARRIVING:
            land();
            break;
        case EMERGENCY:
        case DEPARTING:
            takeoff();
            break;
        default:
            throw "error: no action to be performed for unknown type";
    }

    runway_time_epoch = int(std::time(0));
    completed = true;
    return;
}

void Plane::takeoff() {
    pthread_sleep(operation_time);
    return;
}

void Plane::land() {
    pthread_sleep(operation_time);
    return;
}

void Plane::genID() {
    switch(status) {
        case ARRIVING:
        case EMERGENCY:
            pthread_mutex_lock(&arriving_id_lock);
            id = std::to_string(arriving_id_counter);
            arriving_id_counter += 2;
            pthread_mutex_unlock(&arriving_id_lock);
            break;
        case DEPARTING:
            pthread_mutex_lock(&departing_id_lock);
            id = std::to_string(departing_id_counter);
            departing_id_counter += 2;
            pthread_mutex_unlock(&departing_id_lock);
            break;
        default:
            throw "error: cannot generate ID for unknown type";
    }
}

std::string Plane::log() {
    std::ostringstream log;
    if (completed) {
        log << getPrettyID() << "\t\t" << getPrettyStatus() << "\t" << request_time << "\t\t" << request_time + runway_time_epoch - request_time_epoch << "\t\t" << runway_time_epoch - request_time_epoch << std::flush;
    } else {
        log << getPrettyID() << "\t\t" << getPrettyStatus() << "\t" << request_time << "\t\t" << "TBD" << "\t\t" << "TBD" << std::flush;
    }
    return log.str();
}

