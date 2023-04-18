//
// Created by chenzhi on 2023/4/18.
//



#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//code from chatgpt

//以上是一个简单的reactor模型的C++实现示例，包括EventLoop、Channel和TcpServer三个类。EventLoop负责事件循环和事件的管理，
// Channel代表一个文件描述符的事件状态，TcpServer负责监听socket的管理。在使用时，可以根据实际需求添加业务逻辑处理函数，
// 例如在handleRead()和handleWrite()中处理具体的读写事件。请注意，这只是一个简单的示例，实际应用中还需要考虑错误处理、资源管理、并发等方面的内容。

typedef std::function<void()> ReadCallback;
typedef std::function<void()> WriteCallback;

const int MAX_EVENTS = 500;
const int BUFFER_SIZE = 4096;

class Channel;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);

private:
    int epoll_fd_;
    std::vector<struct epoll_event> events_;
    std::vector<Channel*> active_channels_;

    void handleEvents();
};

EventLoop::EventLoop() {
    epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
    events_.resize(MAX_EVENTS);
}

EventLoop::~EventLoop() {
    close(epoll_fd_);
}

void EventLoop::loop() {
    while (true) {
        handleEvents();
    }
}

void EventLoop::updateChannel(Channel* channel) {
    struct epoll_event event;
    event.events = channel->events_;
    event.data.ptr = channel;
    epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, channel->fd(), &event);
}

void EventLoop::removeChannel(Channel* channel) {
    epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, channel->fd(), NULL);
}

void EventLoop::handleEvents() {
    int num_events = epoll_wait(epoll_fd_, events_.data(), MAX_EVENTS, -1);
    for (int i = 0; i < num_events; ++i) {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        channel->handleRead();
    }
}

class Channel {
public:
    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handleRead();
    void handleWrite();

    int fd() const { return fd_; }
    void set_revents(int revt) { revents_ = revt; }
    bool isWriting() const { return events_ & EPOLLOUT; }
    void enableWriting() { events_ |= EPOLLOUT; }
    // 设置读事件回调函数
    void set_readCallback(const ReadCallback& cb) {
        readCallback_ = cb;
    }

    void set_writeCallback(const ReadCallback& cb) {
        writeCallback_ = cb;
    }

private:
    EventLoop* loop_;
    const int fd_;
    int events_;
    int revents_;

    ReadCallback readCallback_;  // 读事件回调函数
    WriteCallback writeCallback_;  // 写事件回调函数

    void update();
};

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0) {
}

Channel::~Channel() {
    update();
}

void Channel::handleRead() {
    // 处理读事件
}

void Channel::handleWrite() {
    // 处理写事件
}

void Channel::update() {
    loop_->updateChannel(this);
}

class TcpServer {
public:
    TcpServer(EventLoop* loop, const char* ip, int port);
    ~TcpServer();

    void listen();

private:
    EventLoop* loop_;
    int listen_fd_;
    Channel* listen_channel_;

    void handleRead();
};

TcpServer::TcpServer(EventLoop* loop, const char* ip, int port)
    : loop_(loop), listen_fd_(0), listen_channel_(nullptr) {
    // 创建监听socket并绑定地址
    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    bind(listen_fd_, (struct sockaddr*)&addr, sizeof(addr));
    listen_channel_ = new Channel(loop_, listen_fd_);
    listen_channel_->set_events(EPOLLIN);
    listen_channel_->set_readCallback(std::bind(&TcpServer::handleRead, this));
}

TcpServer::~TcpServer() {
    delete listen_channel_;
    close(listen_fd_);
}

void TcpServer::listen() {
    listen(listen_fd_, SOMAXCONN);
    loop_->updateChannel(listen_channel_);
}

void TcpServer::handleRead() {
// 处理监听socket的读事件
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int conn_fd = accept(listen_fd_, (struct sockaddr*)&client_addr, &client_len);
    Channel* conn_channel = new Channel(loop_, conn_fd);
    conn_channel->set_events(EPOLLIN | EPOLLOUT);
    conn_channel->set_readCallback(std::bind(&Channel::handleRead, conn_channel));
    conn_channel->set_writeCallback(std::bind(&Channel::handleWrite, conn_channel));
    loop_->updateChannel(conn_channel);
}

int main() {
    EventLoop loop;
    TcpServer server(&loop, "127.0.0.1", 8888);
    server.listen();
    loop.loop();
    return 0;
}
