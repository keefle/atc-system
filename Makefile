build/atc: src/*.cpp src/*.hh
	mkdir -p build
	find src/  ! -name "*test.cpp" ! -name "*.hh" -name "*.cpp" | xargs g++ -g -Wall -lpthread -o build/atc


tower_test: src/tower_test.cpp
	g++ -g -Wall -lpthread src/sleep.cpp src/plane.cpp src/waitlist.cpp src/tower.cpp src/tower_test.cpp -o build/tower_test
	./build/tower_test

plane_test: src/plane_test.cpp
	g++ -g -Wall -lpthread  src/sleep.cpp src/plane_test.cpp src/plane.cpp -o build/plane_test
	./build/plane_test

waitlist_test: src/waitlist_test.cpp
	g++ -g -Wall -lpthread src/sleep.cpp src/plane.cpp src/waitlist_test.cpp src/waitlist.cpp -o build/waitlist_test
	./build/waitlist_test

clean:
	rm -f tags
	rm -rf build/*
