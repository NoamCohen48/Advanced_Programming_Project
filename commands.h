

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    virtual ~DefaultIO() {}

    // you may add additional methods here
    void readFile(const string &filePath) {
        ofstream file(filePath);
        string line = read();
        while (line != "done") {
            file << line;
        }
        file.close();
    }
};

// you may add here helper classes


// you may edit this class
class Command {
protected:
    DefaultIO *dio;
public:
    Command(DefaultIO *dio) : dio(dio) {}

    virtual ~Command() {}

    virtual void execute() = 0;

    string description;
};

// implement here your command classes

// option 1
class uploadCSVCommand : public Command {
public:
    string description = "upload a time series csv file\n";

    void execute() override {
        // get train csv
        dio->write("please upload your local train csv file.\n");
        dio->readFile("anomalyTrain.csv");
        dio->write("Upload complete.\n");

        // get test csv
        dio->write("please upload your local test csv file.\n");
        dio->readFile("anomalyTest.csv");
        dio->write("Upload complete.\n");
    }
};

class detectAnomaliesCommand : public Command {
    string description = "detect anomalies\n";

    void execute() override {
        TimeSeries timeSeries("anomalyTrain.csv");

        // TODO: assign new threshold to detector
        HybridAnomalyDetector anomalyDetector;

        // learning
        anomalyDetector.learnNormal(timeSeries);

        // detecting
        timeSeries = TimeSeries("anomalyTest.csv");
        anomalyDetector.detect(timeSeries);
    }

};

class algorithmSettingsCommand : public Command {
    string description = "algorithm settings\n";

    void execute() override {

        float wantedThreshold;

        // Get the pearson threshold.
        double pearsonThreshold = (new HybridAnomalyDetector())->getPearsonThreshold();

        while (true) {

            // Write to the client.
            dio->write("The current correlation threshold is " + to_string(pearsonThreshold) + "\n");

            // Get wanted threshold from the client.
            dio->read(&wantedThreshold);

            // Check if the Threshold is between 0 and 1.
            if (wantedThreshold >= 0 && wantedThreshold <= 1) {

                // TODO save new Threshold.
                break;
            }
            dio->write("please choose a value between 0 and 1.\n");
        }
    }
};

#endif /* COMMANDS_H_ */
