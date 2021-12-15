
#include "Server.h"

Server::Server(int port) noexcept(false): port(port) {
    // creating the socket
    int sockID = socket(AF_INET, SOCK_STREAM, 0);
    if (sockID < 0) {
        throw ("cannot open socket");
    }

    // creating the struct to hold info
    sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = port;

    // binding the socket
    int isBindError = bind(sockID, (struct sockaddr *) &serv, sizeof(serv));
    if (isBindError == -1) {
        throw ("cannot bind socket");
    }

    // start listening
    int isListenError = listen(sockID, 5);
    if (isListenError == -1) {
        throw ("cannot listen");
    }


}

void temp() {
    cout << "helo";
}

void Server::start(ClientHandler &ch) noexcept(false) {


    // Open thread for start.
    this->t = new thread([&ch](int clientID) {
        ch.handle(clientID);
    }, 1);

//    this->t = new thread(temp);

}

void Server::stop() {
    t->join(); // do not delete this!
}

Server::~Server() {
    delete (t);
}

// TODO: finish socket IO

string SocketIO::read() {
    return "hey you reed something";
}

void SocketIO::write(string text) {
    cout << "you write string: " << text << endl;
}

void SocketIO::write(float f) {
    cout << "you write float: " << to_string(f) << endl;
}

void SocketIO::read(float *f) {
    *f = 404;
}

SocketIO::SocketIO() {

}
