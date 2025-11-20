#include "http_tcpServer_linux.h"



namespace http {
    TcpServer::TcpServer(std::string ip_address, int port)
    : m_ip_address(ip_address), 
    m_port(port),
    m_socket(),
    m_new_socket(),
    m_incomingMessage(),
    m_socketAddress(),
    m_socketAddress_len(sizeof(m_socketAddress)),
    m_serverMessage() // buildResponse()
    {
        // Assigning socket -> ip:port
        {
            m_socketAddress.sin_family = AF_INET;
            //htons() converts a short from host byte order to network byte order
            m_socketAddress.sin_port = htons(m_port);
            //inet_addr() converts char* to long in network byte order
            //0.0.0.0 is a default IP that lets the operating system choose any IP it wants
            m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
        }

        startServer();
    }

    TcpServer::~TcpServer() {
        LOG_FILE.close();
        closeServer();
    }

    void TcpServer::startServer() {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);

        if (m_socket < 0) exitWithError("Cannot create socket");
        if (bind(m_socket, (sockaddr*) &m_socketAddress, m_socketAddress_len) < 0) {
            exitWithError("Cannot connect socket to address");
        }
    }

    void TcpServer::closeServer() {
        if (close(m_socket) < 0) {
            exitWithError("Couldn't close socket: " + 
                std::to_string(m_socket));
        }
    }

    void TcpServer::startListen() {
        if (listen(m_socket, MAX_NUM_CONNS)) {
            exitWithError("Socket listen failed");
        }

        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: "
           << inet_ntoa(m_socketAddress.sin_addr)
           << " PORT: " << ntohs(m_socketAddress.sin_port)
           << " ***\n\n";
        log(ss.str(), LOG_FILE);
    }
}
