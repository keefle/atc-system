#include "tower.hh"

Tower::Tower() {
    log_file.open ("planes.log");
    log_file << "Plane ID\t"  << "Status\t" << "Request Time\t" << "Runway Time\t" << "Turnaround Time" << std::endl;
    log_file.flush();

    if (pthread_mutex_init(&checking_turn, NULL) != 0) {
        throw "tower: failed to init mutex";
    }
    if (pthread_cond_init(&next_plane_announced, NULL) != 0) {
        throw "tower: failed to init runway_empty condition";
    }
    if (pthread_cond_init(&left_runway, NULL) != 0) {
        throw "tower: failed to init left_runway condition";
    }
    if (pthread_cond_init(&newly_arrived, NULL) != 0) {
        throw "tower: failed to init newly_arrived condition";
    }
};


int Tower::size_waiting() {
    return wl.size_waiting();
}

int Tower::size_waiting(PlaneStatus status) {
    return wl.size_waiting(status);
}

std::string Tower::list_waiting(PlaneStatus status) {
    return wl.list_waiting(status);
}

void Tower::shutdown() {
    timeout = true;
}


void Tower::request(Plane *plane) {
    wl.add(plane);

    pthread_cond_signal(&newly_arrived);
    while (true) {
        // Check if we are next in line otherwise wait until the next announcement
        if(next_plane_id == plane->getID()) {
            // Remove self from the queue of planes of same status
            wl.pop(plane->getStatus());

            // Use the runway (land/takeoff)
            plane->perform_action();

            // Record result to log file
            log_file << plane->log() << std::endl;
            log_file.flush();

            // Remove self from memory
            delete plane;

            // Release the lock and notify tower that the runway is empty again
            pthread_mutex_unlock(&checking_turn);
            pthread_cond_signal(&left_runway);
            return;
        }
        pthread_cond_wait(&next_plane_announced, &checking_turn);
    }
}

void Tower::start() {
    pthread_mutex_lock(&checking_turn);
    while(not timeout) {
        if (process_request()) {
            pthread_cond_wait(&left_runway, &checking_turn);
        } else {
            pthread_cond_wait(&newly_arrived, &checking_turn);
        }
    }

    // Log remaining
    while (wl.size_waiting(EMERGENCY)) {
        Plane *current = wl.front(EMERGENCY);
        log_file << current->log() << std::endl;
        wl.pop(EMERGENCY);
        delete current;
    }

    while (wl.size_waiting(ARRIVING)) {
        Plane *current = wl.front(ARRIVING);
        log_file << current->log() << std::endl;
        wl.pop(ARRIVING);
        delete current;
    }

    while (wl.size_waiting(DEPARTING)) {
        Plane *current = wl.front(DEPARTING);
        log_file << current->log() << std::endl;
        wl.pop(DEPARTING);
        delete current;
    }

    log_file.close();
}

bool Tower::process_request() {

    if (wl.size_waiting(EMERGENCY) > 0) {
        next_plane_id = wl.front(EMERGENCY)->getID();
    } else if (wl.size_waiting(ARRIVING) > 0 && (arriving_turn || wl.size_waiting(DEPARTING) == 0)) {
        next_plane_id = wl.front(ARRIVING)->getID();
        if (++consecutive_arriving >= 3) {
            arriving_turn = false;
            consecutive_arriving = 0;
        }
    } else if(wl.size_waiting(DEPARTING) > 0 && (not arriving_turn || wl.size_waiting(ARRIVING) == 0)) {
        next_plane_id = wl.front(DEPARTING)->getID();
        if (++consecutive_departing >= 2) {
            arriving_turn = true;
            consecutive_departing = 0;
        }
    } else {
        return false;
    }

    // PART 2
    // if (wl.size_waiting(EMERGENCY) > 0) {
    //     next_plane_id = wl.front(EMERGENCY)->getID();
    // } else if (wl.size_waiting(ARRIVING) > 0 && wl.size_waiting(DEPARTING) < 5) {
    //     next_plane_id = wl.front(ARRIVING)->getID();
    // } else if(wl.size_waiting(DEPARTING) > 0) {
    //     next_plane_id = wl.front(DEPARTING)->getID();
    // } else {
    //     return false;
    // }

    // PART 1
    // if (wl.size_waiting(EMERGENCY) > 0) {
    //     next_plane_id = wl.front(EMERGENCY)->getID();
    // } else if (wl.size_waiting(ARRIVING) > 0) < 5) {
    //     next_plane_id = wl.front(ARRIVING)->getID();
    // } else if(wl.size_waiting(DEPARTING) > 0) {
    //     next_plane_id = wl.front(DEPARTING)->getID();
    // } else {
    //     return false;
    // }

    pthread_cond_broadcast(&next_plane_announced);
    return true;
}

