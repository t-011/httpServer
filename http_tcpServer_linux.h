#ifndef HTTP_SERVER_LINUX
#define HTTP_SERVER_LINUX

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

#include <utils.h>

namespace http {

    std::ofstream LOG_FILE("log_file.txt");
    constexpr size_t MAX_NUM_CONNS = 20;

    class TcpServer {
    public:
        TcpServer(std::string, int);
        ~TcpServer();

    private:
        std::string m_ip_address;
        int m_port;
        int m_socket;
        int m_new_socket;
        long m_incomingMessage;
        sockaddr_in m_socketAddress; // in network order bytes
        unsigned int m_socketAddress_len;
        std::string m_serverMessage;

        void startServer();
        void closeServer();

        void startListen();
        void acceptConnection(int&);
    };
}







#endif