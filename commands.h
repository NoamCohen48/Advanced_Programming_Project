

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include <tuple>
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

struct Database {
    float threshold;
    vector<AnomalyReport> result;

};

// you may edit this class
class Command {
protected:
    DefaultIO *dio;
    Database *database;
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

// option 3
class detectAnomaliesCommand : public Command {
    string description = "detect anomalies\n";

    void execute() override {
        TimeSeries timeSeries("anomalyTrain.csv");

        HybridAnomalyDetector anomalyDetector;
        anomalyDetector.setPearsonThreshold(database->threshold);

        // learning
        anomalyDetector.learnNormal(timeSeries);

        // detecting
        timeSeries = TimeSeries("anomalyTest.csv");
        database->result = anomalyDetector.detect(timeSeries);

        // sending completed message
        dio->write("anomaly detection complete");
    }
};

// option 2
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

                // save the new threshold.
                database->threshold = wantedThreshold;
                break;
            }
            dio->write("please choose a value between 0 and 1.\n");
        }
    }
};

// option 4
class displayResultsCommand : public Command {
    string description = "display results\n";

    void execute() override {

        // Run through lines in the result vector and write them.
        for (const auto &line: database->result) {
            dio->write(to_string(line.timeStep) + "\t" + line.description + "\n");
        }

        // Write done.
        dio->write("Done.");
    }
};

// option 5
class uploadAnomaliesCommand : public Command {
    string description = "upload anomalies and analyze results\n";

    vector<tuple<int, int, bool>> initSequencesVec() {
        vector<tuple<int, int, bool>> result;
        const auto &anomalies = database->result;

        int start = anomalies[0].timeStep, end = start, curTimeStep;
        string endDesc = anomalies[0].description, curDesc;
        for (int i = 1; i < anomalies.size(); ++i){
            curTimeStep = anomalies[i].timeStep;
            curDesc = anomalies[i].description;

            if(curTimeStep == end + 1 && endDesc == curDesc){
                end = curTimeStep;
                continue;
            }

            tuple<int, int, bool> temp (start, end, false);
            result.push_back(temp);
            start = curTimeStep;
            end = start;
            endDesc = curDesc;
        }
        return result;
    }

    bool isTruePositive(int start, int end, vector<tuple<int, int, bool>> resultVector) {
        const auto &anomalies = database->result;

        // Run through all the lines in the result.
        for (auto& tuple : resultVector) {

            if (start >= get<0>(tuple) && end <= get<1>(tuple)) {
                get<2>(tuple) = true;
                return true;
            }
        }
        return false;
    }

    void execute() override {

        auto resultVector = initSequencesVec();
        string clientLine;
        int truePositive = 0;
        int falsePositive = 0;
        int sum = 0;
        int positive = 0;
        int n = 0;

        dio->write("Please upload your local anomalies file.\n");

        // Run through the received lines from the client.
        while ((clientLine = dio->read()) != "done") {

            // Get the start time.
            int start = stoi(clientLine.substr(0, clientLine.find(',')));

            // Get the end time.
            int end = stoi(clientLine.erase(0, start + 1));

            // Check if the received in the range.
            if (isTruePositive(start, end, resultVector)) {
                truePositive++;
            }

            // Summering the table length.
            sum = end - start + 1;
            positive++;
        }

        // Calculate the FP.
        for (auto& tuple : resultVector) {

            if (!get<2>(tuple)) {
                falsePositive++;
            }
        }

        // Calculate N.
        // TODO: get the table size (rows)
        //  n = database->testFileSize - sum;

        dio->write("Upload complete.\n");

        // Write TP/P.
        dio->write("True Positive Rate: " + to_string(truePositive / positive) + "\n");

        // Write FP/N.
        dio->write("False Positive Rate: " + to_string(falsePositive / n) + "\n");
    }
};

// option 6
class exitCommand : public Command {
    string description = "exit\n";

    void execute() override {
        //TODO: exit the program
    }
};

#endif /* COMMANDS_H_ */
