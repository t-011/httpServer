.default: all

all: server

clean:
	rm server

server: server_linux.cpp http_tcpServer_linux.cpp
	g++ -o server server_linux.cpp http_tcpServer_linux.cpp
