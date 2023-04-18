//
// Created by chenzhi on 2023/4/18.
//


// 服务器端代码
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int main() {
    // 创建 Socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // 绑定地址
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888);
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to bind address" << std::endl;
        close(server_fd);
        return 1;
    }

    // 监听连接
    if (listen(server_fd, 5) == -1) {
        std::cerr << "Failed to listen" << std::endl;
        close(server_fd);
        return 1;
    }

    std::cout << "Server is listening on port 8888..." << std::endl;

    int n, i=0;
    char buf[1024] = {0};
    while (1) {
        // 接受连接
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_size);
        if (client_fd == -1) {
            std::cerr << "Failed to accept connection" << std::endl;
            close(server_fd);
            return 1;
        }

        n = read(client_fd, buf, sizeof(buf));
        if (n == 0) {
            cout << "client close!" << endl;
        }

        cout << i++ << "次收到数据=" << buf << endl;

        write(client_fd, buf, n);

    }
    close(server_fd);


    //std::cout << "Client connected: " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std
    return 0;
}