all: tunServer tunClient

tunServer: server.cpp
	g++ -o tunServer server.cpp
tunClient: client.cpp
	g++ -o tunClient client.cpp
clean:
	rm tunServer tunClient
