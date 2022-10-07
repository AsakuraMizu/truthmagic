CXX=clang++
CXXFLAGS=-std=c++17

.PHONY: run clean

run: main
	./main

main: main.cpp texttable.hpp truth.hpp
	$(CXX) $< -o $@ $(CXXFLAGS)

clean:
	rm -f main
