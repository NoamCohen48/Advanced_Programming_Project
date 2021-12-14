
#include "Server.h"

Server::Server(int port) throw(const char *) {

}

void Server::start(ClientHandler &ch) throw(const char *) {
}

void Server::stop() {
    t->join(); // do not delete this!
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
