/*
 * Created by:
 * Shaked Cohen, Shakedc159@gmail.com.
 * Noam Cohen, cohennoam48@gmail.com.
*/

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
struct seqResult{
    int start;
    int end;
    string description;
    bool truePositive;
};

struct Database {
    float threshold;
    vector<AnomalyReport> result;
    vector<seqResult> seqResults;
    int numOfRows;

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

// option 3
class detectAnomaliesCommand : public Command {
    string description = "detect anomalies\n";

    void initSequencesVec() {
        seqResult result;
        result.start = 0;
        result.end = 0;
        result.truePositive = false;
        result.description = "";

        // Run through the lines in the database.
        for(auto& line : database->result) {

            // Check if the last line where in the same description and next timeStep.
            if(line.timeStep == result.end+1 && line.description == result.description) {
                result.end++;
            } else {

                // Else update the result to point the next line.
                database->seqResults.push_back(result);
                result.start = line.timeStep;
                result.end = result.start;
                result.description = line.description;
            }
        }

        // Push the last result.
        database->seqResults.push_back(result);

        // Delete the first result (always empty).
        database->seqResults.erase(database->seqResults.begin());
    }

    void execute() override {
        TimeSeries timeSeries("anomalyTrain.csv");

        HybridAnomalyDetector anomalyDetector;
        anomalyDetector.setPearsonThreshold(database->threshold);

        // learning
        anomalyDetector.learnNormal(timeSeries);

        // detecting
        timeSeries = TimeSeries("anomalyTest.csv");
        database->result = anomalyDetector.detect(timeSeries);

        // Get the size.
        database->numOfRows = timeSeries.getRowsSize();

        // sending completed message.
        dio->write("anomaly detection complete");
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

    bool isTruePositive(int start, int end) {
        const auto &anomalies = database->result;

        // Run through all the lines in the result.
        for (auto& line : database->seqResults) {

            if (start >= line.start && end <= line.end) {
                line.truePositive = true;
                return true;
            }
        }
        return false;
    }

    void execute() override {

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
            if (isTruePositive(start, end)) {
                truePositive++;
            }

            // Summering the table length.
            sum = end - start + 1;
            positive++;
        }

        // Calculate the FP.
        for (auto& line : database->seqResults) {

            if (!line.truePositive) {
                falsePositive++;
            }
        }

        // Calculate N.
        n = database->numOfRows - sum;

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
