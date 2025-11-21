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
    m_serverMessage(buildResponse())
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

        if (startServer() != 0) {
            std::ostringstream ss;
            ss << "Failed to start server with PORT: " << ntohs(m_socketAddress.sin_port);
            log(ss.str(), LOG_FILE);
        }
    }

    TcpServer::~TcpServer() {
        LOG_FILE.close();
        closeServer();
    }

    int TcpServer::startServer() {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);

        if (m_socket < 0) exitWithError("Cannot create socket");
        if (bind(m_socket, (sockaddr*) &m_socketAddress, m_socketAddress_len) < 0) {
            exitWithError("Cannot connect socket to address");
            return 1;
        }
        return 0;
    }

    void TcpServer::closeServer() {
        if (close(m_socket) < 0) {
            exitWithError("Couldn't close socket: " + 
                std::to_string(m_socket));
        }
        if (close(m_new_socket) < 0) {
            exitWithError("Couldn't close socket: " + 
                std::to_string(m_new_socket));
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

        int bytesReceived;

        while (true) {
            log("====== Waiting for a new connection ======\n\n\n", LOG_FILE);
            acceptConnection(m_new_socket);

            char buffer[BUFFER_SIZE] = {0};
            bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);
            if (bytesReceived < 0)
            {
                exitWithError("Failed to read bytes from client socket connection");
            }

            std::ostringstream ss;
            ss << "------ Received Request from client ------\n\n";
            log(ss.str(), LOG_FILE);

            sendResponse();

            close(m_new_socket);
        }
    }

    void TcpServer::acceptConnection(int& new_socket) {
        new_socket = accept(m_socket, (sockaddr*) &m_socketAddress.sin_addr, &m_socketAddress_len);

        if (new_socket < 0)
        {
            std::ostringstream ss;
            ss << 
            "\n\n"
            << "SERVER FAILED to accept incoming connection from ADDRESS: " 
            << inet_ntoa(m_socketAddress.sin_addr) << "; PORT: " 
            << ntohs(m_socketAddress.sin_port)
            << "\n\n";
            exitWithError(ss.str());
        }

    }

    std::string TcpServer::buildResponse() {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        return ss.str();
    }

    void TcpServer::sendResponse() {
        long bytesSent;

        bytesSent = write(m_new_socket, m_serverMessage.c_str(), m_serverMessage.size());

        if (bytesSent == m_serverMessage.size())
        {
            log("------ Server Response sent to client ------\n\n", LOG_FILE);
        }
        else
        {
            log("Error sending response to client", LOG_FILE);
        }
    }
}
