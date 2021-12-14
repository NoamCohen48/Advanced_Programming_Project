
#include "Server.h"

Server::Server(int port) noexcept(false) : port(port){
}

void temp(){
    cout<< "helo";
}

void Server::start(ClientHandler &ch) noexcept(false) {

    // Open thread for start.
//    this->t = new thread([&ch](int clientID){
//        ch.handle(clientID);
//    }, 1);

    this->t = new thread(temp);

}

void Server::stop() {
    t->join(); // do not delete this!
    delete(t);
}

Server::~Server() {
}

// TODO: finish socket IO

string SocketIO::read() {
    return "hey you reed something";
}

void SocketIO::write(string text) {
    cout << "you write string" + text << endl;
}

void SocketIO::write(float f) {
    cout << "you write float" + to_string(f) << endl;
}

void SocketIO::read(float *f) {
    *f = 200;
}
