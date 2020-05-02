#include <cassert>
#include <string>
#include <map>
#include "plane.hh"

void test_arriving_ids() {
    std::map<std::string,int> uuid_frequency;

    // LANDING PLANES
    for (int i = 0; i < 1000; i++) {
        Plane plane(ARRIVING);
        std::string current_uuid = plane.getID();
        uuid_frequency[current_uuid]++;

        // ID of landing planes must be even
        assert(std::stoi(current_uuid) % 2 == 0);

        // IDs should not repeat within a reasonable time
        assert(uuid_frequency[current_uuid] <= 1);
    }

    // EMERGENCY LANDING PLANES
    for (int i = 0; i < 1000; i++) {
        Plane plane(EMERGENCY);
        std::string current_uuid = plane.getID();
        uuid_frequency[current_uuid]++;

        // ID of landing planes must be even
        assert(std::stoi(current_uuid) % 2 == 0);

        // IDs should not repeat within a reasonable time
        assert(uuid_frequency[current_uuid] <= 1);
    }
}

void test_departing_ids() {
    std::map<std::string,int> uuid_frequency;

    // DEPARTING PLANES
    for (int i = 0; i < 1000; i++) {
        Plane plane(DEPARTING);
        std::string current_uuid = plane.getID();
        uuid_frequency[current_uuid]++;


        // ID of departing planes must be odd
        assert(std::stoi(current_uuid) % 2 == 1);

        // IDs should not repeat within a reasonable time
        assert(uuid_frequency[current_uuid] <= 1);
    }
}

int main() {
    test_arriving_ids();
    test_departing_ids();
}
