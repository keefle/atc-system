#include "waitlist.hh"
#include <sstream>

wait_list::wait_list()
{
    pthread_mutex_init(&lock, NULL);
}

void wait_list::add(Plane *plane)
{
    pthread_mutex_lock(&lock);

    switch (plane->getStatus())
    {
        case ARRIVING:
            arrive_wait_list.push(plane);
            pthread_mutex_unlock(&lock);
            return;
        case DEPARTING:
            depart_wait_list.push(plane);
            pthread_mutex_unlock(&lock);
            return;
        case EMERGENCY:
            emergency_wait_list.push(plane);
            pthread_mutex_unlock(&lock);
            return;
        default:
            pthread_mutex_unlock(&lock);
            throw "plane with unknown status. Cannot add to wait list";
    }
}

void wait_list::pop(PlaneStatus status)
{
    pthread_mutex_lock(&lock);

    switch (status)
    {
        case ARRIVING:
            arrive_wait_list.pop();
            pthread_mutex_unlock(&lock);
            return;
        case DEPARTING:
            depart_wait_list.pop();
            pthread_mutex_unlock(&lock);
            return;
        case EMERGENCY:
            emergency_wait_list.pop();
            pthread_mutex_unlock(&lock);
            return;
        default:
            pthread_mutex_unlock(&lock);
            throw "plane with unknown status. Cannot add to wait list";
    }
}

bool wait_list::is_front(Plane *plane)
{
    bool is_front;
    pthread_mutex_lock(&lock);
    switch (plane->getStatus())
    {
        case ARRIVING:
            is_front= plane->getID() == arrive_wait_list.front()->getID();
            pthread_mutex_unlock(&lock);
            return is_front;
        case DEPARTING:
            is_front= plane->getID() == depart_wait_list.front()->getID();
            pthread_mutex_unlock(&lock);
            return is_front;
        case EMERGENCY:
            is_front= plane->getID() == emergency_wait_list.front()->getID();
            pthread_mutex_unlock(&lock);
            return is_front;
        default:
            pthread_mutex_unlock(&lock);
            throw "plane with unknown status. Cannot queue";
    }
}

Plane *wait_list::front(PlaneStatus status)
{
    Plane *next;
    pthread_mutex_lock(&lock);
    switch (status)
    {
        case ARRIVING:
            next = arrive_wait_list.front();
            pthread_mutex_unlock(&lock);
            return next;
        case DEPARTING:
            next = depart_wait_list.front();
            pthread_mutex_unlock(&lock);
            return next;
        case EMERGENCY:
            next = emergency_wait_list.front();
            pthread_mutex_unlock(&lock);
            return next;
        default:
            pthread_mutex_unlock(&lock);
            throw "plane with unknown status. Cannot get front";
    }
}

int wait_list::size_waiting() {
    pthread_mutex_lock(&lock);
    int waiting_size = arrive_wait_list.size() + depart_wait_list.size() + emergency_wait_list.size();
    pthread_mutex_unlock(&lock);
    return waiting_size;
}

int wait_list::size_waiting(PlaneStatus status) {
    int waiting_size;
    pthread_mutex_lock(&lock);
    switch (status) {
        case ARRIVING:
            waiting_size = arrive_wait_list.size();
            break;
        case DEPARTING:
            waiting_size = depart_wait_list.size();
            break;
        case EMERGENCY:
            waiting_size = emergency_wait_list.size();
            break;
        default:
            pthread_mutex_unlock(&lock);
            throw "error: could not get queue size because plane status is unknown";
    }
    pthread_mutex_unlock(&lock);
    return waiting_size;
}

std::string wait_list::list_waiting(PlaneStatus status) {
    std::ostringstream list;
    std::queue<Plane*> temp;
    pthread_mutex_lock(&lock);
    switch (status) {
        case ARRIVING:
            temp = arrive_wait_list;
            break;
        case DEPARTING:
            temp = depart_wait_list;
            break;
        case EMERGENCY:
            temp = emergency_wait_list;
            break;
        default:
            pthread_mutex_unlock(&lock);
            throw "error: could not get queue size because plane status is unknown";
    }

    long unsigned int size = temp.size();
    for(long unsigned int i = 0; i < size ; i++) {
        list << temp.front()->getPrettyID() << ", ";
        temp.pop();
    }

    pthread_mutex_unlock(&lock);

    return list.str();
}
