

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO(){}

    // you may add additional methods here
    void readFile(string filePath){

    }
};

// you may add here helper classes


// you may edit this class
class Command{
protected:
    DefaultIO* dio;
public:
    Command(DefaultIO* dio):dio(dio){}
    virtual ~Command(){}
    virtual void execute()=0;
    string description;
};

// implement here your command classes
class uploadCSVCommand : public Command {
public:
    string description = "upload a time series csv file\n";
    void execute() override {
        dio->write("please upload your local train csv file.\n");


    }
};

class algorithmSettingsCommand : public Command {
    string description = "algorithm settings\n";
    void execute() override {

    }

};


#endif /* COMMANDS_H_ */
