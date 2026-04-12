#include "server/asio/tcp_server.h"
#include "threads/thread.h"

#include <iostream>

class ChatSession : public CppServer::Asio::TCPSession
{
public:
    using CppServer::Asio::TCPSession::TCPSession;

protected:
    void onConnected() override
    {
        std::cout << "Chat TCP session with Id " << id() << " connected!" << std::endl;

        std::string message("Hello from TCP chat! Please send a message or '!' to disconnect the client!");
        SendAsync(message);
    }

    void onDisconnected() override
    {
        std::cout << "Chat TCP session with Id " << id() << " disconnected!" << std::endl;
    }

    void onReceived(const void* buffer, size_t size) override
    {
        std::string message((const char*)buffer, size);
        std::cout << "Incoming: " << message << std::endl;

        server()->Multicast(message);

        if (message == "!")
            DisconnectAsync();
    }

    void onError(int error, const std::string& category, const std::string& message) override
    {
        std::cout << "Chat TCP session caught an error with code " << error << " and category '" << category << "': " << message << std::endl;
    }
};

class ChatServer : public CppServer::Asio::TCPServer
{
public:
    using CppServer::Asio::TCPServer::TCPServer;

protected:
    std::shared_ptr<CppServer::Asio::TCPSession> CreateSession(std::shared_ptr<CppServer::Asio::TCPServer> server) override
    {
        return std::make_shared<ChatSession>(server);
    }

protected:
    void onError(int error, const std::string& category, const std::string& message) override
    {
        std::cout << "Chat TCP server caught an error with code " << error << " and category '" << category << "': " << message << std::endl;
    }
};

int main(int argc, char** argv)
{
    int port = 1111;
    if (argc > 1)
        port = std::atoi(argv[1]);

    std::cout << "TCP server port: " << port << std::endl;

    auto service = std::make_shared<CppServer::Asio::Service>();

    std::cout << "Asio service starting...";
    service->Start();
    std::cout << "Done!" << std::endl;

    auto server = std::make_shared<ChatServer>(service, port);

    std::cout << "Server starting...";
    server->Start();
    std::cout << "Done!" << std::endl;

    std::cout << "Press Enter to stop the server or '!' to restart the server..." << std::endl;

    std::string line;
    while (getline(std::cin, line))
    {
        if (line.empty())
            break;

        if (line == "!")
        {
            std::cout << "Server restarting...";
            server->Restart();
            std::cout << "Done!" << std::endl;
            continue;
        }

        line = "(admin) " + line;
        server->Multicast(line);
    }

    std::cout << "Server stopping...";
    server->Stop();
    std::cout << "Done!" << std::endl;

    std::cout << "Asio service stopping...";
    service->Stop();
    std::cout << "Done!" << std::endl;

    return 0;
}