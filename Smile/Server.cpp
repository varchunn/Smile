#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define SERVER_PORT 12345

int main() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }
    
    sockaddr_in server, client;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_PORT);
    
    if (bind(server_sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_sock);
        return 1;
    }
    
    listen(server_sock, 3);
    
    socklen_t c = sizeof(sockaddr_in);
    int client_sock = accept(server_sock, (struct sockaddr*)&client, &c);
    if (client_sock < 0) {
        std::cerr << "Accept failed" << std::endl;
        close(server_sock);
        return 1;
    }
    
    int x = 0, y = 0;
    char buffer[20];
    
    while (true) {
        recv(client_sock, buffer, sizeof(buffer), 0);
        sscanf(buffer, "%d %d", &x, &y);
        system("clear");
        std::cout << "Smiley received at: (" << x << ", " << y << ")" << std::endl;
    }
    
    close(client_sock);
    close(server_sock);
    return 0;
}

