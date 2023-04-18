//
// Created by chenzhi on 2023/4/18.
//


#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#define MAX_EVENTS 10
#define BUF_SIZE 1024



#define LISTEN_PORT 8888

int create_listen_socket() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);  // 创建 TCP 套接字
    if (listen_fd < 0) {
        std::cerr << "Failed to create listen socket" << std::endl;
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(LISTEN_PORT);

    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {  // 绑定地址和端口
        std::cerr << "Failed to bind listen socket" << std::endl;
        close(listen_fd);
        return -1;
    }

    if (listen(listen_fd, SOMAXCONN) < 0) {  // 开始监听连接
        std::cerr << "Failed to listen on listen socket" << std::endl;
        close(listen_fd);
        return -1;
    }

    std::cout << "Listen on port " << LISTEN_PORT << " ..." << std::endl;

    return listen_fd;
}

int main() {
    int listen_fd = create_listen_socket();  // 创建监听套接字
    if (listen_fd < 0) {
        std::cerr << "Failed to create listen socket" << std::endl;
        return -1;
    }

    int epoll_fd = epoll_create1(0);  // 创建 epoll 实例
    if (epoll_fd < 0) {
        std::cerr << "Failed to create epoll instance" << std::endl;
        return -1;
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = listen_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0) {  // 注册事件
        std::cerr << "Failed to add listen socket to epoll" << std::endl;
        return -1;
    }

    struct epoll_event events[MAX_EVENTS];

    while (true) {
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);  // 等待事件
        if (num_events < 0) {
            std::cerr << "Failed to wait for events" << std::endl;
            return -1;
        }

        for (int i = 0; i < num_events; ++i) {
            if (events[i].data.fd == listen_fd) {
                int client_fd = accept(listen_fd, nullptr, nullptr);
                if (client_fd < 0) {
                    std::cerr << "Failed to accept client connection" << std::endl;
                    return -1;
                }

                std::cout << "Accepted client connection, fd = " << client_fd << std::endl;

                ev.events = EPOLLIN;
                ev.data.fd = client_fd;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {  // 注册客户端事件
                    std::cerr << "Failed to add client socket to epoll" << std::endl;
                    return -1;
                }
            } else {
                int client_fd = events[i].data.fd;
                char buf[BUF_SIZE];
                ssize_t n = recv(client_fd, buf, BUF_SIZE, 0);
                if (n < 0) {
                    std::cerr << "Failed to receive data from client" << std::endl;
                    return -1;
                } else if (n == 0) {
                    std::cout << "Client closed connection, fd = " << client_fd << std::endl;
                    close(client_fd);
                } else {
                    write(client_fd, buf, n);
                    std::cout << "Received data from client, fd = " << client_fd << ", data: " << buf << std::endl;
                }
            }
        }
    }

    return 0;
}