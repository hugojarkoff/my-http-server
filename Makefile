CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2

.PHONY: all clean run-server run-client

# default target - build everything
all: server client

server: server.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

client: client.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

run-server: server
	./server

run-client: client
	./client

clean:
	rm -f server client
