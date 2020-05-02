# Requirements
Creating a deadlock free Air Traffic Control system (ATC).


# Command
* Randomness: Flag `-p`
* Simulation Time: Flag `-s`

# Parts
* World
    - Planes enter the world with probability `p` each `t` seconds
    - At time 0 one plane is Arriving and one plane is Departing

* Runway
    - Only one runway
    - Supports Takeoff and Landing
    - Valid?
        - Only one plane at all times
        - Takeoff/Landig will lock the Runway
* Tower
    - Only one tower
    - Favour Arriving planes (fuel efficiency) until no more exist then allows a
      single Departing plane then favour Arriving planes again
    - Code:
        - request(status)
        - wait until signaled, get key, wait again

* Plane
    - Takes `2t` to Land or Takeoff
    - Arriving or Departing Planes request Tower to Takeoff or Land
    - Becomes ready to take off with probability `1-p` each `t` seconds and
      requests the tower
    - Cannot land without acknowledgment from Tower
