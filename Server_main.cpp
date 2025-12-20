#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include "OrderBook.h"

void check(int status, std::string msg)
{
    if (status < 0)
    {
        std::cerr << "Error: " << msg << " (" << strerror(errno) << ")\n";
        exit(1);
    }
}
int main()
{
    // Initialize trading engine
    OrderBook book;
    std::cout << "Trading engine initialized.\n";

    int port = 54321;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    check(serverSocket, "Failed to create socket");

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    check(bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)), "Bind Failed");

    check(listen(serverSocket, 10), "listen failed");
    std::cout << "Waiting for traders on port: " << port << "...\n";

    sockaddr_in clientAddr;
    socklen_t clientSize = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientSize);
    check(clientSocket, "Accept Failed ");

    std::cout << "Trader connected!\n";

    char buffer[1024];
    while (true)
    {
        // clear buffer first
        memset(buffer, 0, 1024);

        int bytesRead = read(clientSocket, buffer, 1024);
        if (bytesRead <= 0)
        {
            std::cout << "Client disconnected.\n";
            break;
        }

        std::string input(buffer);
        std::stringstream ss(input);

        std::string action;
        int id, price, quantity;
        ss >> action >> id >> price >> quantity;

        Side side;
        if (action == "BUY")
            side = Side::BUY;
        else if (action == "SELL")
            side = Side::SELL;
        else
        {
            std::string msg = "Invalid Order.\n";
            write(clientSocket, msg.c_str(), msg.size());
            continue;
        }
        std::cout << "Processing: " << action << " " << price << " " << quantity << "\n";
        book.addOrder(id, price, quantity, side);

        std::string response = "ORDER_ACK " + std::to_string(id) + "\n";
        write(clientSocket, response.c_str(), response.size());

        book.printOrder();
        std::cout << "\n\n";
    }
    close(clientSocket);
    close(serverSocket);
    return 0;
}