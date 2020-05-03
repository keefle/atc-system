# Project 2 Report

## Building and Usage:

I'm using a makefile to build the project.

```bash
$ make # The executable will be located in ./build/atc
```

```bash
Usage: ./build/atc -n <log after n seconds> -s <simulation length> -p <probability>
```

## Implementation Idea

In the simulation there are two main types of objects.

1. Tower
2. Planes

A tower can receive requests from the planes. Once started the tower will start
processing those requests and figure out which plane should be allowed to use
the runway next.

> Pseudo code of Receiving Requests:

```cpp
// Add self to queue
...

// Inform the tower that a new plane arrived
cond_signal(&newly_arrived);
while (true) {
    // Check if we are next in line otherwise wait until the next announcement
    if(next_plane_id == plane->getID()) {
        // Remove self from the queue
        ...

        // Land/Takeoff (sleeping)
        ...

        // Release the lock and notify tower that the runway is empty again
        mutex_unlock(&checking_turn);
        cond_signal(&left_runway);

        // EXIT
        ...
    }

    // Else then wait for the next plane to be announced
    cond_wait(&next_plane_announced, &checking_turn);
}
```



The tower will update a variable containing the next plane's ID then broadcasts a
signal waking up all planes. Each of them will check if its ID matches the ID of
the variable announced by the tower. Only the one who has the same ID will
proceed to use the runway.

Once the plane is finished with the runway. It will inform the tower that it left
and that the runway is empty and can be used. The tower then will process the
remaining requests and newly made ones and determine which plane is next.

> Pseudo code of the main tower operation and accepting Requests:

```cpp
    mutex_lock(&checking_turn);
    while(not timeout) {

        // process_request figures out which plane is next and returns true if it
        // found one, false if it did not find any plane to schedule next.
        if (process_request()) {
            // In case there was a plane found the tower will wait for it to
            // leave the runway and return the lock.
            cond_wait(&left_runway, &checking_turn);
        } else {
            // In case there was NO plane found the tower will wait for new
            // planes to arrive.
            cond_wait(&newly_arrived, &checking_turn);
        }

    }
```

## How does the tower choose who's next? Is starvation possible?

Initially after the first two parts there was starvation due to the use of the
following rules.

1. No more planes is waiting to land,
2. 5 planes or more on the ground are lined up to take off.

If any of the above is true then the departing planes will be favoured to use the
runway. The starvation here is possible if we get an ever growing departing
queue (greater than 5). Departing planes will take all the priority forever and landing planes
will have to wait forever causing them to starve.

This can be avoided by using a method which grantees that every plane in the
queue will get its turn eventually. We can use something similar to a Round
Robbin in which we choose on turn by turn basis.

Example:
0: D
1: L
2: D
3: L
4: D
5: L

Here we switch between each queue type (DEPARTING/ARRIVING) and let them access
the runway equally.

However this solution won't be directly good for fuel consumption. What can be
done is allowing a count of each to proceed. For example for each 2 departing
planes we will allow 3 landing planes.

This will result in lower turnaround time for landing planes (conserving fuel)
while still guaranteeing every plane on the queues to get a turn on the runway.
