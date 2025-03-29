#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <termios.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

void sendCoordinates(int sock, int x, int y) {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%03d %03d", x, y);
    send(sock, buffer, sizeof(buffer), 0);
}

char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }
    
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);
    
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(sock);
        return 1;
    }
    
    int x = 0, y = 0;
    while (true) {
        system("clear");
        std::cout << "Smiley position: (" << x << ", " << y << ")" << std::endl;
        char ch = getch();
        if (ch == 27) break;
        if (ch == 'a') x--; // Left
        if (ch == 'd') x++; // Right
        if (ch == 'w') y--; // Up
        if (ch == 's') y++; // Down
        
        sendCoordinates(sock, x, y);
    }
    
    close(sock);
    return 0;
}
