
#include "Server.h"

Server::Server(int port) noexcept(false): port(port) {
    // creating the socket
    sockID = socket(AF_INET, SOCK_STREAM, 0);
    if (sockID < 0) {
        throw ("cannot open socket");
    }

    // creating the struct to hold info
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // binding the socket
    int isBindError = bind(sockID, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (isBindError == -1) {
        throw ("cannot bind socket");
    }

    // start listening
    int isListenError = listen(sockID, 5);
    if (isListenError == -1) {
        throw ("cannot listen");
    }

    isStopped = false;
}


void sigHandler(int sigNum) {
    cout << "sidH" << endl;
}


void Server::start(ClientHandler &ch) noexcept(false) {
    // Open thread for start.
    this->t = new thread([&ch, this]() {
        signal(SIGALRM, sigHandler);
        while (!isStopped) {
            // trying to accept a connection
            socklen_t socketIDLen = sizeof(sockID);

            alarm(1);
            int clientID = accept(sockID, (struct sockaddr *) &clientAddr, &socketIDLen);
            if (clientID > 0) {
                ch.handle(clientID);
                close(clientID);
            }
            alarm(0);
        }
    });
}


void Server::stop() {
    isStopped = true;
    close(sockID);
    t->join(); // do not delete this!
}

Server::~Server() {
    delete (t);
}


string SocketIO::read() {
    // reading a char every time
    char buffer = 0;
    string result;

    // reading first
    //recv(clientID, &buffer, sizeof(char), 0);
    while (buffer != '\n') {
        recv(clientID, &buffer, sizeof(char), 0);
        result += buffer;
    }

    return result;
}

void SocketIO::write(string text) {
    send(clientID, text.c_str(), text.size(), 0);
}

void SocketIO::write(float f) {
    string text = to_string(f);
    write(text);
}

void SocketIO::read(float *f) {
    // reading the line
    //string text = read();

    // converting the line to float and inserting into pointer
    //*f = stof(text);
}

