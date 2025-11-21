#ifndef HTTP_SERVER_LINUX
#define HTTP_SERVER_LINUX

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

#include "utils.h"

namespace http {

    class TcpServer {
        const size_t MAX_NUM_CONNS = 20;
        const size_t BUFFER_SIZE = 30720;

    public:
        TcpServer(std::string ip_address, int port);
        ~TcpServer();

        void startListen();

    private:
        std::string m_ip_address;
        int m_port;
        int m_socket;
        int m_new_socket;
        long m_incomingMessage;
        sockaddr_in m_socketAddress; // in network order bytes
        unsigned int m_socketAddress_len;
        std::string m_serverMessage;

        int startServer();
        void closeServer();
        
        void acceptConnection(int&);
        std::string buildResponse();
        void sendResponse();
    };
}







#endif