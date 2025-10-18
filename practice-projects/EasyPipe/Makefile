all:client server

client:client.cpp NamedPipe.cpp
	g++ -o $@ $^ -std=c++11
server:server.cpp NamedPipe.cpp
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -r -f client server
