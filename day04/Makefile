CXX = g++
CXXFLAGS = -Wall -Wextra

OBJS = server.o util.o Epoll.o Socket.o

all: server client

server: server.o util.o Epoll.o Socket.o
	$(CXX) $(CXXFLAGS) -o $@ $^

client: client.o util.o Epoll.o Socket.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) server client
