#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    // 创建 Socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // 连接服务端
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 服务器 IP 地址
    server_addr.sin_port = htons(8888); // 服务器端口号
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
        close(client_fd);
        return 1;
    }

    std::cout << "Connected to server" << std::endl;

    while (true) {
        // 发送数据
        std::string message = "Hello, server!";
        if (send(client_fd, message.c_str(), message.length(), 0) == -1) {
            std::cerr << "Failed to send data" << std::endl;
            close(client_fd);
            return 1;
        }

        std::cout << "Sent message: " << message << std::endl;

        // 接收数据
        char buffer[1024] = {0};
        int num_bytes = recv(client_fd, buffer, sizeof(buffer), 0);
        if (num_bytes == -1) {
            std::cerr << "Failed to receive data" << std::endl;
            close(client_fd);
            return 1;
        }

        std::cout << "Received message: " << buffer << std::endl;
    }


    // 关闭 Socket
    close(client_fd);

    return 0;
}