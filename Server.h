/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <thread>
#include "commands.h"
#include "CLI.h"


using namespace std;

// edit your ClientHandler interface here:
class ClientHandler {
public:
    virtual void handle(int clientID) = 0;
};


class SocketIO : public DefaultIO {
public:
    string read() override;

    void write(string text) override;

    void write(float f) override;

    void read(float *f) override;

    ~SocketIO() override = default;

};
// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler : public ClientHandler {
public:
    virtual void handle(int clientID) {

        // Create the socket and start the CLI.
        SocketIO socket;
        CLI cli(&socket);
        cli.start();
    }
};


// implement on Server.cpp
class Server {
    int port;
    int clientID;
    thread *t; // the thread to run the start() method in

    // you may add data members

public:
    Server(int port) throw(const char *);

    virtual ~Server();

    void start(ClientHandler &ch) throw(const char *);

    void stop();
};

#endif /* SERVER_H_ */
